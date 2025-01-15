#pragma once
#include "GraphicShapeBase.h"
class GraphicShapeTorus : public GraphicShapeBase {
public:
    GraphicShapeTorus(float inner, float outter, int prec);
    ~GraphicShapeTorus();

    void initPoints() override;

private:
    float m_fInner;
    float m_fOuter;
    int   m_iPrec;

};

