#pragma once

#include <string>
#include <vector>

#include "mathlib.h"

struct Vertex {
    // Position Vector
    Vector3 Position;

    // Normal Vector
    Vector3 Normal;

    // Texture Coordinate Vector
    Vector2 TextureCoordinate;
};

struct Mesh {
    // Default Constructor
    Mesh() {}
    // Variable Set Constructor
    Mesh(std::vector<Vertex> &_Vertices, std::vector<unsigned int> &_Indices) {
        vertices = _Vertices;
        indices = _Indices;
    }

    // Mesh Name
    std::string meshName;
    // Vertex List
    std::vector<Vertex> vertices;
    // Index List
    std::vector<unsigned int> indices;
};

class Loader {
   public:
    Loader() {}
    ~Loader() { LoadedMeshes.clear(); }
    bool LoadFile(std::string Path);

    // Loaded Mesh Objects
    std::vector<Mesh> LoadedMeshes;
    // Loaded Vertex Objects
    std::vector<Vertex> LoadedVertices;
    // Loaded Index Positions
    std::vector<unsigned int> LoadedIndices;

   private:
    void GenVerticesFromRawOBJ(std::vector<Vertex> &oVerts,
                               const std::vector<Vector3> &iPositions,
                               const std::vector<Vector2> &iTCoords,
                               const std::vector<Vector3> &iNormals,
                               std::string icurline);

    void VertexTriangluation(std::vector<unsigned int> &oIndices,
                             const std::vector<Vertex> &iVerts);
};
