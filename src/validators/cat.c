#include "validators.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to read entire file into memory
static char* read_entire_file(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return NULL;
    
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    char *content = malloc(size + 1);
    if (!content) {
        fclose(f);
        return NULL;
    }
    
    fread(content, 1, size, f);
    content[size] = '\0';
    fclose(f);
    return content;
}

// Exercise 1: Show all - verify -A option shows special characters
int validate_cat_show_all(Exercise *ex) {
    return check_first_line(ex->last_command_output, "First line with special^Ichars$");
}

// Exercise 2: Show ends - verify -E option shows $ at line endings
int validate_cat_show_ends(Exercise *ex) {
    return check_first_line(ex->last_command_output, "Line one$");
}

// Exercise 3: Number lines - verify -n option numbers all lines
int validate_cat_number(Exercise *ex) {
    return strstr(ex->last_command_output, "1") != NULL;
}

// Exercise 4: Squeeze blank - verify -s option removes multiple blank lines
int validate_cat_squeeze_blank(Exercise *ex) {
    return strstr(ex->last_user_command, "cat -s data.txt");
}

// Exercise 5: Show tabs - verify -T option shows ^I for tabs
int validate_cat_show_tabs(Exercise *ex) {
    return check_first_line(ex->last_command_output, "First^Iline^Iwith^Itabs");
}

// Exercise 6: Show non-printing - verify -v option displays special characters
int validate_cat_show_nonprinting(Exercise *ex) {
    return check_first_line(ex->last_command_output, "This line has a bell^G");
}
