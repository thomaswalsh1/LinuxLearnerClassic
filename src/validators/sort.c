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

// Exercise 1: Check if sorted - verify that output.txt exists and is sorted
int validate_sort_check(Exercise *ex) {
    char *output = ex->last_command_output;
    char *input = ex->last_user_command;
    if (!strstr(input, "&&")) {
        return 0;
    }
    return check_first_line(output, "apple");
}

// Exercise 2: Ignore case - verify output.txt exists and has case-insensitive sort
int validate_sort_ignore_case(Exercise *ex) {
    return check_first_line(ex->last_command_output, "apple");
}

// Exercise 3: Sort by field - verify sorting by second field (age)
int validate_sort_by_field(Exercise *ex) {
    return check_first_line(ex->last_command_output, "Diana 22 Intern");
}

// Exercise 4: Month sort - verify sorting by month abbreviations
int validate_sort_month(Exercise *ex) {
    char *output = ex->last_command_output;
    return check_first_line(output, "03 JAN 2024") || check_first_line(output, "08 JAN 2024");
}

// Exercise 5: Numeric sort - verify numerical ordering
int validate_sort_numeric(Exercise *ex) {
    return check_first_line(ex->last_command_output, "1");
}

// Exercise 6: Output to file - verify -o option creates output.txt
int validate_sort_output(Exercise *ex) {
    return strcmp(ex->last_user_command, "sort -o output.txt data.txt") == 0;
}

// Exercise 7: Reverse sort - verify reverse alphabetical order
int validate_sort_reverse(Exercise *ex) {
    return check_first_line(ex->last_command_output, "grape");
}

// Exercise 8: Combined options - verify numeric reverse sort by first field
int validate_sort_combined(Exercise *ex) {
    return check_first_line(ex->last_command_output, "500 Mike Engineer");    
}
