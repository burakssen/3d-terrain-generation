#include "App.h"

#include <iostream>

App::App()
{
    InitWindow(m_size.x, m_size.y, m_title.c_str());

    m_camera.position = {0.0f, 40.0f, 16.0f}; // Camera placed above and back from the terrain
    m_camera.target = {32.0f, 0.0f, 16.0f};   // Target the center of the terrain
    m_camera.up = {0.0f, 1.0f, 0.0f};         // Up direction
    m_camera.fovy = 60.0f;                    // Field of view
    m_camera.projection = CAMERA_PERSPECTIVE;
}

App::~App()
{
    CloseWindow();
}

App *App::GetInstance()
{
    if (g_instance == nullptr)
    {
        g_instance = new App();
    }
    return g_instance;
}

void App::DestroyInstance()
{
    if (g_instance != nullptr)
    {
        delete g_instance;
        g_instance = nullptr;
    }
}

void App::Run()
{
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }
#endif
}

void App::Update()
{
    Vector3 movement = Vector3Scale(Vector3{50.0f, 0.0f, 0.0f}, GetFrameTime());
    m_camera.position = Vector3Add(m_camera.position, movement);
    m_camera.target = Vector3Add(m_camera.target, movement);

    /*std::thread t([this]()
              { this->GenerateChunks(); });
t.detach();*/
    this->GenerateChunks();
}

void App::Draw()
{
    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode3D(m_camera);
    for (auto &chunk : m_chunks)
    {
        chunk->Draw();
    }
    EndMode3D();
    int fps = GetFPS();
    std::string fpsStr = "FPS: " + std::to_string(fps);
    DrawText(fpsStr.c_str(), 10, 10, 30, RED);
    EndDrawing();
}

void App::GenerateChunks()
{

    Vector2 cameraPosition = {m_camera.position.x, m_camera.position.z};
    int cameraChunkX = static_cast<int>(cameraPosition.x / 32);
    int cameraChunkY = static_cast<int>(cameraPosition.y / 32);

    // Use an unordered set to quickly check for existing chunks
    std::unordered_set<std::string> existingChunkKeys;
    for (const auto &chunk : m_chunks)
    {
        existingChunkKeys.insert(GetChunkKey(chunk->GetIndex()));
    }

    // Generate new chunks
    std::vector<std::unique_ptr<Chunk>> newChunks;
    for (int i = 0; i < 4; i++)
    {
        for (int j = -2; j < 3; j++)
        {
            Vector2 index = CalculateChunkIndex(cameraPosition, i, j);
            Vector3 chunkIndex = {index.x, index.y, 0};
            std::string chunkKey = GetChunkKey(chunkIndex);

            if (existingChunkKeys.find(chunkKey) == existingChunkKeys.end())
            {
                newChunks.push_back(std::make_unique<Chunk>(chunkIndex, Vector2{32, 32}));
            }
        }
    }

    // Add new chunks
    m_chunks.insert(m_chunks.end(), std::make_move_iterator(newChunks.begin()), std::make_move_iterator(newChunks.end()));

    // Remove far-away chunks
    m_chunks.erase(std::remove_if(m_chunks.begin(), m_chunks.end(), [&](const std::unique_ptr<Chunk> &c)
                                  {
        Vector2 chunkIndex = {c->GetIndex().x, c->GetIndex().y};
        int chunkX = static_cast<int>(chunkIndex.x / 32);
        int chunkY = static_cast<int>(chunkIndex.y / 32);

        return (cameraChunkX - chunkX) > 2 || (cameraChunkY - chunkY) > 2; }),
                   m_chunks.end());
}
