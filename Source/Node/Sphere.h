///////////////////////////////////////////////////////////////////////////////
// Sphere.h
// ========
// Sphere for OpenGL with (radius, sectors, stacks)
// The min number of sectors is 3 and The min number of stacks are 2.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2017-11-01
// UPDATED: 2018-12-13
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QOpenGLExtraFunctions>
#include <string>
#include <vector>

namespace GlobeRenderer
{

    class Sphere : protected QOpenGLExtraFunctions
    {
      public:
        Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18, bool smooth = true);

        // Getters/setters
        float GetRadius() const { return radius; }
        int GetSectorCount() const { return sectorCount; }
        int GetStackCount() const { return stackCount; }
        void set(float radius, int sectorCount, int stackCount, bool smooth = true);
        void setRadius(float radius);
        void setSectorCount(int sectorCount);
        void setStackCount(int stackCount);
        void setSmooth(bool smooth);

        // for vertex data
        unsigned int GetVertexCount() const { return (unsigned int) vertices.size() / 3; }
        unsigned int GetNormalCount() const { return (unsigned int) normals.size() / 3; }
        unsigned int GetTexCoordCount() const { return (unsigned int) texCoords.size() / 2; }
        unsigned int GetIndexCount() const { return (unsigned int) indices.size(); }
        unsigned int GetLineIndexCount() const { return (unsigned int) lineIndices.size(); }
        unsigned int GetTriangleCount() const { return GetIndexCount() / 3; }
        unsigned int GetVertexSize() const { return (unsigned int) vertices.size() * sizeof(float); }
        unsigned int GetNormalSize() const { return (unsigned int) normals.size() * sizeof(float); }
        unsigned int GetTexCoordSize() const { return (unsigned int) texCoords.size() * sizeof(float); }
        unsigned int GetIndexSize() const { return (unsigned int) indices.size() * sizeof(unsigned int); }
        unsigned int GetLineIndexSize() const { return (unsigned int) lineIndices.size() * sizeof(unsigned int); }
        const float* GetVertices() const { return vertices.data(); }
        const float* GetNormals() const { return normals.data(); }
        const float* GetTexCoords() const { return texCoords.data(); }
        const unsigned int* GetIndices() const { return indices.data(); }
        const unsigned int* GetLineIndices() const { return lineIndices.data(); }

        // for interleaved vertices: V/N/T
        unsigned int GetInterleavedVertexCount() const { return GetVertexCount(); }                                         // # of vertices
        unsigned int GetInterleavedVertexSize() const { return (unsigned int) interleavedVertices.size() * sizeof(float); } // # of bytes
        int GetInterleavedStride() const { return interleavedStride; }                                                      // should be 32 bytes
        const float* GetInterleavedVertices() const { return interleavedVertices.data(); }

        // OpenGL
        void CreateOpenGLStuff();
        void Render();

        std::string ToString() const;

      private:
        // member functions
        void UpdateRadius();
        void BuildVerticesSmooth();
        void BuildVerticesFlat();
        void BuildInterleavedVertices();
        void ClearArrays();
        void AddVertex(float x, float y, float z);
        void AddNormal(float x, float y, float z);
        void AddTexCoord(float s, float t);
        void AddIndices(unsigned int i1, unsigned int i2, unsigned int i3);
        std::vector<float> ComputeFaceNormal(float x1, float y1, float z1,
                                             float x2, float y2, float z2,
                                             float x3, float y3, float z3);

        float radius;
        int sectorCount; // longitude, # of slices
        int stackCount;  // latitude, # of stacks
        bool smooth;
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> texCoords;
        std::vector<unsigned int> indices;
        std::vector<unsigned int> lineIndices;

        // Interleaved
        std::vector<float> interleavedVertices;
        int interleavedStride; // # of bytes to hop to the next vertex (should be 32 bytes)

        // OpenGL handles
        GLuint vao{0};
        GLuint vbo[3];
        GLuint ebo{0};
    };
}