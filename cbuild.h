
typedef struct CBuild {
    void *pwd;
    //relative to pwd
    void *build_dir;
    // int nSource;
    void *source;
} CBuild;

void init(CBuild *c);
void add_source(CBuild *c, char *arg);
void add_include(CBuild *c, char *arg);
void add_lib(CBuild *c, char *arg);
void add_flag(CBuild *c, char *arg);
void add_additional(CBuild *c, char *arg);
void set_build_dir(CBuild *c, char *arg);


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include<windows.h>
#define MAX_LENGTH 100

char* get_abs_path(char *pwd, char *arg){
    int len = strlen(pwd) + strlen(arg) + 1;
    char *dest = (TCHAR*)malloc((len + 1) * sizeof(TCHAR));

    memset(dest, 0, len+1);

    strcat(dest, pwd);
    strcat(dest, "\\");
    strcat(dest, arg);
    return dest;
}


void init(CBuild *c){
    DWORD pathSize = GetCurrentDirectory(0, NULL);
    c->pwd = (TCHAR*)malloc((pathSize + 1) * sizeof(TCHAR)); // Add 1 for null-terminator

    // TCHAR path[pathSize];
    DWORD x = GetCurrentDirectory(
            MAX_LENGTH,
            c->pwd
        );
    // WriteConsole(hConsole, c->pwd, lstrlenA(c->pwd), NULL, NULL);
}
void set_build_dir(CBuild *c, char *arg){
    c->build_dir = get_abs_path(c->pwd, arg);
}
void add_source(CBuild *c, char *arg){
    // len
}
#endif