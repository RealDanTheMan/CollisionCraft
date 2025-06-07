#include "viewportwidget.h"
#include "gridrendermesh.h"
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
    view_mode(ViewportWidget::ViewMode::Default),
    orbit_sensitivity(0.0174533),
    orbit_center(QVector3D(0.0, 0.0, 0.0)),
    orbit_acc_yaw(0.0),
    orbit_acc_pitch(0.0)
{
    this->graphics = std::make_unique<Graphics>();
    this->camera = std::make_unique<ViewportCamera>();
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

/// Remove given render mesh from this viewport render queue.
void ViewportWidget::removeRenderMesh(RenderMesh *mesh)
{
    std::vector<RenderMesh*>::iterator it = std::find(
        this->render_queue.begin(),
        this->render_queue.end(),
        mesh
    );

    if (it != this->render_queue.end())
    {
        this->render_queue.erase(it);
    }
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

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(
        this->background_color.redF(),
        this->background_color.greenF(),
        this->background_color.blueF(),
        1.0f
    );

    this->grid_mesh = std::make_unique<GridRenderMesh>(1024, 1024, 10.0);
    this->grid_mesh->bindShader(this->graphics->getGridShader());
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    this->drawGridMesh(*this->grid_mesh);

    //logDebug("Drawing render queue of size -> {}", this->render_queue.size());
    for (RenderMesh *mesh : this->render_queue)
    {
        if (!mesh->getVisibility())
        {
            continue;
        }

        if (mesh->getStyle() == RenderMeshStyle::Shaded ||
            mesh->getStyle() == RenderMeshStyle::ShadedWireframe)
        {
            this->drawMesh(*mesh);
        }

        if (mesh->getStyle() == RenderMeshStyle::WireframeOnly ||
            mesh->getStyle() == RenderMeshStyle::ShadedWireframe)
        {
            this->drawMeshWireframe(*mesh);
        }
    }
}

/// Draw given render mesh to viewport screen.
void ViewportWidget::drawMesh(RenderMesh &mesh)
{
    QOpenGLShaderProgram *shader = nullptr;
    switch (mesh.getMaterial())
    {
        case RenderMeshMaterial::Standard:
            mesh.bindShader(this->graphics->getModelShader());
            mesh.shader()->bind();
            mesh.shader()->setUniformValue("unlit", false);
            mesh.shader()->release();
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(2.0, 2.0);
            break;
        case RenderMeshMaterial::StandardUnlit:
            mesh.bindShader(this->graphics->getModelShader());
            mesh.shader()->bind();
            mesh.shader()->setUniformValue("unlit", true);
            mesh.shader()->release();
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(2.0, 2.0);
            break;
        case RenderMeshMaterial::Collision:
            mesh.bindShader(this->graphics->getCollisionShader());
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(-2.0, -2.0);
            break;
        default:
            logError(
                "Invalid render mesh material [Material Flag: {}]",
                static_cast<int>(mesh.getMaterial())
            );
            return;
    }

    this->setShaderStandardInputs(mesh);
    mesh.Render();
    glPolygonOffset(0.0, 0.0);
    glDisable(GL_POLYGON_OFFSET_FILL);

}

/// Draw given render mesh wireframe to viewport screen.
void ViewportWidget::drawMeshWireframe(RenderMesh &mesh)
{
    QOpenGLShaderProgram *shader = this->graphics->getWireframeShader();
    mesh.bindShader(shader);
    this->setShaderStandardInputs(mesh);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonOffset(-4.0, -4.0);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glLineWidth(2.0);
    mesh.Render();
    glLineWidth(1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPolygonOffset(0.0, 0.0);
    glDisable(GL_POLYGON_OFFSET_LINE);
}

/// Draw given grid render mesh to viewport screen.
void ViewportWidget::drawGridMesh(GridRenderMesh &grid)
{
    grid.shader()->bind();

    if (grid.shader()->uniformLocation("SV_VIEW_MAT") != -1)
    {
        grid.shader()->setUniformValue("SV_VIEW_MAT", this->camera->getViewMatrix());
    }
    if (grid.shader()->uniformLocation("SV_PROJ_MAT") != -1)
    {
        grid.shader()->setUniformValue("SV_PROJ_MAT", this->camera->getPorjectionMatrix());
    }

    grid.shader()->release();
    grid.render();
}

/// Set the color of the OpenGL surface background
/// @param color New color to use.
void ViewportWidget::setBackgroundColor(const QColor &color)
{
    this->background_color = color;
    glClearColor(
        this->background_color.redF(),
        this->background_color.greenF(),
        this->background_color.blueF(),
        1.0f
    );
}

/// Set the color of the OpenGL surface background.
/// @param red Normalised red channel value.
/// @param green Normalised green channel value.
/// @param blue Normalised blue channel value.
void ViewportWidget::setBackgroundColor(float red, float green, float blue)
{
    this->background_color = QColor::fromRgbF(red, green, blue, 1.0f);
    glClearColor(
        this->background_color.redF(),
        this->background_color.greenF(),
        this->background_color.blueF(),
        1.0f
    );
}

/// Send standard input parameter the shader pipeline expects to receive to draw
/// content to screen correctly.
void ViewportWidget::setShaderStandardInputs(const RenderMesh &mesh)
{
    mesh.shader()->bind();

    if (mesh.shader()->uniformLocation("SV_MODEL_MAT") != -1)
    {
        mesh.shader()->setUniformValue("SV_MODEL_MAT", mesh.getTransform());
    }
    if (mesh.shader()->uniformLocation("SV_NORMAL_MAT") != -1)
    {
        mesh.shader()->setUniformValue("SV_NORMAL_MAT", mesh.getTransform().normalMatrix());
    }
    if (mesh.shader()->uniformLocation("SV_VIEW_MAT") != -1)
    {
        mesh.shader()->setUniformValue("SV_VIEW_MAT", this->camera->getViewMatrix());
    }
    if (mesh.shader()->uniformLocation("SV_PROJ_MAT") != -1)
    {
        mesh.shader()->setUniformValue("SV_PROJ_MAT", this->camera->getPorjectionMatrix());
    }

    mesh.shader()->release();
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
            logDebug("Enabling camera orbit mode");
            this->mouse_pos = event->pos();
            this->setCamMode(ViewMode::Orbit);
            this->orbit_distance = this->orbit_center.distanceToPoint(this->camera->getPosition());
            break;
        case Qt::RightButton:
            if (event->modifiers() == Qt::NoModifier)
            {
                logDebug("Enabling camera panning mode");
                this->mouse_pos = event->pos();
                this->setCamMode(ViewMode::Pan);
                break;
            }
            else if (event->modifiers() & Qt::AltModifier)
            {
                logDebug("Enabling camera zoom mode");
                this->mouse_pos = event->pos();
                this->setCamMode(ViewMode::Zoom);
                break;
            }
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
    logDebug("Reverting back to default camera mode");
    this->mouse_pos = QPoint(0.0, 0.0);
    this->setCamMode(ViewMode::Default);
}

void ViewportWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (this->getCamMode() == ViewMode::Orbit)
    {
        QPoint delta = event->pos() - this->mouse_pos;
        this->orbit_acc_yaw -= this->orbit_sensitivity * delta.x();
        this->orbit_acc_pitch += this->orbit_sensitivity * delta.y();
        this->orbit_acc_pitch = std::clamp(this->orbit_acc_pitch, -1.5, 1.5);
        this->mouse_pos = event->pos();

        this->setCamOrbit(this->orbit_acc_pitch, this->orbit_acc_yaw);
        this->update();
        return;
    }
    else if (this->getCamMode() == ViewMode::Pan)
    {
        const double sensitivity = 1.0;
        QPoint delta = (event->pos() - this->mouse_pos) * sensitivity;
        this->mouse_pos = event->pos();
        this->setCamPan(delta.x(), delta.y());
        this->update();
    }
    else if (this->getCamMode() == ViewMode::Zoom)
    {
        const double sensitivity = 3.0;
        QPoint delta = (event->pos() - this->mouse_pos) * sensitivity;
        this->mouse_pos = event->pos();
        this->setCamZoom(delta.x() * sensitivity);
        this->update();
    }
}

void ViewportWidget::wheelEvent(QWheelEvent *event)
{
    const double delta = event->angleDelta().y();
    const double sensitivity = 1.5;

    this->setCamZoom(-delta * sensitivity);
    this->update();
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

    QVector3D up = QVector3D(0.0, 1.0, 0.0);
    QVector3D cam_forward = (cam_pos - this->orbit_center).normalized();
    QVector3D cam_right = QVector3D::crossProduct(up, cam_forward).normalized();
    QVector3D cam_up = QVector3D::crossProduct(cam_forward, cam_right);

    QMatrix4x4 cam_transform;
    cam_transform.setColumn(0, cam_right.toVector4D());
    cam_transform.setColumn(1, cam_up.toVector4D());
    cam_transform.setColumn(2, cam_forward.toVector4D());
    cam_transform.setColumn(3, QVector4D(cam_pos.x(), cam_pos.y(), cam_pos.z(), 1.0));

    this->camera->setTransform(cam_transform);
}

/// Pan the camera relatively to it current view vector.
/// @param: x Screen space lateral pan delta value.
/// @param: y Screen space vertical pan delta value.
void ViewportWidget::setCamPan(double x, double y)
{

    QVector3D cam_pos = this->camera->getPosition();
    QVector3D lateral = this->camera->right() * -x;
    QVector3D vertical = this->camera->up() * y;
    QVector3D offset = vertical + lateral + cam_pos;

    this->camera->setPosition(offset);
    this->update();
}

/// Zoon in or out camera along it current forward view.
/// @param: value Zoom delta factor.
void ViewportWidget::setCamZoom(double value)
{
    QVector3D cam_pos = this->camera->getPosition();
    QVector3D offset = this->camera->forward() * value;
    this->camera->setPosition(cam_pos + offset);
}
