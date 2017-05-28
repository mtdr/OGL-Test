#ifndef SIMPLEOBJECT3D_H
#define SIMPLEOBJECT3D_H

#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLContext>
#include <QOpenGLTexture>

struct VertexData
{
    VertexData()
    {
    }
    VertexData(QVector3D p, QVector2D t, QVector3D n) :
        position(p), texCoord(t), normal(n)
    {
    }

    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
};

class simpleobject3d
{
public:
    simpleobject3d();
    simpleobject3d(QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture);
    ~simpleobject3d();
    void init(QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture);
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);

private:
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    QMatrix4x4 m_modelMatrix;
    QOpenGLTexture *m_texture;
};

#endif // SIMPLEOBJECT3D_H
