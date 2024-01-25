#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define DEFAULT_PROJECT_NAME "UNKNOWN PROJECT"
#define DEFAULT_BUILD_DIR "build"
#define DEFAULT_COMPILER "gcc"
#define DEFAULT_EXE_NAME "a.exe"
#define DEFAULT_SOURCE "main.c"


#include<windows.h>
#include <stdio.h>

typedef struct LinkedList {
    void *data;
    struct LinkedList *next;
} LinkedList;

typedef struct Library {
    char *lib_name;
    char *lib_dir;
    char *include;
} Library;

typedef struct CBuild {
    TCHAR *pwd;
    //relative to pwd
    TCHAR *build_dir;
    // int nSource;
    TCHAR *source;
    TCHAR *project_name;
    TCHAR *exe_name;
    TCHAR *compiler;
//    LinkedList *libs;
//    LinkedList *includes;
    LinkedList *librarys;
    LinkedList *args;
} CBuild;

void init(CBuild *c);

void add_source(CBuild *c, char *arg);

void add_lib(CBuild *c, Library *arg);

void add_args(CBuild *c, char *arg);

void add_additional(CBuild *c, char *arg);

void set_build_dir(CBuild *c, char *arg);

void set_project_name(CBuild *c, char *arg);

void set_exe_name(CBuild *c, char *arg);

void free_up(CBuild *c);

void build_and_exit(CBuild *c);

char *generate_cmd(CBuild *c);


int build(CBuild *c);

void create_dir(TCHAR *dir);

void set_pwd(CBuild *c) {
    DWORD pathSize = GetCurrentDirectory(0, NULL);
    c->pwd = (TCHAR *) malloc((pathSize + 1) * sizeof(TCHAR)); // Add 1 for null-terminator
    DWORD x = GetCurrentDirectory(
            pathSize,
            c->pwd
    );
}

void init(CBuild *c) {
    c->pwd = NULL;
    c->build_dir = NULL;
    c->source = NULL;
    c->project_name = NULL;
    c->exe_name = NULL;
    c->compiler = NULL;
    c->librarys = NULL;
    c->args = NULL;
    set_pwd(c);
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

void set_project_name(CBuild *c, char *arg) {
    c->project_name = arg;
}

void run_command(TCHAR *command) {

    // Create process information
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // Set the size of the structures
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    si.dwFlags |= STARTF_USESTDHANDLES;
    BOOL cProg = CreateProcess(NULL,   // No module name (use command line)
                               command, // Command line
                               NULL,    // Process handle not inheritable
                               NULL,    // Thread handle not inheritable
                               TRUE,   // Set handle inheritance to FALSE
                               0,       // No creation flags
                               NULL,    // Use parent's environment block
                               NULL,    // Use parent's starting directory
                               &si,     // Pointer to STARTUPINFO structure
                               &pi);   // Pointer to PROCESS_INFORMATION structure
    if (!cProg) {

        fprintf(stderr, "\nCannot run cmd args: %lu", GetLastError());
        exit((int) GetLastError());
    }
    // Close the write end of the pipe - it is not needed by this process
    CloseHandle(pi.hThread);

    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles
    CloseHandle(pi.hProcess);
//    CloseHandle(pi.hThread);
//    CloseHandle(hReadPipe);
}

BOOL FolderExists(const TCHAR *folderPath) {
    DWORD attributes = GetFileAttributes(folderPath);
    return (attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY));
}

void create_dirs(CBuild *c) {
    // build
    if (FolderExists(c->build_dir) == TRUE) {
        printf("\nBuild dir exists: %s", c->build_dir);
    } else {
        CreateDirectory(c->build_dir, NULL);
    }
}

void fill_up(CBuild *c) {
    if (c->project_name == NULL) {
        printf("Project name not specified using: %s\n", DEFAULT_PROJECT_NAME);
        c->project_name = DEFAULT_PROJECT_NAME;
    }
    if (c->build_dir == NULL) {
        printf("Build dir not specified using: %s\n", DEFAULT_BUILD_DIR);
        set_build_dir(c, DEFAULT_BUILD_DIR);
    }
    if (c->source == NULL) {
        printf("Source not specified using: %s\n", DEFAULT_SOURCE);
        add_source(c, DEFAULT_SOURCE);
    }
    if (c->exe_name == NULL) {
        printf("Exe name not specified using: %s\n", DEFAULT_EXE_NAME);
        c->exe_name = "a.exe";
    }
    if (c->compiler == NULL) {
        printf("Compiler not specified using: %s\n", DEFAULT_COMPILER);
        c->compiler = DEFAULT_COMPILER;
    }
}

int build(CBuild *c) {
    fill_up(c);

    printf("Compiling Project\n");
    printf("Project Name: %s\n", c->project_name);
    printf("Compiler: %s\n", c->compiler);
    printf("Build Dir: %s\n", c->build_dir);
    printf("Source: %s\n", c->source);
    printf("exe_name: %s\n", c->exe_name);
    create_dirs(c);
    TCHAR *command = generate_cmd(c);
    run_command(command);
    return EXIT_SUCCESS;
}

void free_up(CBuild *c) {

}

void build_and_exit(CBuild *c) {
    build(c);
    free_up(c);
    exit(EXIT_SUCCESS);
}


char *formatString(const char *format, ...) {
    va_list args;
    va_start(args, format);

    // Determine the length of the formatted string
    size_t length = vsnprintf(NULL, 0, format, args);

    // Allocate memory for the formatted string
    char *result = (char *) malloc(length + 1);

    // Format the string and store it in the allocated memory
    vsnprintf(result, length + 1, format, args);

    va_end(args);

    return result;
}

char *generate_cmd(CBuild *c) {
    // <compiler> "<source>" -o "<build_dir>/<EXE_NAME>"

    return formatString("%s \"%s\" -o \"%s\\%s\"", c->compiler, c->source, c->build_dir, c->exe_name);
}

void set_exe_name(CBuild *c, char *arg) {
    c->exe_name = arg;
}

Library *get_lib_obj(char *lib_name, char *lib_dir, char *include) {
    Library *lib;
    lib->lib_name = lib_name;
    lib->lib_dir = lib_dir;
    lib->include = include;
    return lib;
}

void add_lib(CBuild *c, Library *arg) {
    LinkedList *list = (LinkedList *) malloc(sizeof(LinkedList));
    list->data = arg;
    list->next = c->librarys;
    c->librarys = list;
}

void add_args(CBuild *c, char *arg) {
    LinkedList *list = (LinkedList *) malloc(sizeof(LinkedList));
    list->data = arg;
    list->next = c->args;
    c->args = list;
}


#endif