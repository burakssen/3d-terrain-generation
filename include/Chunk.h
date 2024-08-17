#pragma once

#include <vector>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <external/stb_perlin.h>

class Chunk
{
public:
    Chunk(Vector3 index, Vector2 size);
    ~Chunk();

    void Generate();
    void Draw();
    void DrawWireframe();

    Vector3 GetIndex() const { return m_index; }

private:
    Vector3 m_index;
    Vector2 m_size;
    Mesh m_mesh;
    Material m_material;
};