#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>;

class ViewportCamera
{
public:
    ViewportCamera(double fov = 45.0, double aspect = 1.0);

    const QMatrix4x4& getViewMatrix() const;
    const QMatrix4x4& getPorjectionMatrix() const;
    const QMatrix4x4& getTransformMatrix() const;

    void setFov(double fov);
    void setAspect(double aspect);
    double getFov() const;
    double getAspect() const;

    void setPosition(const QVector3D &pos);
    QVector3D getPosition() const;
    void setTransform(const QMatrix4x4 &transform);

    QVector3D forward() const;
    QVector3D up() const;
    QVector3D right() const;

protected:
    void updateProjection();
    void updateView();

private:
    double fov;
    double aspect;
    double near_clip;
    double far_clip;

    QMatrix4x4 mat_view;
    QMatrix4x4 mat_transform;
    QMatrix4x4 mat_projection;
};

#endif
