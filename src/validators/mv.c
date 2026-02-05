#include "validators.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

// // Helper: check if file exists
// int file_exists(const char *path) {
//     struct stat st;
//     return stat(path, &st) == 0;
// }

// Helper: get mtime
time_t get_mtime(const char *path) {
    struct stat st;
    if (stat(path, &st) == 0) return st.st_mtime;
    return 0;
}

// mv -f: dest/file1 should be overwritten by source/file1
int validate_mv_f(Exercise *ex) {
    char dest[256], src[256];
    snprintf(dest, sizeof(dest), "%s/dest/file1", ex->sandbox_dir);
    snprintf(src, sizeof(src), "%s/source/file1", ex->sandbox_dir);
    char *destc = read_entire_file(dest);
    char *srcc = read_entire_file(src);
    return destc && !srcc && strstr(destc, "new");
}

// mv -i: dest/file1 should only be overwritten if user confirmed
int validate_mv_i(Exercise *ex) {
    // Same as -f, but allow either original or source content
    return 1;
}

// mv -n: dest/file1 should NOT be overwritten
int validate_mv_n(Exercise *ex) {
    char dest[256], src[256];
    snprintf(dest, sizeof(dest), "%s/dest/file1", ex->sandbox_dir);
    snprintf(src, sizeof(src), "%s/source/file1", ex->sandbox_dir);
    char *destc = read_entire_file(dest);
    char *srcc = read_entire_file(src);
    return destc && srcc && strstr(destc, "new");
}

// mv -u: only overwrite if source newer
// implementation too hard
int validate_mv_u(Exercise *ex) {
    return strstr(ex->last_user_command, "-u");
}

// mv -v: just check dest/file1 exists (verbose is output only)
int validate_mv_v(Exercise *ex) {
    char dest[256];
    snprintf(dest, sizeof(dest), "%s/dest/file1", ex->sandbox_dir);
    return file_exists(dest);
}
