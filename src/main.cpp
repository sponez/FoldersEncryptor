#include "application/Application.hpp"
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow) {
    return main(__argc, __argv);
}
#endif

int main(int, char**) {
    SetConsoleOutputCP(CP_UTF8);
    _setmode(_fileno(stdout), _O_U8TEXT);

    fe::Application::start();

    return 0;
}
