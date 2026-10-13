/*
 * FuturaOS YASM compatibility frontend.
 *
 * The original YASM 1.3.0 build requires re2c/gperf during its build.
 * Until those generators are native FuturaOS tools, this command provides
 * the common YASM x86 CLI on top of the native NASM implementation.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void yasm_version(void) {
    puts("yasm 1.3.0-futura (NASM-compatible frontend)");
    puts("FuturaOS native x86 assembler frontend");
}

static void yasm_help(void) {
    puts("yasm - FuturaOS NASM-compatible assembler frontend");
    puts("Usage: yasm [options] file");
    puts("Common options: -f format -o file -g dbgfmt -l listfile");
    puts("                 -I dir -D macro -U macro -p file");
    puts("Common YASM/NASM x86 command-line options are passed through.");
}

int main(int argc, char **argv) {
    if (argc > 1) {
        if (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-V")) {
            yasm_version(); return 0;
        }
        if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
            yasm_help(); return 0;
        }
    }

    char **nargv = calloc((size_t)argc + 1, sizeof(char *));
    if (!nargv) { fputs("yasm: out of memory\n", stderr); return 1; }

    nargv[0] = (char *)"nasm";
    for (int i = 1; i < argc; ++i) nargv[i] = argv[i];
    nargv[argc] = NULL;

    execv("/usr/bin/nasm", nargv);
    execvp("nasm", nargv);
    nargv[0] = (char *)"/bin/nasm";
    execv(nargv[0], nargv);

    perror("yasm: cannot execute nasm");
    free(nargv);
    return 127;
}
