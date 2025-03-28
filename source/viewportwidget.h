#ifndef VIEWPORT_WIDGET_H
#define VIEWPORT_WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class ViewportWidget : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    ViewportWidget(QWidget *parent = nullptr);
    void setBackgroundColor(const QColor &color);
    void setBackgroundColor(float red, float green, float blue);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int width, int height) override;
    virtual void paintGL() override;

    QColor background_color = QColor(0.0f, 0.0f, 0.0f, 1.0f);

};

#endif
