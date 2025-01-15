#include "GraphicShapeBase.h"

#include "GraphicShapeSphere.h"
#include "GraphicShapeTorus.h"
#include "GraphicShapePyramid.h"

std::shared_ptr<GraphicShapeBase> GraphicShapeBase::createSphere(int prec) {
    return std::make_shared<GraphicShapeSphere>(prec);
}

std::shared_ptr<GraphicShapeBase> GraphicShapeBase::createPraymid(std::string modelFile) {
    return std::make_shared<GraphicShapePyramid>(modelFile);
}

std::shared_ptr<GraphicShapeBase> GraphicShapeBase::createTorus(float inner, float outter, int prec) {
    return std::make_shared<GraphicShapeTorus>(inner, outter, prec);
}

GraphicShapeBase::GraphicShapeBase() {

}

GraphicShapeBase::~GraphicShapeBase() {
    m_vVertices.clear();
    m_vTexCoords.clear();
    m_vNormals.clear();
}
