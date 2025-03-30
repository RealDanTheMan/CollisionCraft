#include "viewportwidget.h"
#include "logging.h"
#include <memory>
#include <vector>
#include <QVector3D>
#include <QSurfaceFormat>

ViewportWidget::ViewportWidget(QWidget *parent) : QOpenGLWidget(parent) 
{
	this->graphics = std::make_unique<Graphics>();
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

    glClearColor(
        this->background_color.redF(),
        this->background_color.greenF(),
        this->background_color.blueF(),
        1.0f
    );

	Logger::active()->debug("Initialising viewport fallback render mesh");
	std::vector<QVector3D> fallback_mesh_verts = {
		QVector3D(-0.5f, -0.5f, 0.0f),
		QVector3D(0.5f, -0.5f, 0.0f),
		QVector3D(0.0f, 0.5f, 0.0f),
	};

	std::vector<int> fallback_mesh_indices = {0, 1, 2};
	Mesh mesh(fallback_mesh_verts, fallback_mesh_indices);
	this->fallback_mesh = std::make_unique<RenderMesh>(mesh);
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
	if (this->fallback_mesh)
	{
		QOpenGLShaderProgram *shader = this->graphics->getModelShader();
		this->fallback_mesh->Render(*shader);
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
