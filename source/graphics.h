#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <memory>
#include <QString>
#include <QOpenGLShaderProgram>
#include <qopenglshaderprogram.h>


class Graphics
{
public:
    Graphics();
    bool init();
    QOpenGLShaderProgram * getModelShader() const;
    QOpenGLShaderProgram * getCollisionShader() const;

protected:
    bool initDefaultShaders();
    bool initModelShader();
    bool initCollisionShader();

    bool compileShaderResource(const std::string &shader_name, QOpenGLShaderProgram &shader);
    static bool loadShaderResource(const QString &resoure_path, QString &shader_src);

    static bool compileShader(
        const QString &vertex_src,
        const QString &pixel_src,
        QOpenGLShaderProgram &shader
    );

private:
    std::unique_ptr<QOpenGLShaderProgram> model_shader;
    std::unique_ptr<QOpenGLShaderProgram> collision_shader;

};

#endif
