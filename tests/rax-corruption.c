#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char *program_name = NULL;

void set_program_name (const char *argv0) {
    const char *slash;
    const char *base;

    if (argv0 == NULL) {
        abort();
    }

    slash = strrchr(argv0, '/');
    base = (slash != NULL ? slash + 1 : argv0);

    if (base - argv0 >= 7 && strncmp(base - 7, "/.libs/", 7) == 0) {
        argv0 = base;
        if (strncmp(base, "lt-", 3) == 0) {
            argv0 = base + 3;
        }
    }

    program_name = argv0;
    printf("%s\n", program_name);
}

int main(int argc, char **argv) {
    char *data = "./home/user/rax-corruption";
    set_program_name((const char *) data);
    return 0;
}
