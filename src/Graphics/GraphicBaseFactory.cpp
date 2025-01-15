#include "GraphicsBase.h"

#include "GraphicsPoint.h"
#include "GraphicsTriangle.h"
#include "GraphicsAnimation.h"
#include "GraphicsCube.h"
#include "GraphicsInstance.h"
#include "GraphicsStackAni.h"
#include "GraphicsRectangle.h"
#include "GraphicsGradientTriangle.h"
#include "GraphicsImageTexture.h"
#include "GraphicsImage3D.h"
#include "GraphicsCamera.h"
#include "GraphicsConeWithTex.h"
#include "GraphicsSphere.h"
#include "GraphicsTorus.h"
#include "GraphicsShadow.h"


std::shared_ptr<GraphicBase> GraphicBase::createGraphicObj(GLFWwindow *window, GraphicType type) {
    if (type < 0) {
        return nullptr;
    }

    GraphicSpObject graphic = nullptr;

    switch (type) {
    case GraphicType_Point:
        graphic = std::make_shared<GraphicPoint>();
        break;
    case GraphicType_triangle:
        graphic = std::make_shared <GraphicTriagle>();
        break;
    case GraphicType_rectangle:
        graphic = std::make_shared<GraphicRectangle>();
        break;
    case GraphicType_animation:
        graphic = std::make_shared<GraphicAnimation>();
        break;
    case GraphicType_cube:
        graphic = std::make_shared<GraphicCube>();
        break;
    case GraphicType_instance:
        graphic = std::make_shared<GraphicInstance>();
        break;
    case GraphicType_StackAni:
        graphic = std::make_shared<GraphicStackAni>();
        break;
    case GraphicType_GradientTriangle:
        graphic = std::make_shared<GraphicGradientTriangle>();
        break;
    case GraphicType_GraphicImage:
        graphic = std::make_shared<GraphicImageTexture>();
        break;
    case GraphicType_3DTexture:
        graphic = std::make_shared<GraphicImage3D>();
        break;
    case GraphicType_camera:
        graphic = std::make_shared<GraphicCamera>();
        break;
    case GraphicType_ConeWithTex:
        graphic = std::make_shared<GraphicsConeWithTex>();
        break;
    case GraphicType_Sphere:
        graphic = std::make_shared<GraphicsSphere>();
        break;
    case GraphicType_Torus:
        graphic = std::make_shared<GraphicTorus>();
        break;
    case GraphicType_Shadow:
        graphic = std::make_shared<GraphicShadow>();
        break;
    default:
        return nullptr;
        break;
    }

    bool graphicInit = graphic->init(window);
    if (!graphicInit) {
        graphic = nullptr;
        // TODO: graphic init error;
        return nullptr;
    }

    return graphic;
}