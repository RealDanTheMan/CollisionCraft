#include "viewportwidget.h"
#include "logging.h"
#include "rendermesh.h"

#include <algorithm>
#include <cmath>
#include <csignal>
#include <memory>
#include <vector>

#include <Qt>
#include <QVector3D>
#include <QSurfaceFormat>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Min_sphere_of_spheres_d.h>
#include <CGAL/Min_sphere_of_spheres_d_traits_d.h>

using CGAL_Kernel = CGAL::Simple_cartesian<double>;
using CGAL_Traits = CGAL::Min_sphere_of_spheres_d_traits_3<CGAL_Kernel,double>;
using CGAL_Point3 = CGAL_Kernel::Point_3;
using CGAL_Sphere = CGAL_Traits::Sphere;
using CGAL_MinSphere = CGAL::Min_sphere_of_spheres_d<CGAL_Traits>;

ViewportWidget::ViewportWidget(QWidget *parent) : 
	QOpenGLWidget(parent),
	aspect(0.0),
	fov(45.0),
	cam_mode(ViewportWidget::CameraMode::Default),
	cam_sensitivity(0.0174533),
	cam_yaw(0.0),
	cam_pitch(0.0)
{
	this->graphics = std::make_unique<Graphics>();
	this->mat_view.setToIdentity();
	this->mat_perspective.setToIdentity();
	this->mat_model.setToIdentity();
}

/// Clear this viewport render queue / empty the viewport.
void ViewportWidget::clearRenderMeshes()
{
	this->render_queue.clear();
}

/// Add render mesh to this viewport render queue.
///
/// Meshes in the render queue will re-draw each time the viewport paint event is called.
/// @param: mesh Render mesh to add to the queue.
void ViewportWidget::addRenderMesh(RenderMesh *mesh)
{
	this->render_queue.push_back(mesh);
}

/// Update camera view transform to envelop currently loaded meshes.
void ViewportWidget::autoFrameCamera()
{
	if (this->render_queue.size() == 0)
	{
		return;
	}

	float v_fov = qDegreesToRadians(this->fov);
	float h_fov = 2.0f * std::atan(std::tan(v_fov * 0.5f) * this->aspect);

	QVector3D scene_center(0.0, 0.0, 0.0);
	float scene_radius;
	computeSceneBoundingSphere(scene_center, scene_radius);

	logWarning("Scene center -> x={} y={} z={}", scene_center.x(), scene_center.y(), scene_center.z());
	logWarning("Scene radius -> {}", scene_radius);

	float v_dist = scene_radius / std::tan(v_fov * 0.5);
	float h_dist = scene_radius / std::tan(h_fov * 0.5);
	float dist = std::max(v_dist, h_dist);

	QVector3D forward = QVector3D(0.0, 0.0, -1.0);
	QVector3D up = QVector3D(0.0, 1.0, 0.0);
	QVector3D cam_pos = scene_center - forward * dist;

	this->mat_view.setToIdentity();
	this->mat_view.lookAt(cam_pos, scene_center, up);
	this->cam_focus = scene_center;
}

/// Compute bounding sphere that envelops currently loaded render meshes.
void ViewportWidget::computeSceneBoundingSphere(QVector3D &center, float &radius) const
{
	std::vector<CGAL_Sphere> scene_spheres;
	for (const RenderMesh *mesh : this->render_queue)
	{
		QVector3D mesh_center = mesh->getBoundingShereCenter();
		float mesh_radius = mesh->getBoundingShereRadius();

		CGAL_Point3 p(mesh_center.x(), mesh_center.y(), mesh_center.z());
		scene_spheres.emplace_back(p, mesh_radius * mesh_radius);
	}

	CGAL_MinSphere sphere(scene_spheres.begin(), scene_spheres.end());

	double x = static_cast<double>(*(sphere.center_cartesian_begin() + 0));
	double y = static_cast<double>(*(sphere.center_cartesian_begin() + 1));
	double z = static_cast<double>(*(sphere.center_cartesian_begin() + 2));

	center = QVector3D(x, y, z);
	radius = std::sqrt(sphere.radius());
}

/// Initialize OpenGL graphics context for this widget.
void ViewportWidget::initializeGL()
{
    initializeOpenGLFunctions();

	Logger::active()->debug("Initialising graphics");
	if (!this->graphics->init())
	{
		Logger::active()->error("Failed to initialise graphics!");
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
    glClearColor(
        this->background_color.redF(),
        this->background_color.greenF(),
        this->background_color.blueF(),
        1.0f
    );

	this->resizeGL(this->width(), this->height());
	this->updatePerspectiveProjection();
	Q_EMIT this->graphicsReady();
}

/// Event handler invoked when the widget GL surface is resized.
///
/// Note: this can happen when widget is resized by the QT layout engine.
/// @param width New width of the GL surface in pixels.
/// @param height New height of the GL surface in pixels.
void ViewportWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
	this->aspect = static_cast<float>(width) / static_cast<float>(height);
	this->updatePerspectiveProjection();
}

/// Event handler invoked when redraw is called for this widget.
void ViewportWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//logDebug("Drawing render queue of size -> {}", this->render_queue.size());
	for (RenderMesh *mesh : this->render_queue)
	{
		QOpenGLShaderProgram *shader = this->graphics->getModelShader();
		this->setShaderStandardInputs(*shader);
		mesh->Render(*shader);
	}
}

/// Set the color of the OpenGL surface background
/// @param color New color to use.
void ViewportWidget::setBackgroundColor(const QColor &color)
{
    this->background_color = color;
}

/// Set the color of the OpenGL surface background.
/// @param red Normalised red channel value.
/// @param green Normalised green channel value.
/// @param blue Normalised blue channel value.
void ViewportWidget::setBackgroundColor(float red, float green, float blue)
{
    this->background_color = QColor::fromRgbF(red, green, blue, 1.0f);
}

/// Recalulates perspective projection transform based on the current size of viewport.
void ViewportWidget::updatePerspectiveProjection()
{
	const float near = 0.001f;
	const float far = 1000000.0f;

	this->mat_perspective.setToIdentity();
	this->mat_perspective.perspective(this->fov, this->aspect, near, far);
}

/// Send standard input parameter the shader pipeline expects to receive to draw
/// content to screen correctly.
void ViewportWidget::setShaderStandardInputs(QOpenGLShaderProgram &shader)
{
	shader.bind();
	shader.setUniformValue("SV_MODEL_MAT", this->mat_model);
	shader.setUniformValue("SV_VIEW_MAT", this->mat_view);
	shader.setUniformValue("SV_PROJ_MAT", this->mat_perspective);
	shader.release();
}

void ViewportWidget::setCamMode(ViewportWidget::CameraMode mode)
{
	this->cam_mode = mode;
}

ViewportWidget::CameraMode ViewportWidget::getCamMode() const
{
	return this->cam_mode;
}

/// Event handler invoked when user presses mouse key over viewport widget.
/// Activates camera mode states.
void ViewportWidget::mousePressEvent(QMouseEvent *event)
{
	switch (event->button())
	{
		case Qt::LeftButton:
			this->mouse_pos = event->pos();
			this->setCamMode(CameraMode::Rotate);
			break;
		case Qt::RightButton:
			this->mouse_pos = event->pos();
			this->setCamMode(CameraMode::Pan);
			break;
		default:
			this->setCamMode(CameraMode::Default);
			this->mouse_pos = QPoint(0.0, 0.0);
			break;
	}
}

/// Event handler invoked when user releases mouse key over viewport widget.
/// Resets the camera mode state.
void ViewportWidget::mouseReleaseEvent(QMouseEvent *event)
{
	this->mouse_pos = QPoint(0.0, 0.0);
	this->setCamMode(CameraMode::Default);
}

void ViewportWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (this->getCamMode() == CameraMode::Rotate)
	{
		QPoint delta = event->pos() - this->mouse_pos;
		this->cam_yaw -= this->cam_sensitivity * delta.x();
		this->cam_pitch += this->cam_sensitivity * delta.y();
		this->cam_pitch = std::clamp(this->cam_pitch, -1.5, 1.5);
		this->mouse_pos = event->pos();
		
		this->setCamOrbit(this->cam_pitch, this->cam_yaw);
		this->update();
	}
}

/// Rotate the camera along current distance and focus point.
/// @param: pitch Pitch angle in radians.
/// @param: yaw Yaw angle in radians.
void ViewportWidget::setCamOrbit(double pitch, double yaw)
{
	double dist = this->cam_focus.distanceToPoint(this->mat_view.inverted().column(3).toVector3D());

	QVector3D cam_pos;
	cam_pos.setX(dist * qCos(this->cam_pitch) * qSin(this->cam_yaw));
	cam_pos.setY(dist * qSin(this->cam_pitch));
	cam_pos.setZ(dist * qCos(this->cam_pitch) * qCos(this->cam_yaw));

	this->mat_view.setToIdentity();
	this->mat_view.lookAt(this->cam_focus + cam_pos, this->cam_focus, QVector3D(0.0, 1.0, 0.0));
}
