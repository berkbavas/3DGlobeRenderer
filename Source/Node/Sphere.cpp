///////////////////////////////////////////////////////////////////////////////
// Sphere.cpp
// ==========
// Sphere for OpenGL with (radius, sectors, stacks)
// The min number of sectors is 3 and the min number of stacks are 2.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2017-11-01
// UPDATED: 2019-12-18
///////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#include <windows.h> // include windows.h to avoid thousands of compile errors even though this class is not depending on Windows
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "Sphere.h"

#include <cmath>
#include <iomanip>
#include <sstream>

// constants //////////////////////////////////////////////////////////////////
const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT = 2;

///////////////////////////////////////////////////////////////////////////////
// ctor
///////////////////////////////////////////////////////////////////////////////
GlobeRenderer::Sphere::Sphere(float radius, int sectors, int stacks, bool smooth)
    : interleavedStride(32)
{
    Set(radius, sectors, stacks, smooth);
}

///////////////////////////////////////////////////////////////////////////////
// OpenGL Stuff
///////////////////////////////////////////////////////////////////////////////

void GlobeRenderer::Sphere::Construct()
{
    initializeOpenGLFunctions();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glGenBuffers(3, &vbo[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) nullptr);
}

void GlobeRenderer::Sphere::Render()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (unsigned int) indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void GlobeRenderer::Sphere::Destroy()
{
    if (vao)
    {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }

    if (ebo)
    {
        glDeleteBuffers(1, &ebo);
        ebo = 0;
    }

    for (int i = 0; i < 3; ++i)
    {
        if (vbo[i])
        {
            glDeleteBuffers(1, &vbo[i]);
            vbo[i] = 0;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// setters
///////////////////////////////////////////////////////////////////////////////
void GlobeRenderer::Sphere::Set(float radius, int sectors, int stacks, bool smooth)
{
    this->radius = radius;
    this->sectorCount = sectors;
    if (sectors < MIN_SECTOR_COUNT)
        this->sectorCount = MIN_SECTOR_COUNT;
    this->stackCount = stacks;
    if (sectors < MIN_STACK_COUNT)
        this->sectorCount = MIN_STACK_COUNT;
    this->smooth = smooth;

    if (smooth)
        BuildVerticesSmooth();
    else
        BuildVerticesFlat();
}

void GlobeRenderer::Sphere::SetRadius(float radius)
{
    this->radius = radius;
    UpdateRadius();
}

void GlobeRenderer::Sphere::SetSectorCount(int sectors)
{
    Set(radius, sectors, stackCount, smooth);
}

void GlobeRenderer::Sphere::SetStackCount(int stacks)
{
    Set(radius, sectorCount, stacks, smooth);
}

void GlobeRenderer::Sphere::SetSmooth(bool smooth)
{
    if (this->smooth == smooth)
        return;

    this->smooth = smooth;
    if (smooth)
        BuildVerticesSmooth();
    else
        BuildVerticesFlat();
}

///////////////////////////////////////////////////////////////////////////////
// print itself
///////////////////////////////////////////////////////////////////////////////
std::string GlobeRenderer::Sphere::ToString() const
{
    std::stringstream ss;

    ss << "===== Sphere =====\n"
       << "        Radius: " << radius << "\n"
       << "  Sector Count: " << sectorCount << "\n"
       << "   Stack Count: " << stackCount << "\n"
       << "Smooth Shading: " << (smooth ? "true" : "false") << "\n"
       << "Triangle Count: " << GetTriangleCount() << "\n"
       << "   Index Count: " << GetIndexCount() << "\n"
       << "  Vertex Count: " << GetVertexCount() << "\n"
       << "  Normal Count: " << GetNormalCount() << "\n"
       << "TexCoord Count: " << GetTexCoordCount();

    return ss.str();
}

///////////////////////////////////////////////////////////////////////////////
// update vertex positions only
///////////////////////////////////////////////////////////////////////////////
void GlobeRenderer::Sphere::UpdateRadius()
{
    float scale = sqrtf(radius * radius / (vertices[0] * vertices[0] + vertices[1] * vertices[1] + vertices[2] * vertices[2]));

    std::size_t i, j;
    std::size_t count = vertices.size();
    for (i = 0, j = 0; i < count; i += 3, j += 8)
    {
        vertices[i] *= scale;
        vertices[i + 1] *= scale;
        vertices[i + 2] *= scale;

        // for interleaved array
        interleavedVertices[j] *= scale;
        interleavedVertices[j + 1] *= scale;
        interleavedVertices[j + 2] *= scale;
    }
}

///////////////////////////////////////////////////////////////////////////////
// dealloc vectors
///////////////////////////////////////////////////////////////////////////////
void GlobeRenderer::Sphere::ClearArrays()
{
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);
    std::vector<unsigned int>().swap(indices);
    std::vector<unsigned int>().swap(lineIndices);
}

///////////////////////////////////////////////////////////////////////////////
// build vertices of sphere with smooth shading using parametric equation
// x = r * cos(u) * cos(v)
// y = r * cos(u) * sin(v)
// z = r * sin(u)
// where u: stack(latitude) angle (-90 <= u <= 90)
//       v: sector(longitude) angle (0 <= v <= 360)
///////////////////////////////////////////////////////////////////////////////
void GlobeRenderer::Sphere::BuildVerticesSmooth()
{
    const float PI = acos(-1);

    // clear memory of prev arrays
    ClearArrays();

    float x, y, z, xy;                           // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius; // normal
    float s, t;                                  // texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);      // r * cos(u)
        z = radius * sinf(stackAngle);       // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep; // starting from 0 to 2pi

            // vertex position
            x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
            AddVertex(x, y, z);

            // normalized vertex normal
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            AddNormal(nx, ny, nz);

            // vertex tex coord between [0, 1]
            s = (float) j / sectorCount;
            t = (float) i / stackCount;
            AddTexCoord(s, t);
        }
    }

    // indices
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    unsigned int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1); // beginning of current stack
        k2 = k1 + sectorCount + 1;  // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding 1st and last stacks
            if (i != 0)
            {
                AddIndices(k1, k2, k1 + 1); // k1---k2---k1+1
            }

            if (i != (stackCount - 1))
            {
                AddIndices(k1 + 1, k2, k2 + 1); // k1+1---k2---k2+1
            }

            // vertical lines for all stacks
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if (i != 0) // horizontal lines except 1st stack
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

    // generate interleaved vertex array as well
    BuildInterleavedVertices();
}

///////////////////////////////////////////////////////////////////////////////
// generate vertices with flat shading
// each triangle is independent (no shared vertices)
///////////////////////////////////////////////////////////////////////////////
void GlobeRenderer::Sphere::BuildVerticesFlat()
{
    const float PI = acos(-1);

    // tmp vertex definition (x,y,z,s,t)
    struct Vertex
    {
        float x, y, z, s, t;
    };
    std::vector<Vertex> tmpVertices;

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    // compute all vertices first, each vertex contains (x,y,z,s,t) except normal
    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;  // starting from pi/2 to -pi/2
        float xy = radius * cosf(stackAngle); // r * cos(u)
        float z = radius * sinf(stackAngle);  // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep; // starting from 0 to 2pi

            Vertex vertex;
            vertex.x = xy * cosf(sectorAngle);  // x = r * cos(u) * cos(v)
            vertex.y = xy * sinf(sectorAngle);  // y = r * cos(u) * sin(v)
            vertex.z = z;                       // z = r * sin(u)
            vertex.s = (float) j / sectorCount; // s
            vertex.t = (float) i / stackCount;  // t
            tmpVertices.push_back(vertex);
        }
    }

    // clear memory of prev arrays
    ClearArrays();

    Vertex v1, v2, v3, v4; // 4 vertex positions and tex coords
    std::vector<float> n;  // 1 face normal

    int i, j, k, vi1, vi2;
    int index = 0; // index for vertex
    for (i = 0; i < stackCount; ++i)
    {
        vi1 = i * (sectorCount + 1); // index of tmpVertices
        vi2 = (i + 1) * (sectorCount + 1);

        for (j = 0; j < sectorCount; ++j, ++vi1, ++vi2)
        {
            // Get 4 vertices per sector
            //  v1--v3
            //  |    |
            //  v2--v4
            v1 = tmpVertices[vi1];
            v2 = tmpVertices[vi2];
            v3 = tmpVertices[vi1 + 1];
            v4 = tmpVertices[vi2 + 1];

            // if 1st stack and last stack, store only 1 triangle per sector
            // otherwise, store 2 triangles (quad) per sector
            if (i == 0) // a triangle for first stack ==========================
            {
                // put a triangle
                AddVertex(v1.x, v1.y, v1.z);
                AddVertex(v2.x, v2.y, v2.z);
                AddVertex(v4.x, v4.y, v4.z);

                // put tex coords of triangle
                AddTexCoord(v1.s, v1.t);
                AddTexCoord(v2.s, v2.t);
                AddTexCoord(v4.s, v4.t);

                // put normal
                n = ComputeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v4.x, v4.y, v4.z);
                for (k = 0; k < 3; ++k) // same normals for 3 vertices
                {
                    AddNormal(n[0], n[1], n[2]);
                }

                // put indices of 1 triangle
                AddIndices(index, index + 1, index + 2);

                // indices for line (first stack requires only vertical line)
                lineIndices.push_back(index);
                lineIndices.push_back(index + 1);

                index += 3; // for next
            }
            else if (i == (stackCount - 1)) // a triangle for last stack =========
            {
                // put a triangle
                AddVertex(v1.x, v1.y, v1.z);
                AddVertex(v2.x, v2.y, v2.z);
                AddVertex(v3.x, v3.y, v3.z);

                // put tex coords of triangle
                AddTexCoord(v1.s, v1.t);
                AddTexCoord(v2.s, v2.t);
                AddTexCoord(v3.s, v3.t);

                // put normal
                n = ComputeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
                for (k = 0; k < 3; ++k) // same normals for 3 vertices
                {
                    AddNormal(n[0], n[1], n[2]);
                }

                // put indices of 1 triangle
                AddIndices(index, index + 1, index + 2);

                // indices for lines (last stack requires both vert/hori lines)
                lineIndices.push_back(index);
                lineIndices.push_back(index + 1);
                lineIndices.push_back(index);
                lineIndices.push_back(index + 2);

                index += 3; // for next
            }
            else // 2 triangles for others ====================================
            {
                // put quad vertices: v1-v2-v3-v4
                AddVertex(v1.x, v1.y, v1.z);
                AddVertex(v2.x, v2.y, v2.z);
                AddVertex(v3.x, v3.y, v3.z);
                AddVertex(v4.x, v4.y, v4.z);

                // put tex coords of quad
                AddTexCoord(v1.s, v1.t);
                AddTexCoord(v2.s, v2.t);
                AddTexCoord(v3.s, v3.t);
                AddTexCoord(v4.s, v4.t);

                // put normal
                n = ComputeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
                for (k = 0; k < 4; ++k) // same normals for 4 vertices
                {
                    AddNormal(n[0], n[1], n[2]);
                }

                // put indices of quad (2 triangles)
                AddIndices(index, index + 1, index + 2);
                AddIndices(index + 2, index + 1, index + 3);

                // indices for lines
                lineIndices.push_back(index);
                lineIndices.push_back(index + 1);
                lineIndices.push_back(index);
                lineIndices.push_back(index + 2);

                index += 4; // for next
            }
        }
    }

    // generate interleaved vertex array as well
    BuildInterleavedVertices();
}

///////////////////////////////////////////////////////////////////////////////
// generate interleaved vertices: V/N/T
// stride must be 32 bytes
///////////////////////////////////////////////////////////////////////////////
void GlobeRenderer::Sphere::BuildInterleavedVertices()
{
    std::vector<float>().swap(interleavedVertices);

    std::size_t i, j;
    std::size_t count = vertices.size();
    for (i = 0, j = 0; i < count; i += 3, j += 2)
    {
        interleavedVertices.push_back(vertices[i]);
        interleavedVertices.push_back(vertices[i + 1]);
        interleavedVertices.push_back(vertices[i + 2]);

        interleavedVertices.push_back(normals[i]);
        interleavedVertices.push_back(normals[i + 1]);
        interleavedVertices.push_back(normals[i + 2]);

        interleavedVertices.push_back(texCoords[j]);
        interleavedVertices.push_back(texCoords[j + 1]);
    }
}

///////////////////////////////////////////////////////////////////////////////
// add single vertex to array
///////////////////////////////////////////////////////////////////////////////
void GlobeRenderer::Sphere::AddVertex(float x, float y, float z)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}

///////////////////////////////////////////////////////////////////////////////
// add single normal to array
///////////////////////////////////////////////////////////////////////////////
void GlobeRenderer::Sphere::AddNormal(float nx, float ny, float nz)
{
    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);
}

///////////////////////////////////////////////////////////////////////////////
// add single texture coord to array
///////////////////////////////////////////////////////////////////////////////
void GlobeRenderer::Sphere::AddTexCoord(float s, float t)
{
    texCoords.push_back(s);
    texCoords.push_back(t);
}

///////////////////////////////////////////////////////////////////////////////
// add 3 indices to array
///////////////////////////////////////////////////////////////////////////////
void GlobeRenderer::Sphere::AddIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}

///////////////////////////////////////////////////////////////////////////////
// return face normal of a triangle v1-v2-v3
// if a triangle has no surface (normal length = 0), then return a zero vector
///////////////////////////////////////////////////////////////////////////////
std::vector<float> GlobeRenderer::Sphere::ComputeFaceNormal(float x1, float y1, float z1, // v1
                                                            float x2, float y2, float z2, // v2
                                                            float x3, float y3, float z3) // v3
{
    const float EPSILON = 0.000001f;

    std::vector<float> normal(3, 0.0f); // default return value (0,0,0)
    float nx, ny, nz;

    // find 2 edge vectors: v1-v2, v1-v3
    float ex1 = x2 - x1;
    float ey1 = y2 - y1;
    float ez1 = z2 - z1;
    float ex2 = x3 - x1;
    float ey2 = y3 - y1;
    float ez2 = z3 - z1;

    // cross product: e1 x e2
    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;

    // normalize only if the length is > 0
    float length = sqrtf(nx * nx + ny * ny + nz * nz);
    if (length > EPSILON)
    {
        // normalize
        float lengthInv = 1.0f / length;
        normal[0] = nx * lengthInv;
        normal[1] = ny * lengthInv;
        normal[2] = nz * lengthInv;
    }

    return normal;
}
