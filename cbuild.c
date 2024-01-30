#include "cbuild.h"
#include <stdio.h>
#include <windows.h>

// WriteConsole(hConsole, c.pwd, lstrlenA(c.pwd), NULL, NULL);

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    CBuild c;
    init(&c);
    set_build_dir(&c, "build");
    set_project_name(&c, "TEST PROJECT");
    add_source(&c, "main.c");
    build(&c);
    free_up(&c);
    return EXIT_SUCCESS;
}