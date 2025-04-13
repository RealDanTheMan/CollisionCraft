#include "viewportwidget.h"
#include "logging.h"
#include "rendermesh.h"
#include "viewportcamera.h"

#include <algorithm>
#include <cmath>
#include <csignal>
#include <memory>
#include <vector>

#include <Qt>
#include <QVector3D>
#include <QMatrix3x3>
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
    view_mode(ViewportWidget::CameraMode::Default),
    orbit_sensitivity(0.0174533),
    orbit_center(QVector3D(0.0, 0.0, 0.0)),
    orbit_acc_yaw(0.0),
    orbit_acc_pitch(0.0)
{
    this->graphics = std::make_unique<Graphics>();
    this->camera = std::make_unique<ViewportCamera>();
    this->mat_model.setToIdentity();
}

ViewportCamera* ViewportWidget::getCamera()
{
    return this->camera.get();
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

    double v_fov = qDegreesToRadians(this->camera->getFov());
    double h_fov = 2.0 * std::atan(std::tan(v_fov * 0.5) * this->camera->getAspect());

    QVector3D scene_center(0.0, 0.0, 0.0);
    double scene_radius;
    computeSceneBoundingSphere(scene_center, scene_radius);

    double v_dist = scene_radius / std::tan(v_fov * 0.5);
    double h_dist = scene_radius / std::tan(h_fov * 0.5);
    double dist = std::max(v_dist, h_dist);
    QVector3D cam_pos = scene_center + this->camera->forward() * dist;

    this->camera->setPosition(cam_pos);
    this->orbit_center = scene_center;
}

/// Compute bounding sphere that envelops currently loaded render meshes.
void ViewportWidget::computeSceneBoundingSphere(QVector3D &center, double &radius) const
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
    double aspect = static_cast<float>(width) / static_cast<float>(height);
    this->camera->setAspect(aspect);
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

/// Send standard input parameter the shader pipeline expects to receive to draw
/// content to screen correctly.
void ViewportWidget::setShaderStandardInputs(QOpenGLShaderProgram &shader)
{
    shader.bind();
    shader.setUniformValue("SV_MODEL_MAT", this->mat_model);
    shader.setUniformValue("SV_VIEW_MAT", this->camera->getViewMatrix());
    shader.setUniformValue("SV_PROJ_MAT", this->camera->getPorjectionMatrix());
    shader.release();
}

void ViewportWidget::setCamMode(ViewportWidget::ViewMode mode)
{
    this->view_mode = mode;
}

ViewportWidget::ViewMode ViewportWidget::getCamMode() const
{
    return this->view_mode;
}

/// Event handler invoked when user presses mouse key over viewport widget.
/// Activates camera mode states.
void ViewportWidget::mousePressEvent(QMouseEvent *event)
{
    switch (event->button())
    {
        case Qt::LeftButton:
            this->mouse_pos = event->pos();
            this->setCamMode(ViewMode::Orbit);
            this->orbit_distance = this->orbit_center.distanceToPoint(this->camera->getPosition());
            break;
        case Qt::RightButton:
            this->mouse_pos = event->pos();
            this->pan_acc_x = this->camera->getPosition().x();
            this->pan_acc_y = this->camera->getPosition().y();
            this->setCamMode(ViewMode::Pan);
            break;
        default:
            this->setCamMode(ViewMode::Default);
            this->mouse_pos = QPoint(0.0, 0.0);
            break;
    }
}

/// Event handler invoked when user releases mouse key over viewport widget.
/// Resets the camera mode state.
void ViewportWidget::mouseReleaseEvent(QMouseEvent *event)
{
    this->mouse_pos = QPoint(0.0, 0.0);
    this->setCamMode(ViewMode::Default);
}

void ViewportWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (this->getCamMode() == ViewMode::Orbit)
    {
        QPoint delta = event->pos() - this->mouse_pos;
        this->acc_yaw -= this->orbit_sensitivity * delta.x();
        this->orbit_acc_pitch += this->orbit_sensitivity * delta.y();
        this->orbit_acc_pitch = std::clamp(this->orbit_acc_pitch, -1.5, 1.5);
        this->mouse_pos = event->pos();
        
        this->setCamOrbit(this->orbit_acc_pitch, this->acc_yaw);
        this->update();
        return;
    }
    else if (this->getCamMode() == ViewMode::Pan)
    {
        const double sensitivity = 1.0;
        QPoint delta = event->pos() - this->mouse_pos;
        this->pan_acc_x += sensitivity * delta.x();
        this->pan_acc_y += sensitivity * delta.y();
        this->mouse_pos = event->pos();
    
        QVector3D cam_pos = this->camera->getPosition();
        QVector3D offset(this->pan_acc_x, this->pan_acc_y, cam_pos.z());

        this->camera->setPosition(offset);
        this->update();
    }
}

/// Rotate the camera along current distance and focus point.
/// @param: pitch Pitch angle in radians.
/// @param: yaw Yaw angle in radians.
void ViewportWidget::setCamOrbit(double pitch, double yaw)
{

    QVector3D cam_pos;
    cam_pos.setX(this->orbit_distance * qCos(pitch) * qSin(yaw));
    cam_pos.setY(this->orbit_distance * qSin(pitch));
    cam_pos.setZ(this->orbit_distance * qCos(pitch) * qCos(yaw));

    QVector3D cam_forward = (cam_pos - this->orbit_center).normalized();
    QVector3D cam_right = QVector3D::crossProduct(cam_forward, QVector3D(0, 1, 0)).normalized();
    QVector3D cam_up = QVector3D::crossProduct(cam_right, cam_forward);

    QMatrix4x4 cam_transform;
    cam_transform.setColumn(0, cam_right.toVector4D());
    cam_transform.setColumn(1, cam_up.toVector4D());
    cam_transform.setColumn(2, cam_forward.toVector4D());
    cam_transform.setColumn(3, QVector4D(cam_pos.x(), cam_pos.y(), cam_pos.z(), 1.0));

    this->camera->setTransform(cam_transform);
}
