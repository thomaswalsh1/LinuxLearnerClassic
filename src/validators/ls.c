#include <ctype.h>
#include "validators.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// Exercise 1: Show all files (-a)
int validate_ls_show_all(Exercise *ex) {
	// Should show .hiddenfile, file1, file2, . and ..
	const char *out = ex->last_command_output;
	return strstr(out, ".hiddenfile") && strstr(out, "file1") && strstr(out, "file2") && strstr(out, ".") && strstr(out, "..");
}

// Exercise 2: Show directory metadata (-d)
int validate_ls_show_directory(Exercise *ex) {
	// Should show a single line starting with 'd' (directory) and the directory name
	const char *out = ex->last_command_output;
	return (strchr(out, 'd') && strchr(out, '.'));
}

// Exercise 3: Classify files (-F)
int validate_ls_classify(Exercise *ex) {
	// Should show dir1/ and file1, file2 (no /)
	const char *out = ex->last_command_output;
	return strstr(out, "dir1/") && strstr(out, "file1") && strstr(out, "file2");
}

// Exercise 4: Show inodes (-i)
int validate_ls_show_inodes(Exercise *ex) {
	// Should show numbers before file names
	const char *out = ex->last_command_output;
	// Look for a number followed by a space and file1
	return (strstr(out, " file1") && isdigit((unsigned char)out[0]));
}

// Exercise 5: Long listing (-l)
int validate_ls_long_listing(Exercise *ex) {
	// Should show lines starting with -rw or similar, and file1/file2
	const char *out = ex->last_command_output;
	return (strstr(out, "file1") && strstr(out, "file2") && (strstr(out, "-rw") || strstr(out, "-r-")));
}

// Exercise 6: Recursive listing (-R)
int validate_ls_recursive(Exercise *ex) {
	// Should show ex06:, dir1:, file1, dir1/file2
	const char *out = ex->last_command_output;
	return strstr(out, "dir1:") && strstr(out, "file1") && strstr(out, "file2");
}
#include "validators.h"
#include "exercises.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

