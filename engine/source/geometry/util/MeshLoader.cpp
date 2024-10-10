#include "MeshLoader.h"
#include <fstream>
namespace algorithm
{
    // Vector3 Multiplication Opertor Overload
    Vector3 operator*(const float &left, const Vector3 &right)
    {
        return Vector3(right.x * left, right.y * left, right.z * left);
    }

    // A test to see if P1 is on the same side as P2 of a line segment ab
    bool SameSide(Vector3 p1, Vector3 p2, Vector3 a, Vector3 b)
    {
        Vector3 cp1 = Vector3::Cross(b - a, p1 - a);
        Vector3 cp2 = Vector3::Cross(b - a, p2 - a);

        if (Vector3::Dot(cp1, cp2) >= 0)
            return true;
        else
            return false;
    }

    // Generate a cross produect normal for a triangle
    Vector3 GenTriNormal(Vector3 t1, Vector3 t2, Vector3 t3)
    {
        Vector3 u = t2 - t1;
        Vector3 v = t3 - t1;

        Vector3 normal = Vector3::Cross(u, v);

        return normal;
    }

    // Check to see if a Vector3 Point is within a 3 Vector3 Triangle
    bool inTriangle(Vector3 point, Vector3 tri1, Vector3 tri2, Vector3 tri3)
    {
        // Test to see if it is within an infinite prism that the triangle outlines.
        bool within_tri_prisim = SameSide(point, tri1, tri2, tri3) && SameSide(point, tri2, tri1, tri3) && SameSide(point, tri3, tri1, tri2);

        // If it isn't it will never be on the triangle
        if (!within_tri_prisim)
            return false;

        // Calulate Triangle's Normal
        Vector3 n = GenTriNormal(tri1, tri2, tri3);

        // Project the point onto this normal
        Vector3 proj = Vector3::Projection(point, n);

        // If the distance from the triangle to the point is 0
        //	it lies on the triangle
        if (proj.length() < 0.000001f)
            return true;
        else
            return false;
    }

    // Split a String into a string array at a given token
    inline void split(const std::string &in,
                      std::vector<std::string> &out,
                      std::string token)
    {
        out.clear();

        std::string temp;

        for (int i = 0; i < int(in.size()); i++)
        {
            std::string test = in.substr(i, token.size());

            if (test == token)
            {
                if (!temp.empty())
                {
                    out.push_back(temp);
                    temp.clear();
                    i += (int)token.size() - 1;
                }
                else
                {
                    out.push_back("");
                }
            }
            else if (i + token.size() >= in.size())
            {
                temp += in.substr(i, token.size());
                out.push_back(temp);
                break;
            }
            else
            {
                temp += in[i];
            }
        }
    }

    // Get tail of string after first token and possibly following spaces
    inline std::string tail(const std::string &in)
    {
        size_t token_start = in.find_first_not_of(" \t");
        size_t space_start = in.find_first_of(" \t", token_start);
        size_t tail_start = in.find_first_not_of(" \t", space_start);
        size_t tail_end = in.find_last_not_of(" \t");
        if (tail_start != std::string::npos && tail_end != std::string::npos)
        {
            return in.substr(tail_start, tail_end - tail_start + 1);
        }
        else if (tail_start != std::string::npos)
        {
            return in.substr(tail_start);
        }
        return "";
    }

    // Get first token of string
    inline std::string firstToken(const std::string &in)
    {
        if (!in.empty())
        {
            size_t token_start = in.find_first_not_of(" \t");
            size_t token_end = in.find_first_of(" \t", token_start);
            if (token_start != std::string::npos && token_end != std::string::npos)
            {
                return in.substr(token_start, token_end - token_start);
            }
            else if (token_start != std::string::npos)
            {
                return in.substr(token_start);
            }
        }
        return "";
    }

    // Get element at given index position
    template <class T>
    inline const T &getElement(const std::vector<T> &elements, std::string &index)
    {
        int idx = std::stoi(index);
        if (idx < 0)
            idx = int(elements.size()) + idx;
        else
            idx--;
        return elements[idx];
    }
}

bool Loader::LoadFile(std::string Path)
{
    if (Path.substr(Path.size() - 4, 4) != ".obj")
        return false;

    std::ifstream file(Path);

    if (!file.is_open())
        return false;

    LoadedMeshes.clear();
    LoadedVertices.clear();
    LoadedIndices.clear();

    std::vector<Vector3> Positions;
    std::vector<Vector2> TCoords;
    std::vector<Vector3> Normals;

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

    std::vector<std::string> MeshMatNames;

    std::string meshname;
    Mesh tempMesh;

    bool listening = false;

    std::string curline;
    while (std::getline(file, curline))
    // Generate a Mesh Object or Prepare for an object to be created
    {
        if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g" || curline[0] == 'g')
        {
            if (!listening)
            {
                listening = true;

                if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g")
                {
                    meshname = algorithm::tail(curline);
                }
                else
                {
                    meshname = "unnamed";
                }
            }
            else
            {
                // Generate the mesh to put into the array

                if (!Indices.empty() && !Vertices.empty())
                {
                    // Create Mesh
                    tempMesh = Mesh(Vertices, Indices);
                    tempMesh.meshName = meshname;

                    // Insert Mesh
                    LoadedMeshes.push_back(tempMesh);

                    // Cleanup
                    Vertices.clear();
                    Indices.clear();
                    meshname.clear();

                    meshname = algorithm::tail(curline);
                }
                else
                {
                    if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g")
                    {
                        meshname = algorithm::tail(curline);
                    }
                    else
                    {
                        meshname = "unnamed";
                    }
                }
            }
        }
        // Generate a Vertex Position
        if (algorithm::firstToken(curline) == "v")
        {
            std::vector<std::string> spos;
            Vector3 vpos;
            algorithm::split(algorithm::tail(curline), spos, " ");

            vpos.x = std::stof(spos[0]);
            vpos.y = std::stof(spos[1]);
            vpos.z = std::stof(spos[2]);

            Positions.push_back(vpos);
        }
        // Generate a Vertex Texture Coordinate
        if (algorithm::firstToken(curline) == "vt")
        {
            std::vector<std::string> stex;
            Vector2 vtex;
            algorithm::split(algorithm::tail(curline), stex, " ");

            vtex.x = std::stof(stex[0]);
            vtex.y = std::stof(stex[1]);

            TCoords.push_back(vtex);
        }
        // Generate a Vertex Normal;
        if (algorithm::firstToken(curline) == "vn")
        {
            std::vector<std::string> snor;
            Vector3 vnor;
            algorithm::split(algorithm::tail(curline), snor, " ");

            vnor.x = std::stof(snor[0]);
            vnor.y = std::stof(snor[1]);
            vnor.z = std::stof(snor[2]);

            Normals.push_back(vnor);
        }
        // Generate a Face (vertices & indices)
        if (algorithm::firstToken(curline) == "f")
        {
            // Generate the vertices
            std::vector<Vertex> vVerts;
            GenVerticesFromRawOBJ(vVerts, Positions, TCoords, Normals, curline);

            // Add Vertices
            for (int i = 0; i < int(vVerts.size()); i++)
            {
                Vertices.push_back(vVerts[i]);

                LoadedVertices.push_back(vVerts[i]);
            }

            std::vector<unsigned int> iIndices;

            VertexTriangluation(iIndices, vVerts);

            // Add Indices
            for (int i = 0; i < int(iIndices.size()); i++)
            {
                unsigned int indnum = (unsigned int)((Vertices.size()) - vVerts.size()) + iIndices[i];
                Indices.push_back(indnum);

                indnum = (unsigned int)((LoadedVertices.size()) - vVerts.size()) + iIndices[i];
                LoadedIndices.push_back(indnum);
            }
        }
        // Get Mesh Material Name
        if (algorithm::firstToken(curline) == "usemtl")
        {
            MeshMatNames.push_back(algorithm::tail(curline));

            // Create new Mesh, if Material changes within a group
            if (!Indices.empty() && !Vertices.empty())
            {
                // Create Mesh
                tempMesh = Mesh(Vertices, Indices);
                tempMesh.meshName = meshname;
                int i = 2;
                while (1)
                {
                    tempMesh.meshName = meshname + "_" + std::to_string(i);

                    for (auto &m : LoadedMeshes)
                        if (m.meshName == tempMesh.meshName)
                            continue;
                    break;
                }

                // Insert Mesh
                LoadedMeshes.push_back(tempMesh);

                // Cleanup
                Vertices.clear();
                Indices.clear();
            }
        }
        // Load Materials
        if (algorithm::firstToken(curline) == "mtllib")
        {
            // skip
        }
    }

    // Deal with last mesh

    if (!Indices.empty() && !Vertices.empty())
    {
        // Create Mesh
        tempMesh = Mesh(Vertices, Indices);
        tempMesh.meshName = meshname;

        // Insert Mesh
        LoadedMeshes.push_back(tempMesh);
    }

    file.close();

    // Set Materials for each Mesh
    for (int i = 0; i < MeshMatNames.size(); i++)
    {
        std::string matname = MeshMatNames[i];

        // Find corresponding material name in loaded materials
        // when found copy material variables into mesh material
        // for (int j = 0; j < LoadedMaterials.size(); j++)
        // {
        //     if (LoadedMaterials[j].name == matname)
        //     {
        //         LoadedMeshes[i].MeshMaterial = LoadedMaterials[j];
        //         break;
        //     }
        // }
    }
    if (LoadedMeshes.empty() && LoadedVertices.empty() && LoadedIndices.empty())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void VertexTriangluation(std::vector<unsigned int> &oIndices,
                         const std::vector<Vertex> &iVerts)
{
    // If there are 2 or less verts,
    // no triangle can be created,
    // so exit
    if (iVerts.size() < 3)
    {
        return;
    }
    // If it is a triangle no need to calculate it
    if (iVerts.size() == 3)
    {
        oIndices.push_back(0);
        oIndices.push_back(1);
        oIndices.push_back(2);
        return;
    }

    // Create a list of vertices
    std::vector<Vertex> tVerts = iVerts;

    while (true)
    {
        // For every vertex
        for (int i = 0; i < int(tVerts.size()); i++)
        {
            // pPrev = the previous vertex in the list
            Vertex pPrev;
            if (i == 0)
            {
                pPrev = tVerts[tVerts.size() - 1];
            }
            else
            {
                pPrev = tVerts[i - 1];
            }

            // pCur = the current vertex;
            Vertex pCur = tVerts[i];

            // pNext = the next vertex in the list
            Vertex pNext;
            if (i == tVerts.size() - 1)
            {
                pNext = tVerts[0];
            }
            else
            {
                pNext = tVerts[i + 1];
            }

            // Check to see if there are only 3 verts left
            // if so this is the last triangle
            if (tVerts.size() == 3)
            {
                // Create a triangle from pCur, pPrev, pNext
                for (int j = 0; j < int(tVerts.size()); j++)
                {
                    if (iVerts[j].Position == pCur.Position)
                        oIndices.push_back(j);
                    if (iVerts[j].Position == pPrev.Position)
                        oIndices.push_back(j);
                    if (iVerts[j].Position == pNext.Position)
                        oIndices.push_back(j);
                }

                tVerts.clear();
                break;
            }
            if (tVerts.size() == 4)
            {
                // Create a triangle from pCur, pPrev, pNext
                for (int j = 0; j < int(iVerts.size()); j++)
                {
                    if (iVerts[j].Position == pCur.Position)
                        oIndices.push_back(j);
                    if (iVerts[j].Position == pPrev.Position)
                        oIndices.push_back(j);
                    if (iVerts[j].Position == pNext.Position)
                        oIndices.push_back(j);
                }

                Vector3 tempVec;
                for (int j = 0; j < int(tVerts.size()); j++)
                {
                    if (tVerts[j].Position != pCur.Position && tVerts[j].Position != pPrev.Position && tVerts[j].Position != pNext.Position)
                    {
                        tempVec = tVerts[j].Position;
                        break;
                    }
                }

                // Create a triangle from pCur, pPrev, pNext
                for (int j = 0; j < int(iVerts.size()); j++)
                {
                    if (iVerts[j].Position == pPrev.Position)
                        oIndices.push_back(j);
                    if (iVerts[j].Position == pNext.Position)
                        oIndices.push_back(j);
                    if (iVerts[j].Position == tempVec)
                        oIndices.push_back(j);
                }

                tVerts.clear();
                break;
            }

            // If Vertex is not an interior vertex
            float angle = Vector3::Angle(pPrev.Position - pCur.Position, pNext.Position - pCur.Position) * (180 / 3.14159265359);
            if (angle <= 0 && angle >= 180)
                continue;

            // If any vertices are within this triangle
            bool inTri = false;
            for (int j = 0; j < int(iVerts.size()); j++)
            {
                if (algorithm::inTriangle(iVerts[j].Position, pPrev.Position, pCur.Position, pNext.Position) && iVerts[j].Position != pPrev.Position && iVerts[j].Position != pCur.Position && iVerts[j].Position != pNext.Position)
                {
                    inTri = true;
                    break;
                }
            }
            if (inTri)
                continue;

            // Create a triangle from pCur, pPrev, pNext
            for (int j = 0; j < int(iVerts.size()); j++)
            {
                if (iVerts[j].Position == pCur.Position)
                    oIndices.push_back(j);
                if (iVerts[j].Position == pPrev.Position)
                    oIndices.push_back(j);
                if (iVerts[j].Position == pNext.Position)
                    oIndices.push_back(j);
            }

            // Delete pCur from the list
            for (int j = 0; j < int(tVerts.size()); j++)
            {
                if (tVerts[j].Position == pCur.Position)
                {
                    tVerts.erase(tVerts.begin() + j);
                    break;
                }
            }

            // reset i to the start
            // -1 since loop will add 1 to it
            i = -1;
        }

        // if no triangles were created
        if (oIndices.size() == 0)
            break;

        // if no more vertices
        if (tVerts.size() == 0)
            break;
    }
}

void Loader::GenVerticesFromRawOBJ(std::vector<Vertex> &oVerts,
                                   const std::vector<Vector3> &iPositions,
                                   const std::vector<Vector2> &iTCoords,
                                   const std::vector<Vector3> &iNormals,
                                   std::string icurline)
{
    std::vector<std::string> sface, svert;
    Vertex vVert;
    algorithm::split(algorithm::tail(icurline), sface, " ");

    bool noNormal = false;

    // For every given vertex do this
    for (int i = 0; i < int(sface.size()); i++)
    {
        // See What type the vertex is.
        int vtype;

        algorithm::split(sface[i], svert, "/");

        // Check for just position - v1
        if (svert.size() == 1)
        {
            // Only position
            vtype = 1;
        }

        // Check for position & texture - v1/vt1
        if (svert.size() == 2)
        {
            // Position & Texture
            vtype = 2;
        }

        // Check for Position, Texture and Normal - v1/vt1/vn1
        // or if Position and Normal - v1//vn1
        if (svert.size() == 3)
        {
            if (svert[1] != "")
            {
                // Position, Texture, and Normal
                vtype = 4;
            }
            else
            {
                // Position & Normal
                vtype = 3;
            }
        }

        // Calculate and store the vertex
        switch (vtype)
        {
        case 1: // P
        {
            vVert.Position = algorithm::getElement(iPositions, svert[0]);
            vVert.TextureCoordinate = Vector2(0, 0);
            noNormal = true;
            oVerts.push_back(vVert);
            break;
        }
        case 2: // P/T
        {
            vVert.Position = algorithm::getElement(iPositions, svert[0]);
            vVert.TextureCoordinate = algorithm::getElement(iTCoords, svert[1]);
            noNormal = true;
            oVerts.push_back(vVert);
            break;
        }
        case 3: // P//N
        {
            vVert.Position = algorithm::getElement(iPositions, svert[0]);
            vVert.TextureCoordinate = Vector2(0, 0);
            vVert.Normal = algorithm::getElement(iNormals, svert[2]);
            oVerts.push_back(vVert);
            break;
        }
        case 4: // P/T/N
        {
            vVert.Position = algorithm::getElement(iPositions, svert[0]);
            vVert.TextureCoordinate = algorithm::getElement(iTCoords, svert[1]);
            vVert.Normal = algorithm::getElement(iNormals, svert[2]);
            oVerts.push_back(vVert);
            break;
        }
        default:
        {
            break;
        }
        }
    }

    // take care of missing normals
    // these may not be truly acurate but it is the
    // best they get for not compiling a mesh with normals
    if (noNormal)
    {
        Vector3 A = oVerts[0].Position - oVerts[1].Position;
        Vector3 B = oVerts[2].Position - oVerts[1].Position;

        Vector3 normal = Vector3::Cross(A, B);

        for (int i = 0; i < int(oVerts.size()); i++)
        {
            oVerts[i].Normal = normal;
        }
    }
}

void Loader::VertexTriangluation(std::vector<unsigned int> &oIndices,
                                 const std::vector<Vertex> &iVerts)
{
    // If there are 2 or less verts,
    // no triangle can be created,
    // so exit
    if (iVerts.size() < 3)
    {
        return;
    }
    // If it is a triangle no need to calculate it
    if (iVerts.size() == 3)
    {
        oIndices.push_back(0);
        oIndices.push_back(1);
        oIndices.push_back(2);
        return;
    }

    // Create a list of vertices
    std::vector<Vertex> tVerts = iVerts;

    while (true)
    {
        // For every vertex
        for (int i = 0; i < int(tVerts.size()); i++)
        {
            // pPrev = the previous vertex in the list
            Vertex pPrev;
            if (i == 0)
            {
                pPrev = tVerts[tVerts.size() - 1];
            }
            else
            {
                pPrev = tVerts[i - 1];
            }

            // pCur = the current vertex;
            Vertex pCur = tVerts[i];

            // pNext = the next vertex in the list
            Vertex pNext;
            if (i == tVerts.size() - 1)
            {
                pNext = tVerts[0];
            }
            else
            {
                pNext = tVerts[i + 1];
            }

            // Check to see if there are only 3 verts left
            // if so this is the last triangle
            if (tVerts.size() == 3)
            {
                // Create a triangle from pCur, pPrev, pNext
                for (int j = 0; j < int(tVerts.size()); j++)
                {
                    if (iVerts[j].Position == pCur.Position)
                        oIndices.push_back(j);
                    if (iVerts[j].Position == pPrev.Position)
                        oIndices.push_back(j);
                    if (iVerts[j].Position == pNext.Position)
                        oIndices.push_back(j);
                }

                tVerts.clear();
                break;
            }
            if (tVerts.size() == 4)
            {
                // Create a triangle from pCur, pPrev, pNext
                for (int j = 0; j < int(iVerts.size()); j++)
                {
                    if (iVerts[j].Position == pCur.Position)
                        oIndices.push_back(j);
                    if (iVerts[j].Position == pPrev.Position)
                        oIndices.push_back(j);
                    if (iVerts[j].Position == pNext.Position)
                        oIndices.push_back(j);
                }

                Vector3 tempVec;
                for (int j = 0; j < int(tVerts.size()); j++)
                {
                    if (tVerts[j].Position != pCur.Position && tVerts[j].Position != pPrev.Position && tVerts[j].Position != pNext.Position)
                    {
                        tempVec = tVerts[j].Position;
                        break;
                    }
                }

                // Create a triangle from pCur, pPrev, pNext
                for (int j = 0; j < int(iVerts.size()); j++)
                {
                    if (iVerts[j].Position == pPrev.Position)
                        oIndices.push_back(j);
                    if (iVerts[j].Position == pNext.Position)
                        oIndices.push_back(j);
                    if (iVerts[j].Position == tempVec)
                        oIndices.push_back(j);
                }

                tVerts.clear();
                break;
            }

            // If Vertex is not an interior vertex
            float angle = Vector3::Angle(pPrev.Position - pCur.Position, pNext.Position - pCur.Position) * (180 / 3.14159265359);
            if (angle <= 0 && angle >= 180)
                continue;

            // If any vertices are within this triangle
            bool inTri = false;
            for (int j = 0; j < int(iVerts.size()); j++)
            {
                if (algorithm::inTriangle(iVerts[j].Position, pPrev.Position, pCur.Position, pNext.Position) && iVerts[j].Position != pPrev.Position && iVerts[j].Position != pCur.Position && iVerts[j].Position != pNext.Position)
                {
                    inTri = true;
                    break;
                }
            }
            if (inTri)
                continue;

            // Create a triangle from pCur, pPrev, pNext
            for (int j = 0; j < int(iVerts.size()); j++)
            {
                if (iVerts[j].Position == pCur.Position)
                    oIndices.push_back(j);
                if (iVerts[j].Position == pPrev.Position)
                    oIndices.push_back(j);
                if (iVerts[j].Position == pNext.Position)
                    oIndices.push_back(j);
            }

            // Delete pCur from the list
            for (int j = 0; j < int(tVerts.size()); j++)
            {
                if (tVerts[j].Position == pCur.Position)
                {
                    tVerts.erase(tVerts.begin() + j);
                    break;
                }
            }

            // reset i to the start
            // -1 since loop will add 1 to it
            i = -1;
        }

        // if no triangles were created
        if (oIndices.size() == 0)
            break;

        // if no more vertices
        if (tVerts.size() == 0)
            break;
    }
}
