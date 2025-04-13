#include "viewportcamera.h"

ViewportCamera::ViewportCamera(double fov, double aspect) :
	fov(fov),
	aspect(aspect),
	near_clip(0.001),
	far_clip(1000000.0)
{
	this->mat_view.setToIdentity();
	this->mat_transform.setToIdentity();
	this->mat_projection.setToIdentity();
}

void ViewportCamera::setFov(double fov)
{
	this->fov = fov;
	this->updateProjection();
}

double ViewportCamera::getFov() const
{
	return this->fov;
}

double ViewportCamera::getAspect() const
{
	return this->aspect;
}

void ViewportCamera::setAspect(double aspect)
{
	this->aspect = aspect;
	this->updateProjection();
}

void ViewportCamera::setPosition(const QVector3D &pos)
{
	QVector4D translation(
		pos.x(),
		pos.y(),
		pos.z(),
		1.0
	);

	this->mat_transform.setColumn(3, translation);
	this->updateView();
}

void ViewportCamera::setTransform(const QMatrix4x4 &transform)
{
	this->mat_transform = transform;
	this->updateView();
}

QVector3D ViewportCamera::getPosition() const
{
	return this->mat_transform.column(3).toVector3D();
}

void ViewportCamera::updateProjection()
{
	this->mat_projection.setToIdentity();
	this->mat_projection.perspective(this->fov, this->aspect, this->near_clip, this->far_clip);
}

void ViewportCamera::updateView()
{
	this->mat_view = this->getTransformMatrix().inverted();
}

const QMatrix4x4& ViewportCamera::getViewMatrix() const
{
	return this->mat_view;
}

const QMatrix4x4& ViewportCamera::getTransformMatrix() const
{
	return this->mat_transform;
}

const QMatrix4x4& ViewportCamera::getPorjectionMatrix() const
{
	return this->mat_projection;
}

QVector3D ViewportCamera::forward() const
{
	return this->getTransformMatrix().column(2).toVector3D().normalized();
}

QVector3D ViewportCamera::up() const
{
	return this->getTransformMatrix().column(1).toVector3D().normalized();
}

QVector3D ViewportCamera::right() const
{
	return this->getTransformMatrix().column(0).toVector3D().normalized();
}
