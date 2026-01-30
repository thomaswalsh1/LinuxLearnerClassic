#ifndef VALIDATORS_H
#define VALIDATORS_H

#include "exercises.h"

// Generic reusable validators
int file_exists(const char *path);
int file_contains(const char *path, const char *needle);
int check_first_line(const char *output, const char *expected_line);

// Grep validators
int validate_grep_basic(void);
int validate_grep_count(void);
int validate_grep_dir_skip(void);
int validate_grep_extended(void);
int validate_grep_ignore_case(void);
int validate_grep_recursive(void);
int validate_grep_invert(void);
int validate_grep_combo(void);

// Cut validators
int validate_cut_characters(void);
int validate_cut_bytes(void);
int validate_cut_delimiter(void);
int validate_cut_fields(void);
int validate_cut_only_delimited(void);

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
int validate_cat_show_all(void);
int validate_cat_show_ends(void);
int validate_cat_number(void);
int validate_cat_squeeze_blank(void);
int validate_cat_show_tabs(void);
int validate_cat_show_nonprinting(void);

// Sed validators
int validate_sed_expression(void);
int validate_sed_file(void);
int validate_sed_extended_regex(void);

// Awk validators
int validate_awk_field_separator(Exercise *ex);
int validate_awk_file(Exercise *ex);
int validate_awk_sandbox(Exercise *ex);

#endif
