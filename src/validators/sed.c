#include "validators.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exercise 1: Expression option - verify -e option output with multiple substitutions
int validate_sed_expression(Exercise *ex) {
    char *output = ex->last_command_output;
    return !strstr(output, "the") && !strstr(output, "The") && strstr(output, "THE");
}

// Exercise 2: File option - verify -f option reads script from file
int validate_sed_file(Exercise *ex) {
    char *output = ex->last_command_output;
    return !strstr(output, "apple") && !strstr(output, "banana") && strstr(output, "date");
}

// Exercise 3: Extended regex - verify -r option enables extended regex patterns
int validate_sed_extended_regex(Exercise *ex) {
    char *output = ex->last_command_output;
    // there's probably a better way to do this
    return strstr(output, "NUM") &&
        !(strstr(output, "0") ||
        strstr(output, "1") ||
        strstr(output, "2") ||
        strstr(output, "3") ||
        strstr(output, "4") ||
        strstr(output, "5") ||
        strstr(output, "6") ||
        strstr(output, "7") ||
        strstr(output, "8") ||
        strstr(output, "9")
    );
}
