#include "validators.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// Helper: read entire file into memory


// Exercise 1: Archive copy (-a)
int validate_cp_archive(Exercise *ex)
{
    // Check that dest/file1 and dest/file2 exist and contents match source
    char src1[256], src2[256], dst1[256], dst2[256];
    snprintf(src1, sizeof(src1), "%s/source/file1", ex->sandbox_dir);
    snprintf(src2, sizeof(src2), "%s/source/file2", ex->sandbox_dir);
    snprintf(dst1, sizeof(dst1), "%s/dest/source/file1", ex->sandbox_dir);
    snprintf(dst2, sizeof(dst2), "%s/dest/source/file2", ex->sandbox_dir);
    char *src1c = read_entire_file(src1);
    char *src2c = read_entire_file(src2);
    char *dst1c = read_entire_file(dst1);
    char *dst2c = read_entire_file(dst2);
    int ok = src1c && src2c && dst1c && dst2c && (strcmp(src1c, dst1c) == 0) && (strcmp(src2c, dst2c) == 0);
    free(src1c);
    free(src2c);
    free(dst1c);
    free(dst2c);
    return ok;
}

// Exercise 2: Force overwrite (-f)
int validate_cp_force(Exercise *ex)
{
    char dst[256];
    snprintf(dst, sizeof(dst), "%s/dest/file1", ex->sandbox_dir);
    char *dstc = read_entire_file(dst);
    int ok = dstc && strstr(dstc, "Force overwrite test file.");
    free(dstc);
    return ok;
}

// skip for now
// Exercise 3: Interactive overwrite (-i)
// broken due to piping outputs
int validate_cp_interactive(Exercise *ex)
{
    return 1;
}

// Exercise 4: No clobber (-n)
int validate_cp_no_clobber(Exercise *ex)
{
    char dst[256];
    snprintf(dst, sizeof(dst), "%s/dest/file1", ex->sandbox_dir);
    char *dstc = read_entire_file(dst);
    int ok = dstc && strstr(dstc, "Old file that should not be overwritten.");
    free(dstc);
    return ok;
}

// Exercise 5: Recursive copy (-R)
int validate_cp_recursive(Exercise *ex)
{
    char dst1[256], dst2[256];
    snprintf(dst1, sizeof(dst1), "%s/dest/source/file1", ex->sandbox_dir);
    snprintf(dst2, sizeof(dst2), "%s/dest/source/subdir/file2", ex->sandbox_dir);
    char *dst1c = read_entire_file(dst1);
    char *dst2c = read_entire_file(dst2);
    int ok = dst1c && dst2c && strstr(dst1c, "Recursive copy test file.") && strstr(dst2c, "File in subdir for recursive copy.");
    free(dst1c);
    free(dst2c);
    return ok;
}

// Exercise 6: Update only (-u)
// broken for output verification because the file for dest is actually NEWER
// must use input verification
int validate_cp_update(Exercise *ex)
{
    // char dst[256];
    // snprintf(dst, sizeof(dst), "%s/dest/file1", ex->sandbox_dir);
    // char *dstc = read_entire_file(dst);
    // int ok = dstc && strstr(dstc, "Update only test file (newer).");
    // free(dstc);
    // return ok;
    return strstr(ex->last_user_command, "-u");
}

// Exercise 7: Verbose (-v)
int validate_cp_verbose(Exercise *ex)
{
    // Check that output.txt contains 'file1' and 'copied'
    char outpath[256];
    snprintf(outpath, sizeof(outpath), "%s/dest/file1", ex->sandbox_dir);
    char *out = read_entire_file(outpath);
    int ok = out && strstr(out, "file") && strstr(ex->last_command_output, "->");
    free(out);
    return ok;
}
