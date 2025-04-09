#include "viewportwidget.h"
#include "logging.h"
#include "rendermesh.h"
#include <cmath>
#include <csignal>
#include <memory>
#include <qtmetamacros.h>
#include <vector>
#include <QVector3D>
#include <QSurfaceFormat>

ViewportWidget::ViewportWidget(QWidget *parent) : 
	QOpenGLWidget(parent),
	aspect(0.0),
	fov(45.0)
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

	RenderMesh *mesh = this->render_queue[0];
	float v_dist = mesh->getBoundingShereRadius() / std::tan(v_fov * 0.5);
	float h_dist = mesh->getBoundingShereRadius() / std::tan(h_fov * 0.5);
	float dist = std::max(v_dist, h_dist);

	QVector3D forward = QVector3D(0.0, 0.0, -1.0);
	QVector3D up = QVector3D(0.0, 1.0, 0.0);
	QVector3D cam_pos = mesh->getBoundingShereCenter() - forward * dist;

	this->mat_view.setToIdentity();
	this->mat_view.lookAt(cam_pos, mesh->getBoundingShereCenter(), up);
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
	glDisable(GL_CULL_FACE);
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
