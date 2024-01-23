#include "cbuild.h"
#include <stdio.h>
#include <windows.h>

// WriteConsole(hConsole, c.pwd, lstrlenA(c.pwd), NULL, NULL);
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    // printf("Hello");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CBuild c;
    init(&c);
    set_build_dir(&c, "build");
    add_source(&c, "main.c");
    // add_include(&c, "");
    // add_lib(&c, "");
    // add_flag(&c, "");
    // add_aditional(&c, "");
    build(&c);
    return EXIT_SUCCESS;
}