#ifndef VIEWPORT_WIDGET_H
#define VIEWPORT_WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
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

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int width, int height) override;
    virtual void paintGL() override;

    QColor background_color = QColor(0.0f, 0.0f, 0.0f, 1.0f);
	std::unique_ptr<Graphics> graphics = nullptr;

private:
	std::unique_ptr<RenderMesh> fallback_mesh;
	std::vector<RenderMesh*> render_queue;

};

#endif
