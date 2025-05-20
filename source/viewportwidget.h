#ifndef VIEWPORT_WIDGET_H
#define VIEWPORT_WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QMouseEvent>

#include <memory>
#include <vector>

#include "graphics.h"
#include "rendermesh.h"
#include "gridrendermesh.h"
#include "viewportcamera.h"

struct ViewportSettings
{
    bool collision_shaded;
    bool collision_wireframe;
    bool collision_hidden;
    bool model_shaded;
    bool model_wireframe;
    bool model_lighting;
    bool model_hidden;

    static ViewportSettings Default()
    {
        ViewportSettings settings;
        settings.collision_shaded = false;
        settings.collision_wireframe = true;
        settings.collision_hidden = false;
        settings.model_shaded = true;
        settings.model_wireframe = false;
        settings.model_lighting = false;
        settings.model_hidden = false;

        return settings;
    }
};

class ViewportWidget : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    ViewportWidget(QWidget *parent = nullptr);

    void setBackgroundColor(const QColor &color);
    void setBackgroundColor(float red, float green, float blue);

    void clearRenderMeshes();
    void addRenderMesh(RenderMesh *mesh);
    void removeRenderMesh(RenderMesh *mesh);
    void autoFrameCamera();
    void computeSceneBoundingSphere(QVector3D &center, double &radius) const;

    ViewportCamera* getCamera();

    Q_SIGNAL
    void graphicsReady();

protected:
    enum class ViewMode
    {
        Default,
        Zoom,
        Pan,
        Orbit
    };

    virtual void initializeGL() override;
    virtual void resizeGL(int width, int height) override;
    virtual void paintGL() override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    
    void setShaderStandardInputs(const RenderMesh &mesh);
    void setCamMode(ViewMode mode);
    void setCamOrbit(double pitch, double yaw);
    void setCamPan(double x, double y);
    void setCamZoom(double value);
    ViewMode getCamMode() const;

    void drawMesh(RenderMesh &mesh);
    void drawMeshWireframe(RenderMesh &mesh);
    void drawGridMesh(GridRenderMesh &grid);

protected:
    QColor background_color = QColor(0.0f, 0.0f, 0.0f, 1.0f);
    std::unique_ptr<Graphics> graphics = nullptr;
    std::unique_ptr<ViewportCamera> camera = nullptr;

private:
    std::unique_ptr<GridRenderMesh> grid_mesh;
    std::vector<RenderMesh*> render_queue;
    QPoint mouse_pos;
    ViewMode view_mode;

    QVector3D orbit_center;
    double orbit_distance;
    double orbit_acc_yaw;
    double orbit_acc_pitch;
    double orbit_sensitivity;
};

#endif
