#include "GraphicShapeTorus.h"
#include "Utils/utils.h"

GraphicShapeTorus::GraphicShapeTorus(float inner, float outter, int prec)
    : m_fInner(inner)
    , m_fOuter(outter)
    , m_iPrec(prec){

    initPoints();
}

GraphicShapeTorus::~GraphicShapeTorus() {

}

void GraphicShapeTorus::initPoints() {
    m_iNumVertices = (m_iPrec + 1) * (m_iPrec + 1);
    m_iNumIndices = m_iPrec * m_iPrec * 6;

    for (int i = 0; i < m_iNumVertices; i++) {
        m_vVertices.push_back(glm::vec3());
        m_vTexCoords.push_back(glm::vec2());
        m_vNormals.push_back(glm::vec3());
        m_vSTangents.push_back(glm::vec3());
        m_vTTangents.push_back(glm::vec3());
    }

    for (int i = 0; i < m_iNumIndices; i++) {
        m_vIndices.push_back(0);
    }

    // the first ring.
    for (int i = 0; i < m_iPrec + 1; i++) {
        float amt = Utils::toRadians(i * 360.0f / m_iPrec);

        glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::vec3 pos(rMat * glm::vec4(m_fOuter, 0.0f, 0.0f, 1.0f));

        m_vVertices[i] = glm::vec3(pos + glm::vec3(m_fInner, 0.0f, 0.0f));
        m_vTexCoords[i] = glm::vec2(0.0f, (float)i / (float)m_iPrec);

        rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
        m_vTTangents[i] = glm::vec3(rMat * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));

        m_vSTangents[i] = glm::vec3(glm::vec3(0.0f, 0.0f, -1.0f));

        m_vNormals[i] = glm::cross(m_vTTangents[i], m_vSTangents[i]);
    }

    // rest of the ring.
    for (int i = 1; i < m_iPrec + 1; i++) {
        for (int j = 0; j < m_iPrec + 1; j++) {
            float amt = Utils::toRadians((float)i * 360.0f / m_iPrec);

            glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
            m_vVertices[i * (m_iPrec + 1) + j] = glm::vec3(rMat * glm::vec4(m_vVertices[j], 1.0f));

            m_vTexCoords[i * (m_iPrec + 1) + j] = glm::vec2((float)i * 2.0f / (float)m_iPrec, m_vTexCoords[j].t);
            if (m_vTexCoords[i * (m_iPrec + 1) + j].s > 1.0) {
                m_vTexCoords[i * (m_iPrec + 1) + j].s -= 1.0f;
            }

            rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
            m_vSTangents[i * (m_iPrec + 1) + j] = glm::vec3(rMat * glm::vec4(m_vSTangents[j], 1.0f));

            rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
            m_vTTangents[i * (m_iPrec + 1) + j] = glm::vec3(rMat * glm::vec4(m_vTTangents[j], 1.0f));

            rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
            m_vNormals[i * (m_iPrec + 1) + j] = glm::vec3(rMat * glm::vec4(m_vNormals[j], 1.0f));
        }
    }

    for (int ring = 0; ring < m_iPrec; ring++) {
        for (int i = 0; i < m_iPrec; i++) {
            m_vIndices[((ring * m_iPrec + i) * 2) * 3 + 0] = ring * (m_iPrec + 1) + i;
            m_vIndices[((ring * m_iPrec + i) * 2) * 3 + 1] = (ring + 1) * (m_iPrec + 1) + i;
            m_vIndices[((ring * m_iPrec + i) * 2) * 3 + 2] = ring * (m_iPrec + 1) + i + 1;
            m_vIndices[((ring * m_iPrec + i) * 2 + 1) * 3 + 0] = ring * (m_iPrec + 1) + i + 1;
            m_vIndices[((ring * m_iPrec + i) * 2 + 1) * 3 + 1] = (ring + 1) * (m_iPrec + 1) + i;
            m_vIndices[((ring * m_iPrec + i) * 2 + 1) * 3 + 2] = (ring + 1) * (m_iPrec + 1) + i + 1;
        }
    }
}
