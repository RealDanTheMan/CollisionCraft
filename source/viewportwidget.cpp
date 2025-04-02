#include "viewportwidget.h"
#include "logging.h"
#include "rendermesh.h"
#include <memory>
#include <qtmetamacros.h>
#include <vector>
#include <QVector3D>
#include <QSurfaceFormat>

ViewportWidget::ViewportWidget(QWidget *parent) : QOpenGLWidget(parent) 
{
	this->graphics = std::make_unique<Graphics>();
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
}

/// Event handler invoked when redraw is called for this widget.
void ViewportWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	logDebug("Drawing render queue of size -> {}", this->render_queue.size());
	for (RenderMesh *mesh : this->render_queue)
	{
		QOpenGLShaderProgram *shader = this->graphics->getModelShader();
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
