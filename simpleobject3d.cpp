#include "simpleobject3d.h"

simpleobject3d::simpleobject3d():
    m_indexBuffer(QOpenGLBuffer::IndexBuffer), m_texture(0)
{

}
simpleobject3d::simpleobject3d(QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture):
    m_indexBuffer(QOpenGLBuffer::IndexBuffer), m_texture(0)
{
    init(vertData, indexes, texture);
}

simpleobject3d::~simpleobject3d()
{
    if (m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if (m_indexBuffer.isCreated())
        m_indexBuffer.destroy();

    if (m_texture != 0) {
        if (m_texture->isCreated())
            m_texture->destroy();
    }

}

void simpleobject3d::init(QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture)
{
    if (m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if (m_indexBuffer.isCreated())
        m_indexBuffer.destroy();

    if (m_texture != 0) {
        if (m_texture->isCreated())
        {
            delete m_texture;
            m_texture = 0;
        }
    }

    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertData.constData(), vertData.size() * sizeof(VertexData));
    m_vertexBuffer.release();

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    m_indexBuffer.release();

    m_texture = new QOpenGLTexture(texture.mirrored());
    // Set nearest filtering mode for texture minification
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    m_texture->setWrapMode(QOpenGLTexture::Repeat);

    m_modelMatrix.setToIdentity();

}

void simpleobject3d::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    if (!m_vertexBuffer.isCreated() || !m_indexBuffer.isCreated())
        return;

    m_texture->bind(0);
    program->setUniformValue("u_texture", 0);
    program->setUniformValue("u_modelMatrix", m_modelMatrix);

    int offset = 0;

    int vertLoc = program->attributeLocation("a_position");
    program->enableAttributeArray(vertLoc);
    program->setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int texLoc = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texLoc);
    program->setAttributeBuffer(texLoc, GL_FLOAT, offset, 2, sizeof(VertexData));

    offset += sizeof(QVector2D);

    int normLoc = program->attributeLocation("a_normal");
    program->enableAttributeArray(normLoc);
    program->setAttributeBuffer(normLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    m_indexBuffer.bind();

    functions->glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);

    m_vertexBuffer.release();
    m_indexBuffer.release();
    m_texture->release();
}
