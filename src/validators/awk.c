#include "validators.h"
#include "exercises.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exercise 1: Field separator - verify -F option correctly parses delimited fields
int validate_awk_field_separator(Exercise *ex) {
    if (!ex || !ex->last_command_output)
        return 0;

    char *output = ex->last_command_output;
    return check_first_line(output, "JohnEngineer");
}

// Exercise 2: File program - verify -f option reads awk program from file
int validate_awk_file(Exercise *ex) {
    if (!ex || !ex->last_command_output)
        return 0;

    return check_first_line(ex->last_command_output, "apple12");
}

// Exercise 3: Sandbox mode - verify -s option executes in sandbox
int validate_awk_sandbox(Exercise *ex)
{
    if (!ex || !ex->last_command_output)
        return 0;

    char *command = ex->last_user_command;
    if (strstr(command, "-s") == NULL) return 0; 
    char *output = ex->last_command_output;
    return check_first_line(output, "600");
}