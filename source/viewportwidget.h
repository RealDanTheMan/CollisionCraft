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

class ViewportWidget : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    ViewportWidget(QWidget *parent = nullptr);
    void setBackgroundColor(const QColor &color);
    void setBackgroundColor(float red, float green, float blue);

	void clearRenderMeshes();
	void addRenderMesh(RenderMesh *mesh);
	void autoFrameCamera();
	void computeSceneBoundingSphere(QVector3D &center, float &radius) const;

	Q_SIGNAL
	void graphicsReady();

protected:
	enum class CameraMode
	{
		Default,
		Zoom,
		Pan,
		Rotate
	};

    virtual void initializeGL() override;
    virtual void resizeGL(int width, int height) override;
    virtual void paintGL() override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	
	void updatePerspectiveProjection();
	void setShaderStandardInputs(QOpenGLShaderProgram &shader);
	void setCamMode(CameraMode mode);
	void setCamOrbit(double pitch, double yaw);
	CameraMode getCamMode() const;

protected:
    QColor background_color = QColor(0.0f, 0.0f, 0.0f, 1.0f);
	std::unique_ptr<Graphics> graphics = nullptr;

private:
	std::vector<RenderMesh*> render_queue;

	QMatrix4x4 mat_model;
	QMatrix4x4 mat_view;
	QMatrix4x4 mat_perspective;
	QPoint mouse_pos;
	
	CameraMode cam_mode;
	QVector3D cam_focus;
	double cam_sensitivity;
	double cam_yaw;
	double cam_pitch;
	float aspect;
	float fov;
};

#endif
