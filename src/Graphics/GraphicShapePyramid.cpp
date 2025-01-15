#include "GraphicShapePyramid.h"

GraphicShapePyramid::GraphicShapePyramid() {

}

GraphicShapePyramid::GraphicShapePyramid(std::string modelFile) {
    m_model = GraphicModel(modelFile);
    
    initPoints();
}

GraphicShapePyramid::~GraphicShapePyramid() {

}

void GraphicShapePyramid::initPoints() {
    m_vVertices =  m_model.getVertices();
    m_vTexCoords =  m_model.getTexCoords();
    m_iNumVertices =  m_model.getNumVertices();
    m_vNormals = m_model.getNormals();
}
