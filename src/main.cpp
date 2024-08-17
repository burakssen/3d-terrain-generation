#include "App.h"

int main()
{
    App *app = App::GetInstance();
    app->Run();
    App::DestroyInstance();
    return EXIT_SUCCESS;
}