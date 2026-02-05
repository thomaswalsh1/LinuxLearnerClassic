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
int validate_sort_check(Exercise *ex);
int validate_sort_ignore_case(Exercise *ex);
int validate_sort_by_field(Exercise *ex);
int validate_sort_month(Exercise *ex);
int validate_sort_numeric(Exercise *ex);
int validate_sort_output(Exercise *ex);
int validate_sort_reverse(Exercise *ex);
int validate_sort_combined(Exercise *ex);

// Wc validators
int validate_wc_bytes(Exercise *ex);
int validate_wc_lines(Exercise *ex);
int validate_wc_words(Exercise *ex);
int validate_wc_chars(Exercise *ex);
int validate_wc_max_line_length(Exercise *ex);

// Cat validators
int validate_cat_show_all(Exercise *ex);
int validate_cat_show_ends(Exercise *ex);
int validate_cat_number(Exercise *ex);
int validate_cat_squeeze_blank(Exercise *ex);
int validate_cat_show_tabs(Exercise *ex);
int validate_cat_show_nonprinting(Exercise *ex);

// Sed validators
int validate_sed_expression(Exercise *ex);
int validate_sed_file(Exercise *ex);
int validate_sed_extended_regex(Exercise *ex);

// Awk validators
int validate_awk_field_separator(Exercise *ex);
int validate_awk_file(Exercise *ex);
int validate_awk_sandbox(Exercise *ex);

// ls validators
int validate_ls_show_all(Exercise *ex);
int validate_ls_show_directory(Exercise *ex);
int validate_ls_classify(Exercise *ex);
int validate_ls_show_inodes(Exercise *ex);
int validate_ls_long_listing(Exercise *ex);
int validate_ls_recursive(Exercise *ex);

// cp validators
int validate_cp_archive(Exercise *ex);
int validate_cp_force(Exercise *ex);
int validate_cp_interactive(Exercise *ex);
int validate_cp_no_clobber(Exercise *ex);
int validate_cp_recursive(Exercise *ex);
int validate_cp_update(Exercise *ex);
int validate_cp_verbose(Exercise *ex);

#endif
