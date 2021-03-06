#include "model.h"



bool Model::load(const QString &filePath)
{
    m_fileName=QFileInfo(filePath).fileName();
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"can't open "<<m_fileName;
        return false;
    }


    QVector3D boundsMin( 1e9, 1e9, 1e9);
    QVector3D boundsMax(-1e9,-1e9,-1e9);

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString input = in.readLine();
        if (input.isEmpty() || input[0] == '#')
            continue;

        QTextStream ts(&input);
        QString id;
        ts >> id;
        if (id == "v")
        {
            QVector3D p;
            for (int i = 0; i < 3; ++i)
            {
                ts >> p[i];
                boundsMin[i] = qMin(boundsMin[i], p[i]);
                boundsMax[i] = qMax(boundsMax[i], p[i]);
            }
            oriPoints << p;
        }
        else if (id == "f" || id == "fo")
        {
            QVarLengthArray<int, 4> p;

            while (!ts.atEnd())
            {
                QString vertex;
                ts >> vertex;
                const int vertexIndex = vertex.split('/').value(0).toInt();
                if (vertexIndex)
                    p.append(vertexIndex > 0 ? vertexIndex - 1 : oriPoints.size()+vertexIndex);
            }

            for (int i = 0; i < p.size(); ++i)
            {
                const int edgeA = p[i];
                const int edgeB = p[(i + 1) % p.size()];

                if (edgeA < edgeB)
                    edgeIndices << edgeA << edgeB;
            }

            for (int i = 0; i < 3; ++i)
                pointIndices << p[i];

            if (p.size() == 4)
                for (int i = 0; i < 3; ++i)
                    pointIndices << p[(i + 2) % 4];
        }
    }

//    const QVector3D bounds = boundsMax - boundsMin;
//    const qreal scale = 1 / qMax(bounds.x(), qMax(bounds.y(), bounds.z()));
//    for (int i = 0; i < oriPoints.size(); ++i)
//        oriPoints[i] = (oriPoints[i]-(boundsMin + bounds*0.5))*scale;

    oriNormals.resize(oriPoints.size());
    for (int i = 0; i < pointIndices.size(); i += 3)
    {
        const QVector3D a = oriPoints.at(pointIndices.at(i));
        const QVector3D b = oriPoints.at(pointIndices.at(i+1));
        const QVector3D c = oriPoints.at(pointIndices.at(i+2));
        const QVector3D normal = QVector3D::crossProduct(b-a,c-a).normalized();

        for (int j = 0; j < 3; ++j)
            oriNormals[pointIndices.at(i + j)] += normal;
    }

    for (int i = 0; i < oriNormals.size(); ++i)
        oriNormals[i] = oriNormals[i].normalized();


    //填充更新后顶点和向量数组
    for(int i=0; i<oriPoints.size(); i++)
    {
        points.push_back(oriPoints[i]);
    }
    for(int i=0; i<oriNormals.size(); i++)
    {
        normals.push_back(oriNormals[i]);
    }

    return true;
}

bool Model::init()
{
    arrayBuf.create();
    arrayBuf.bind();
    arrayBuf.allocate(getPointsData(), countPoints() * sizeof(QVector3D));

    indexBuf.create();
    indexBuf.bind();
    indexBuf.allocate(getIndicesData(), countIndices()* sizeof(GLuint));

    normalBuf.create();
    normalBuf.bind();
    normalBuf.allocate(getNormalsData(),countNormals()*sizeof(QVector3D));


    return true;
}

void Model::draw(QOpenGLShaderProgram& shaderProgram)
{

    if(texture)
    {
        shaderProgram.setUniformValue("texture",0);
        texture->bind(0);
    }




    normalBuf.bind();

    int normalLocation=shaderProgram.attributeLocation("vNormal");
    shaderProgram.enableAttributeArray(normalLocation);
    shaderProgram.setAttributeBuffer(normalLocation,GL_FLOAT,0,3,sizeof(QVector3D));

    arrayBuf.bind();
    indexBuf.bind();

    quintptr offset = 0;

    int vertexLocation = shaderProgram.attributeLocation("vVertex");
    shaderProgram.enableAttributeArray(vertexLocation);
    shaderProgram.setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(QVector3D));

    if(texture)
    {
        texBuf.bind();
        int texcoordLocation = shaderProgram.attributeLocation("vTexCoords");
        shaderProgram.enableAttributeArray(texcoordLocation);
        shaderProgram.setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(QVector2D));
    }


    //    offset += sizeof(QVector3D);
    //
    //    int texcoordLocation = program.attributeLocation("a_texcoord");
    //    program.enableAttributeArray(texcoordLocation);
    //    program.setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    //glDrawElements(GL_POINTS, model->indexs(), GL_UNSIGNED_INT, 0);
    glDrawElements(GL_TRIANGLES, countIndices(), GL_UNSIGNED_INT, 0);
    //    glDrawElements(GL_LINES, model->m_edgeIndices.size(), GL_UNSIGNED_INT, model->m_edgeIndices.data());

    //    glDrawElements(GL_LINES, model->m_edgeIndices.size(), GL_UNSIGNED_INT, model->m_edgeIndices.data());

}

void Model::update(Env &env, float dt)
{
    Q_UNUSED(env);
    Q_UNUSED(dt);

    arrayBuf.bind();
    arrayBuf.allocate(getPointsData(), countPoints() * sizeof(QVector3D));

    indexBuf.bind();
    indexBuf.allocate(getIndicesData(), countIndices()* sizeof(GLuint));

    normalBuf.bind();
    normalBuf.allocate(getNormalsData(),countNormals()*sizeof(QVector3D));

    if(texture)
    {
        texBuf.bind();
        texBuf.allocate(getTexCoordData(),countTexCoord()*sizeof(QVector2D));
    }
}

void Model::rotate(float angle, float x, float y, float z)
{
//    mMatrix.setToIdentity();
//    mMatrix.rotate(angle,x,y,z);
    QMatrix4x4 m;
    m.rotate(angle,x,y,z);

    for(int i=0; i<points.size(); i++)
    {
        QVector4D temp=points[i].toVector4D();
        points[i]=(temp*m).toVector3D();
    }
    for(int i=0; i<normals.size(); i++)
    {
        QVector4D temp=normals[i].toVector4D();
        normals[i]=(temp*m).toVector3D();
    }

    //更新顶点和法向数据
    arrayBuf.bind();
    arrayBuf.allocate(getPointsData(), countPoints() * sizeof(QVector3D));

    normalBuf.bind();
    normalBuf.allocate(getNormalsData(),countNormals()*sizeof(QVector3D));
}

void Model::setTexture(const QString &texPath)
{
    texture=new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture->setFormat(QOpenGLTexture::RGBA8U);
    texture->setData(QImage(texPath).mirrored());
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);

    texCoordBox.append(QVector2D(0,0));
    texCoordBox.append(QVector2D(1,1));

    texBuf.create();
    texBuf.bind();
    texBuf.allocate(getTexCoordData(),countTexCoord()*sizeof(QVector2D));
}

void Model::setMatrix(const QMatrix4x4 &matrix)
{
    mMatrix = matrix;
}

ShaderType Model::getShaderType() const
{
    return shaderType;
}

void Model::setShaderType(const ShaderType &value)
{
    shaderType = value;
}

Model::Model()
{
    initializeOpenGLFunctions();
    mMatrix.setToIdentity();

}

Model::~Model()
{
    normalBuf.destroy();
    arrayBuf.destroy();
    indexBuf.destroy();
}
