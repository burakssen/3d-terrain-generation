#include <raylib.h>
#include <string>
#include <memory>
#include <future>
#include <vector>
#include <unordered_set>
#include "Chunk.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

// Helper function to generate chunk index
static Vector2 CalculateChunkIndex(const Vector2 &cameraPosition, int i, int j)
{
    int cameraChunkX = static_cast<int>(cameraPosition.x / 32);
    int cameraChunkY = static_cast<int>(cameraPosition.y / 32);
    return {(float)(cameraChunkX * 32 + i * 32), (float)(cameraChunkY * 32 + j * 32)};
}

// Helper function to get chunk key for uniqueness
static std::string GetChunkKey(const Vector3 &index)
{
    return std::to_string(static_cast<int>(index.x)) + "_" + std::to_string(static_cast<int>(index.y));
}

class App
{
public:
    App();
    ~App();

    static App *GetInstance();
    static void DestroyInstance();
    void Run();

    void Update();
    void Draw();

private:
    void GenerateChunks();

private:
    Vector2 m_size = {1280, 720};
    std::string m_title = "3D Terrain Generation";
    Camera3D m_camera = {0};
    std::vector<std::unique_ptr<Chunk>> m_chunks;
    std::mutex chunksMutex;
};

inline App *g_instance = nullptr;

static void UpdateDrawFrame()
{
    App &app = *App::GetInstance();
    app.Update();
    app.Draw();
}