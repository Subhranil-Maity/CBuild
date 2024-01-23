//#include <string.h>

/*
 * LPCWSTR command = L"cmd /c echo Hello, World!";

    // Create process information
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // Set the size of the structures
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

    // Create the process
    if (CreateProcess(NULL,   // No module name (use command line)
                      command, // Command line
                      NULL,    // Process handle not inheritable
                      NULL,    // Thread handle not inheritable
                      FALSE,   // Set handle inheritance to FALSE
                      0,       // No creation flags
                      NULL,    // Use parent's environment block
                      NULL,    // Use parent's starting directory
                      &si,     // Pointer to STARTUPINFO structure
                      &pi)     // Pointer to PROCESS_INFORMATION structure
    ) {
        // Wait for the process to finish
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Close process and thread handles
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        // Handle error
        MessageBox(NULL, L"Failed to create process", L"Error", MB_OK | MB_ICONERROR);
    }
 */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include<windows.h>
typedef struct CBuild {
    TCHAR *pwd;
    //relative to pwd
    TCHAR *build_dir;
    // int nSource;
    TCHAR *source;
    TCHAR *project_name;
    TCHAR *exe_name;
    TCHAR *compiler;
} CBuild;

void init(CBuild *c);

void add_source(CBuild *c, char *arg);

void add_include(CBuild *c, char *arg);

void add_lib(CBuild *c, char *arg);

void add_flag(CBuild *c, char *arg);

void add_additional(CBuild *c, char *arg);

void set_build_dir(CBuild *c, char *arg);

void set_project_name(CBuild *c, char *arg);
void generate_cmd(CBuild *c, char *destination);

int build(CBuild *c);
void simple_print(TCHAR *c){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsole(
            hConsole,
            c,
            lstrlen(c),
            NULL,
            NULL);
}
void init(CBuild *c) {
    DWORD pathSize = GetCurrentDirectory(0, NULL);
    c->pwd = (TCHAR *) malloc((pathSize + 1) * sizeof(TCHAR)); // Add 1 for null-terminator
    DWORD x = GetCurrentDirectory(
            pathSize,
            c->pwd
    );
    c->build_dir = NULL;
    c->source = NULL;
    c->project_name = NULL;
    c->exe_name = NULL;
    c->compiler = NULL;
}

void set_build_dir(CBuild *c, char *arg) {
    int len = strlen(c->pwd) + strlen(arg) + 1;
    c->build_dir = (TCHAR *) malloc((len + 1) * sizeof(TCHAR));

    memset(c->build_dir, 0, len + 1);

    strcat(c->build_dir, c->pwd);
    strcat(c->build_dir, "\\");
    strcat(c->build_dir, arg);
}

void add_source(CBuild *c, char *arg) {
    int len = strlen(c->pwd) + strlen(arg) + 1;
    c->source = (TCHAR *) malloc((len + 1) * sizeof(TCHAR));

    memset(c->source, 0, len + 1);

    strcat(c->source, c->pwd);
    strcat(c->source, "\\");
    strcat(c->source, arg);
}
void set_project_name(CBuild *c, char *arg){
    c->project_name = arg;
}
int build(CBuild *c){
    if (c->project_name == NULL){
        c->project_name = "UNKNOWN PROJECT";
    }
    if (c->build_dir == NULL){
        set_build_dir(c, "build");
    }
    if (c->source == NULL) {
        add_source(c, "main.c");
    }
    if (c->exe_name == NULL){
        c->exe_name = "a.exe";
    }
    if (c->compiler == NULL){
        c->compiler = "gcc";
    }



//    simple_print();
    return EXIT_SUCCESS;
}

void generate_cmd(CBuild *c, char *destination) {
    // <compiler> <source> -o <build_dir>/<EXE_NAME>

    int len = lstrlen(c->compiler) + 1 +
            lstrlen(c->source) + 4 +
            lstrlen(c->build_dir) + 1 +
            lstrlen(c->exe_name);
//    char gen = "gcc " + (TCHAR*)c->source;
}

#endif