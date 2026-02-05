#include "validators.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int check_first_line(const char *output, const char *expected_line) {
    // Find the first newline
    char *newline = strchr(output, '\n');

    // Calculate length of the first line
    size_t line_len = newline ? (size_t)(newline - output) : strlen(output);

    // Copy the first line into a temporary buffer
    char first_line[1024] = {0};
    if (line_len >= sizeof(first_line))
        line_len = sizeof(first_line) - 1;
    strncpy(first_line, output, line_len);
    first_line[line_len] = '\0';

    // Optional: trim whitespace at the ends
    char *start = first_line;
    while (*start && isspace((unsigned char)*start)) start++;
    char *end = first_line + strlen(first_line) - 1;
    while (end > start && isspace((unsigned char)*end)) *end-- = '\0';

    // Compare against the expected first line
    if (strcmp(start, expected_line) == 0)
        return 1;

    return 0;
}

char *read_entire_file(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *content = malloc(size + 1);
    if (!content)
    {
        fclose(f);
        return NULL;
    }
    fread(content, 1, size, f);
    content[size] = '\0';
    fclose(f);
    return content;
}
