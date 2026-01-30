#include "validators.h"
#include "exercises.h"
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

// Exercise 1: Field separator - verify -F option correctly parses delimited fields
int validate_awk_field_separator(Exercise *ex) {
    if (!ex || !ex->last_command_output)
        return 0;

    char *output = ex->last_command_output;
    return check_first_line(output, "JohnEngineer");
}

// Exercise 2: File program - verify -f option reads awk program from file
int validate_awk_file(Exercise *ex) {
    return check_first_line(ex->last_command_output, "Apple12");
}

// Exercise 3: Sandbox mode - verify -s option executes in sandbox
int validate_awk_sandbox(Exercise *ex)
{
    if (!ex || !ex->last_command_output)
        return 0;

    char *output = ex->last_command_output;
    return check_first_line(output, "JohnEngineer");
}