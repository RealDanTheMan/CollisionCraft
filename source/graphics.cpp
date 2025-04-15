#include "graphics.h"
#include "logging.h"
#include <QFile>
#include <QTextStream>
#include <QOpenGLShader>
#include <format>
#include <memory>

/// Default ctor
Graphics::Graphics()
{
    this->model_shader = std::make_unique<QOpenGLShaderProgram>();
    this->collision_shader = std::make_unique<QOpenGLShaderProgram>();
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

/// Get handle to the default model shader program.
QOpenGLShaderProgram* Graphics::getModelShader() const
{
    return this->model_shader.get();
}

/// Get handle to the default collision model shader program.
QOpenGLShaderProgram* Graphics::getCollisionShader() const
{
    return this->collision_shader.get();
}

/// Initialise default set of shaders used during graphics operations.
/// Shader source code lives in resources/shaders/
bool Graphics::initDefaultShaders()
{
    /// Load and compile default model shader.
    if (!Graphics::compileShaderResource("model", *this->model_shader))
    {
        logError("Failed to initialise default model shader");
        return false;
    }

    /// Load and compile default collision shader.
    if (!Graphics::compileShaderResource("collision", *this->collision_shader))
    {
        logError("Failed to initialise default collision shader");
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

/// Loads and compiles vertex and pixel shader matching given name into shader program.
/// @param: shader_name Base name matching vertex and pixel shader resource.
/// @param: shader Out shader handle to load compile shader program.
bool Graphics::compileShaderResource(const std::string &shader_name, QOpenGLShaderProgram &shader)
{
    /// Note we use std::vformat as std::format is not fully implemented on MacOS
    std::string vs_res = std::vformat(
        ":shaders/{}.vs",
        std::make_format_args(shader_name)
    );

    std::string ps_res = std::vformat(
        ":shaders/{}.ps",
        std::make_format_args(shader_name)
    );

    QString vs_src;
    logDebug("Loading shader resouce -> {}", vs_res);
    if (!Graphics::loadShaderResource(QString(vs_res.c_str()), vs_src))
    {
        logError("Failed to load vertex shader resource -> {}", vs_res);
        return false;
    }

    QString ps_src;
    logDebug("Loading shader resouce -> {}", ps_res);
    if (!Graphics::loadShaderResource(QString(ps_res.c_str()), ps_src))
    {
        logError("Failed to load pixel shader resource -> {}", ps_res);
        return false;
    }

    logDebug("Compiling shader resouce -> {}", shader_name);
    if(!Graphics::compileShader(vs_src, ps_src, shader))
    {
        logError("Failed to compile shader program -> {}", shader_name);
        return false;
    }

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
