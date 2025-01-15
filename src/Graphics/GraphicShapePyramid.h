#pragma once
#include "GraphicShapeBase.h"

#include <string>

class GraphicShapePyramid : public GraphicShapeBase {
public:
    GraphicShapePyramid();
    GraphicShapePyramid(std::string modelFile);
    ~GraphicShapePyramid();

    void initPoints() override;

private:
    GraphicModel m_model;
};

