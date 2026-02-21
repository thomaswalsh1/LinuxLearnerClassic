#include "validators.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

// Helper: compare file contents
int files_match(const char *f1, const char *f2) {
    FILE *a = fopen(f1, "r"), *b = fopen(f2, "r");
    if (!a || !b) return 0;
    int valid = 1;
    char buf1[256], buf2[256];
    while (fgets(buf1, sizeof(buf1), a)) {
        if (!fgets(buf2, sizeof(buf2), b) || strcmp(buf1, buf2) != 0) {
            valid = 0;
            break;
        }
    }
    fclose(a); fclose(b);
    return valid;
}

// Helper: check directory contents recursively
int dir_files_match(const char *src, const char *dst) {
    DIR *d = opendir(src);
    if (!d) return 0;
    struct dirent *entry;
    int valid = 1;
    while ((entry = readdir(d))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        char src_path[256], dst_path[256];
        snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
        snprintf(dst_path, sizeof(dst_path), "%s/%s", dst, entry->d_name);
        struct stat st;
        stat(src_path, &st);
        if (S_ISDIR(st.st_mode)) {
            valid &= dir_files_match(src_path, dst_path);
        } else {
            valid &= files_match(src_path, dst_path);
        }
    }
    closedir(d);
    return valid;
}

// rsync -a: archive mode (files and dirs match)
int validate_rsync_a(Exercise *ex) {
    char src[256], dst[256];
    snprintf(src, sizeof(src), "%s/source", ex->sandbox_dir);
    snprintf(dst, sizeof(dst), "%s/dest", ex->sandbox_dir);
    return dir_files_match(src, dst);
}

// rsync -D: device/special files (not implemented, just check file exists)
int validate_rsync_D(Exercise *ex) {
    char dst[256];
    snprintf(dst, sizeof(dst), "%s/dest/file.txt", ex->sandbox_dir);
    return file_exists(dst);
}

// rsync -g: group (just check file copied)
int validate_rsync_g(Exercise *ex) {
    char dst[256];
    snprintf(dst, sizeof(dst), "%s/dest/groupfile.txt", ex->sandbox_dir);
    return file_exists(dst);
}

// rsync -h: human-readable (output only, check file copied)
int validate_rsync_h(Exercise *ex) {
    char dst[256];
    snprintf(dst, sizeof(dst), "%s/dest/largefile.txt", ex->sandbox_dir);
    return file_exists(dst);
}

// rsync -l: links
// warning, hardcoded to the last command
int validate_rsync_l(Exercise *ex) {
    char dst[256];
    snprintf(dst, sizeof(dst), "%s/dest/symlink", ex->sandbox_dir);
    return strstr(ex->last_user_command, "-l");
}


// rsync -o: owner (just check file copied)
int validate_rsync_o(Exercise *ex) {
    char dst[256];
    snprintf(dst, sizeof(dst), "%s/dest/ownerfile.txt", ex->sandbox_dir);
    return file_exists(dst);
}

// rsync -p: perms (just check file copied)
int validate_rsync_p(Exercise *ex) {
    char dst[256];
    snprintf(dst, sizeof(dst), "%s/dest/permfile.txt", ex->sandbox_dir);
    return file_exists(dst);
}

// rsync --progress: output only, check file copied
int validate_rsync_progress(Exercise *ex) {
    char dst[256];
    snprintf(dst, sizeof(dst), "%s/dest/progressfile.txt", ex->sandbox_dir);
    return file_exists(dst);
}

// rsync -r: recursive (check files and subdirs copied)
int validate_rsync_r(Exercise *ex) {
    char src[256], dst[256];
    snprintf(src, sizeof(src), "%s/source", ex->sandbox_dir);
    snprintf(dst, sizeof(dst), "%s/dest", ex->sandbox_dir);
    return dir_files_match(src, dst);
}

// rsync --stats: output only, check file copied
int validate_rsync_stats(Exercise *ex) {
    char dst[256];
    snprintf(dst, sizeof(dst), "%s/dest/statsfile.txt", ex->sandbox_dir);
    return file_exists(dst);
}

// rsync -t: times (just check file copied)
int validate_rsync_t(Exercise *ex) {
    char dst[256];
    snprintf(dst, sizeof(dst), "%s/dest/timefile.txt", ex->sandbox_dir);
    return file_exists(dst);
}

// rsync -v: verbose (output only, check file copied)
int validate_rsync_v(Exercise *ex) {
    char dst[256];
    snprintf(dst, sizeof(dst), "%s/dest/verbosefile.txt", ex->sandbox_dir);
    return file_exists(dst);
}
