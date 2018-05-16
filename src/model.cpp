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


    Point3d boundsMin( 1e9, 1e9, 1e9);
    Point3d boundsMax(-1e9,-1e9,-1e9);

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString input = in.readLine();
        if (input.isEmpty() || input[0] == '#')
            continue;

        QTextStream ts(&input);
        QString id;
        ts >> id;
        if (id == "v") {
            Point3d p;
            for (int i = 0; i < 3; ++i) {
                ts >> p[i];
                boundsMin[i] = qMin(boundsMin[i], p[i]);
                boundsMax[i] = qMax(boundsMax[i], p[i]);
            }
            points << p;
        } else if (id == "f" || id == "fo") {
            QVarLengthArray<int, 4> p;

            while (!ts.atEnd()) {
                QString vertex;
                ts >> vertex;
                const int vertexIndex = vertex.split('/').value(0).toInt();
                if (vertexIndex)
                    p.append(vertexIndex > 0 ? vertexIndex - 1 : points.size()+vertexIndex);
            }

            for (int i = 0; i < p.size(); ++i) {
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
    const Point3d bounds = boundsMax - boundsMin;
    const qreal scale = 1 / qMax(bounds.x, qMax(bounds.y, bounds.z));
    for (int i = 0; i < points.size(); ++i)
        points[i] = (points[i]-(boundsMin + bounds*0.5))*scale;

    normals.resize(points.size());
    for (int i = 0; i < pointIndices.size(); i += 3) {
        const Point3d a = points.at(pointIndices.at(i));
        const Point3d b = points.at(pointIndices.at(i+1));
        const Point3d c = points.at(pointIndices.at(i+2));
        const Point3d normal = cross(b-a,c-a).normalize();

        for (int j = 0; j < 3; ++j)
            normals[pointIndices.at(i + j)] += normal;
    }

    for (int i = 0; i < normals.size(); ++i)
        normals[i] = normals[i].normalize();

    return true;
}

bool Model::initBuf()
{
    arrayBuf.create();
    arrayBuf.bind();
    arrayBuf.allocate(getPointsData(), countPoints() * sizeof(Point3d));

    indexBuf.create();
    indexBuf.bind();
    indexBuf.allocate(getIndicesData(), countIndices()* sizeof(GLuint));

    normalBuf.create();
    normalBuf.bind();
    normalBuf.allocate(getNormalsData(),countNormals()*sizeof(Point3d));

    return true;
}

void Model::draw(QOpenGLShaderProgram& shaderProgram)
{
    normalBuf.bind();

    int normalLocation=shaderProgram.attributeLocation("vNormal");
    shaderProgram.enableAttributeArray(normalLocation);
    shaderProgram.setAttributeBuffer(normalLocation,GL_FLOAT,0,3,sizeof(Point3d));

    arrayBuf.bind();
    indexBuf.bind();

    quintptr offset = 0;

    int vertexLocation = shaderProgram.attributeLocation("vVertex");
    shaderProgram.enableAttributeArray(vertexLocation);
    shaderProgram.setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(Point3d));

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

Model::Model()
{
    initializeOpenGLFunctions();
}

Model::~Model()
{
    normalBuf.destroy();
    arrayBuf.destroy();
    indexBuf.destroy();
}
