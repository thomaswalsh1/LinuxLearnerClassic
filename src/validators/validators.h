#ifndef VALIDATORS_H
#define VALIDATORS_H

#include "exercises.h"

// Generic reusable validators
int file_exists(const char *path);
int file_contains(const char *path, const char *needle);
int check_first_line(const char *output, const char *expected_line);

// Grep validators
int validate_grep_basic(Exercise *ex);
int validate_grep_count(Exercise *ex);
int validate_grep_dir_skip(Exercise *ex);
int validate_grep_extended(Exercise *ex);
int validate_grep_ignore_case(Exercise *ex);
int validate_grep_recursive(Exercise *ex);
int validate_grep_invert(Exercise *ex);
int validate_grep_combo(Exercise *ex);

// Cut validators
int validate_cut_characters(Exercise *ex);
int validate_cut_bytes(Exercise *ex);
int validate_cut_delimiter(Exercise *ex);
int validate_cut_fields(Exercise *ex);
int validate_cut_only_delimited(Exercise *ex);

// Sort validators
int validate_sort_check(void);
int validate_sort_ignore_case(void);
int validate_sort_by_field(void);
int validate_sort_month(void);
int validate_sort_numeric(void);
int validate_sort_output(void);
int validate_sort_reverse(void);
int validate_sort_combined(void);

// Wc validators
int validate_wc_bytes(void);
int validate_wc_lines(void);
int validate_wc_words(void);
int validate_wc_chars(void);
int validate_wc_max_line_length(void);

// Cat validators
int validate_cat_show_all(Exercise *ex);
int validate_cat_show_ends(Exercise *ex);
int validate_cat_number(Exercise *ex);
int validate_cat_squeeze_blank(Exercise *ex);
int validate_cat_show_tabs(Exercise *ex);
int validate_cat_show_nonprinting(Exercise *ex);

// Sed validators
int validate_sed_expression(void);
int validate_sed_file(void);
int validate_sed_extended_regex(void);

// Awk validators
int validate_awk_field_separator(Exercise *ex);
int validate_awk_file(Exercise *ex);
int validate_awk_sandbox(Exercise *ex);

#endif
