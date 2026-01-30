#include "validators.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int validate_cut_characters(Exercise *ex) {
    return check_first_line(ex->last_command_output, "12345");
}

int validate_cut_bytes(Exercise *ex) {
    return check_first_line(ex->last_command_output, "AB");
}

int validate_cut_delimiter(Exercise *ex) {
    return check_first_line(ex->last_command_output, "alice");
}

int validate_cut_fields(Exercise *ex) {
    return check_first_line(ex->last_command_output, "John	2023-01-15");
}

int validate_cut_only_delimited(Exercise *ex) {
    if (strstr(ex->last_command_output, "NODASHRECORD")) return 0;
    return check_first_line(ex->last_command_output, "one");
}
