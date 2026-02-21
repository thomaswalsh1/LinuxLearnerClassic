
#include "validators.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// ...existing code...

int validate_grep_basic(Exercise *ex)
{
    return check_first_line(ex->last_command_output, "error: file not found");
}

int validate_grep_count(Exercise *ex)
{
    return check_first_line(ex->last_command_output, "2"); // assuming 2 error lines
}

int validate_grep_dir_skip(Exercise *ex)
{
    return check_first_line(ex->last_command_output, "logs/log1.txt:error: log1 failed");
}

int validate_grep_extended(Exercise *ex)
{
    return strstr(ex->last_command_output, "error") && strstr(ex->last_command_output, "warning");
}

int validate_grep_ignore_case(Exercise *ex)
{
    return strstr(ex->last_command_output, "Error") && strstr(ex->last_command_output, "error");
}

int validate_grep_recursive(Exercise *ex)
{

    return strstr(ex->last_command_output, "log1") && strstr(ex->last_command_output, "log2") && strstr(ex->last_command_output, "log3");
}

int validate_grep_invert(Exercise *ex)
{
    return !check_first_line(ex->last_command_output, "error");
}

int validate_grep_combo(Exercise *ex)
{
    char *output = ex->last_command_output;
    return !strstr(output, "Error") &&
        !strstr(output, "error") &&
        strstr(output, "info") &&
        strstr(output, "warning");
}