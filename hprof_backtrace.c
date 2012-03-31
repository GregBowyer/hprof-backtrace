#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>

#include "hprof_backtrace.h"
#include "swab.h"

void die(int check, char* message) {
    if (check) {
        printf(message);
        exit(1);
    }
}

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: %s HPROF_FILE\n", argv[0]);
        exit(1);
    }

    FILE* hprof_fp = fopen(argv[1], "r");
    die(hprof_fp == NULL, "File error");

    hprof_header* header;
    int err = fread(header, sizeof(hprof_header), 1, hprof_fp);
    printf("%d\n", err);

    printf("Header: version=%s identsize=%ju \n", 
            header->version,
            (uintmax_t) be32_to_native(header->ident_size));

    fclose(hprof_fp);
    return 0;
}
