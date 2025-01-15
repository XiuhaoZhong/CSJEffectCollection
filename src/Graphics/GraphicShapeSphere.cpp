#include "GraphicShapeSphere.h"

#include "Utils/utils.h"

GraphicShapeSphere::GraphicShapeSphere(int prec)
    : m_iPrec(prec) {
    initPoints();
}

GraphicShapeSphere::~GraphicShapeSphere() {

}

void GraphicShapeSphere::initPoints() {
    m_iNumVertices = (m_iPrec + 1) * (m_iPrec + 1);
    m_iNumIndices = m_iPrec * m_iPrec * 6;

    for (int i = 0; i < m_iNumVertices; i++) {
        m_vVertices.push_back(glm::vec3());
        m_vTexCoords.push_back(glm::vec2());
        m_vNormals.push_back(glm::vec3());
        m_vTTangents.push_back(glm::vec3());
    }

    for (int i = 0; i < m_iNumIndices; i++) {
        m_vIndices.push_back(0);
    }

    for (int i = 0; i <= m_iPrec; i++) {
        for (int j = 0; j <= m_iPrec; j++) {
            float y = (float)cos(Utils::toRadians(180.0f - i * 180.0f / m_iPrec));
            float x = -(float)cos(Utils::toRadians(j * 360.0f / m_iPrec)) * (float)abs(cos(asin(y)));
            float z = (float)sin(Utils::toRadians(j * 360.0f / m_iPrec)) * (float)abs(cos(asin(y)));

            m_vVertices[i * (m_iPrec + 1) + j] = glm::vec3(x, y, z);
            m_vTexCoords[i * (m_iPrec + 1) + j] = glm::vec2(((float)j / m_iPrec), ((float)i / m_iPrec));
            m_vNormals[i * (m_iPrec + 1) + j] = glm::vec3(x, y, z);

            // calculate tangent vector
            if (((x == 0) && (y == 1) && (z == 0)) || ((x == 0) && (y == -1) && (z == 0))) {
                m_vTTangents[i * (m_iPrec + 1) + j] = glm::vec3(0.0f, 0.0f, -1.0f);
            } else {
                m_vTTangents[i * (m_iPrec + 1) + j] = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(x, y, z));
            }
        }
    }

    // calculate triangle indices
    for (int i = 0; i < m_iPrec; i++) {
        for (int j = 0; j < m_iPrec; j++) {
            m_vIndices[6 * (i * m_iPrec + j) + 0] = i * (m_iPrec + 1) + j;
            m_vIndices[6 * (i * m_iPrec + j) + 1] = i * (m_iPrec + 1) + j + 1;
            m_vIndices[6 * (i * m_iPrec + j) + 2] = (i + 1) * (m_iPrec + 1) + j;
            m_vIndices[6 * (i * m_iPrec + j) + 3] = i * (m_iPrec + 1) + j + 1;
            m_vIndices[6 * (i * m_iPrec + j) + 4] = (i + 1) * (m_iPrec + 1) + j + 1;
            m_vIndices[6 * (i * m_iPrec + j) + 5] = (i + 1) * (m_iPrec + 1) + j;
        }
    }
}
