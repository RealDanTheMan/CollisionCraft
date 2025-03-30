#include "graphics.h"
#include "logging.h"
#include <QFile>
#include <QTextStream>
#include <QOpenGLShader>

/// Default ctor
Graphics::Graphics()
{
	this->model_shader = std::make_unique<QOpenGLShaderProgram>();
}

/// Initialise graphics.
/// Graphics have to be initialised before performing any graphics operations.
bool Graphics::init()
{
	Logger::active()->debug("Initialising default shaders");
	if (!initDefaultShaders())
	{
		Logger::active()->error("Failed to initialise default shaders!");
		return false;
	}

	return true;
}

/// Initialise default set of shaders used during graphics operations.
/// Shader source code lives in resources/shaders/
bool Graphics::initDefaultShaders()
{
	QString model_vs = "";
	Logger::active()->debug("Loading default model vertex shader");
	if (!Graphics::loadShaderResource(":shaders/model.vs", model_vs))
	{
		Logger::active()->error("Failed to load default model vertex shader!");
		return false;
	}

	QString model_ps = "";
	Logger::active()->debug("Loading default model pixel shader");
	if (!Graphics::loadShaderResource(":shaders/model.ps", model_ps))
	{
		Logger::active()->error("Failed to load default model pixel shader!");
		return false;
	}

	Logger::active()->debug("Compiling default model shader");
	if (!Graphics::compileShader(model_vs, model_ps, *this->model_shader))
	{
		Logger::active()->error("Failed to compile default model shader!");
		return false;
	}

	return true;
}

/// Load shader source from given resource file path.
/// @param: resource_path Filepath that points to shader source code.
/// @param: shader_src String to capture loaded shader code.
/// @returns: True if loading was successfull, false otherwise.
bool Graphics::loadShaderResource(const QString &resource_path, QString &shader_src)
{
	QFile file(resource_path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		Logger::active()->error("Failed to open shader resource!");
		return false;
	}

	QTextStream src(&file);
	shader_src = src.readAll();
	file.close();

	return true;
}

/// Compile gven vertex and pixel shader source code into shader bytecode program.
/// @param: vertex_src String containing vertex shader source code.
/// @param: pixel_src String containing pixel shader source code.
/// @param: shader Shader program handle to load shader bytecode program.
/// @returns: true if shader compilation and linking was successfull, false otherwise.
bool Graphics::compileShader(
	const QString &vertex_src,
	const QString &pixel_src,
	QOpenGLShaderProgram &shader
)
{
	QOpenGLShader vs(QOpenGLShader::Vertex);
	QOpenGLShader ps(QOpenGLShader::Fragment);

	if (!vs.compileSourceCode(vertex_src))
	{
		Logger::active()->debug(vs.log().toStdString());
		Logger::active()->error("Failed to compile vertex shader source!");
		return false;
	}

	if (!ps.compileSourceCode(pixel_src))
	{
		Logger::active()->debug(ps.log().toStdString());
		Logger::active()->error("Failed to compile pixel shader source!");
		return false;
	}

	shader.addShader(&vs);
	shader.addShader(&ps);
	if(!shader.link())
	{
		Logger::active()->debug(shader.log().toStdString());
		Logger::active()->error("Failed to link shader program file!");
		return false;
	}

	return true;
}
