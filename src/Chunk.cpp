#include "Chunk.h"

#include <iostream>

Chunk::Chunk(Vector3 index, Vector2 size)
    : m_index(index), m_size(size)
{
    m_mesh = {0};
    m_material = LoadMaterialDefault();

    this->Generate();
}

Chunk::~Chunk()
{
    UnloadMaterial(m_material);
    UnloadMesh(m_mesh);
}

void Chunk::Generate()
{
    Image image = GenImagePerlinNoise(m_size.x + 1, m_size.y + 1, this->m_index.x, this->m_index.y, 1.0f);
    m_mesh = GenMeshHeightmap(image, Vector3{this->m_size.x, 30.0f, this->m_size.y});
    m_material.maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(image);
    UnloadImage(image);
}

void Chunk::Draw()
{
    rlPushMatrix();
    rlTranslatef(m_index.x, 0, m_index.y);
    rlScalef(1, 1, 1);

    // Enable wireframe mode to draw only the triangles
    rlEnableWireMode();
    DrawMesh(m_mesh, m_material, MatrixIdentity());
    rlDisableWireMode();

    rlPopMatrix();
}