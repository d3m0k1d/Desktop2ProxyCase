// main.c
#include <stdio.h>
#include <stdlib.h>
#include "parse_xml.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s scan.xml\n", argv[0]);
        return 1;
    }
    char *report = parse_open_ports(argv[1]);
    if (!report) {
        fprintf(stderr, "Ошибка парсинга XML\n");
        return 1;
    }
    printf("%s", report);
    free(report);
    return 0;
}
