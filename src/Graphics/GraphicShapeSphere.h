#pragma once
#include "GraphicShapeBase.h"
class GraphicShapeSphere : public GraphicShapeBase {
public:
    GraphicShapeSphere(int prec);
    ~GraphicShapeSphere();

    void initPoints() override;

private:
    int m_iPrec;
};

