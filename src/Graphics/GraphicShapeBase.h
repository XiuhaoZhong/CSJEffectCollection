#pragma once

#include <vector>
#include <memory>

#include "glm/gtc/type_ptr.hpp"

#include "GraphicModelImporter.h"

using GraphicVec3Array = std::vector<glm::vec3>;
using GraphicVec2Array = std::vector<glm::vec2>;
using GraphicIntArray = std::vector<int>;

class GraphicShapeBase {
public:
    static std::shared_ptr<GraphicShapeBase> createSphere(int prec);
    static std::shared_ptr<GraphicShapeBase> createPraymid(std::string modelFile);
    static std::shared_ptr<GraphicShapeBase> createTorus(float inner, float outter, int prec);

    GraphicShapeBase();
    virtual ~GraphicShapeBase();

    // Initialize vertexes, texture coordinates, normals and so on.
    virtual void initPoints() {};

    // Get all the vertexes.
    virtual GraphicVec3Array getVertices() {
        return m_vVertices;
    };

    virtual GraphicIntArray getIndices() {
        return m_vIndices;
    }

    // Get all the texture coordinates.
    virtual GraphicVec2Array getTexCoords() {
        return m_vTexCoords;
    };

    virtual GraphicVec3Array getNormals() {
        return m_vNormals;
    };

    virtual GraphicVec3Array getTTangents() {
        return m_vTTangents;
    }

    virtual GraphicVec3Array getSTangents() {
        return m_vSTangents;
    }

    // Get the number of vertexes.
    virtual int getVerticeNum() {
        return m_iNumVertices;
    };

    // Get the number texture coordinates.
    virtual int getIndiceNum() {
        return m_iNumIndices;
    };

protected:
    GraphicIntArray  m_vIndices;   // vertexes' index vector
    GraphicVec3Array m_vVertices;  // vertex vector
    GraphicVec2Array m_vTexCoords; // texture coordinates vector
    GraphicVec3Array m_vNormals;   // Normal vector 
    GraphicVec3Array m_vSTangents; // Horizontal tangent vectors
    GraphicVec3Array m_vTTangents; // Vertical tangent vectors

    int m_iNumVertices;
    int m_iNumIndices;
};

using GraphicSpShape = std::shared_ptr<GraphicShapeBase>;

