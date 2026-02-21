#include "validators.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to parse wc output
// wc output format: "  <count> <filename>" or just "<count>"
static int parse_wc_count(const char *line) {
    int count = 0;
    sscanf(line, "%d", &count);
    return count;
}

// Helper function to read first line of output file
static char* read_first_line(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return NULL;
    
    static char line[256];
    if (fgets(line, sizeof(line), f)) {
        fclose(f);
        return line;
    }
    fclose(f);
    return NULL;
}

// Exercise 1: Byte count - verify -c option output
int validate_wc_bytes(Exercise *ex) {
    return check_first_line(ex->last_command_output, "115 data.txt");
}

// Exercise 2: Line count - verify -l option output
int validate_wc_lines(Exercise *ex) {
    return check_first_line(ex->last_command_output, "5 data.txt");
}

// Exercise 3: Word count - verify -w option output
int validate_wc_words(Exercise *ex) {
    return check_first_line(ex->last_command_output, "14 data.txt");
}

// Exercise 4: Character count - verify -m option output
int validate_wc_chars(Exercise *ex) {
    return check_first_line(ex->last_command_output, "54 data.txt");
}

// Exercise 5: Max line length - verify -L option output
int validate_wc_max_line_length(Exercise *ex) {
    return check_first_line(ex->last_command_output, "93 data.txt");
}
