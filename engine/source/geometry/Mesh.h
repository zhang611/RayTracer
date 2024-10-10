#pragma once
#include "scene/Scene.h"
#include <cassert>
#include "mathlib.h"
#include "util/MeshLoader.h"
#include "Triangle.h"
#include <string>


namespace RayTracer
{
    class MeshTriangle : public Scene
    {
    public:
        MeshTriangle(std::string filename, std::shared_ptr<Material> m)
        :MeshTriangle(filename, Vector3::zero, m)
        {
        }
        MeshTriangle(std::string filename, Vector3 pos, std::shared_ptr<Material> m)
        {
            Loader loader;
            loader.LoadFile(filename);

            assert(loader.LoadedMeshes.size() == 1);
            auto mesh = loader.LoadedMeshes[0];
            Vector3 face_vertices[3];
            for (unsigned int i = 0; i < mesh.vertices.size(); i += 3) {
                for (int j = 0; j < 3; j++) {
                    auto vert = Vector3(mesh.vertices[i + j].Position.x,
                                        mesh.vertices[i + j].Position.y,
                                        mesh.vertices[i + j].Position.z) *
                                60.f;
                    vert += pos;
                    face_vertices[j] = vert;
                }
                add(std::make_shared<Triangle>(face_vertices[0], face_vertices[1], face_vertices[2], m));
            }

            BuildBVH();
        }

        ~MeshTriangle() override {}
    private:
    };

}