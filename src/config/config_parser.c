#include "config_parser.h"
#include "../validators/validators.h"
#include "exercises.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>
#include "runner.h"

#define MAX_LINE 512
#define MAX_EXERCISES 100

// Validator registry - maps validator names to function pointers
typedef struct
{
    const char *name;
    int (*func)(Exercise *ex);
} ValidatorEntry;

static ValidatorEntry validator_registry[] = {
    {"grep_basic", validate_grep_basic},
    {"grep_count", validate_grep_count},
    {"grep_dir_skip", validate_grep_dir_skip},
    {"grep_extended", validate_grep_extended},
    {"grep_ignore_case", validate_grep_ignore_case},
    {"grep_recursive", validate_grep_recursive},
    {"grep_invert", validate_grep_invert},
    {"grep_combo", validate_grep_combo},
    {"ls_show_all", validate_ls_show_all},
    {"ls_show_directory", validate_ls_show_directory},
    {"ls_classify", validate_ls_classify},
    {"ls_show_inodes", validate_ls_show_inodes},
    {"ls_long_listing", validate_ls_long_listing},
    {"ls_recursive", validate_ls_recursive},
    {"cp_archive", validate_cp_archive},
    {"cp_force", validate_cp_force},
    {"cp_interactive", validate_cp_interactive},
    {"cp_no_clobber", validate_cp_no_clobber},
    {"cp_recursive", validate_cp_recursive},
    {"cp_update", validate_cp_update},
    {"cp_verbose", validate_cp_verbose},
    {"cut_characters", validate_cut_characters},
    {"cut_bytes", validate_cut_bytes},
    {"cut_delimiter", validate_cut_delimiter},
    {"cut_fields", validate_cut_fields},
    {"cut_only_delimited", validate_cut_only_delimited},
    {"sort_check", validate_sort_check},
    {"sort_ignore_case", validate_sort_ignore_case},
    {"sort_by_field", validate_sort_by_field},
    {"sort_month", validate_sort_month},
    {"sort_numeric", validate_sort_numeric},
    {"sort_output", validate_sort_output},
    {"sort_reverse", validate_sort_reverse},
    {"sort_combined", validate_sort_combined},
    {"wc_bytes", validate_wc_bytes},
    {"wc_lines", validate_wc_lines},
    {"wc_words", validate_wc_words},
    {"wc_chars", validate_wc_chars},
    {"wc_max_line_length", validate_wc_max_line_length},
    {"cat_show_all", validate_cat_show_all},
    {"cat_show_ends", validate_cat_show_ends},
    {"cat_number", validate_cat_number},
    {"cat_squeeze_blank", validate_cat_squeeze_blank},
    {"cat_show_tabs", validate_cat_show_tabs},
    {"cat_show_nonprinting", validate_cat_show_nonprinting},
    {"sed_expression", validate_sed_expression},
    {"sed_file", validate_sed_file},
    {"sed_extended_regex", validate_sed_extended_regex},
    {"awk_field_separator", validate_awk_field_separator},
    {"awk_file", validate_awk_file},
    {"awk_sandbox", validate_awk_sandbox},
    {"validate_mv_f", validate_mv_f},
    {"validate_mv_i", validate_mv_i},
    {"validate_mv_n", validate_mv_n},
    {"validate_mv_u", validate_mv_u},
    {"validate_mv_v", validate_mv_v},
    {"validate_rsync_a", validate_rsync_a},
    {"validate_rsync_D", validate_rsync_D},
    {"validate_rsync_g", validate_rsync_g},
    {"validate_rsync_h", validate_rsync_h},
    {"validate_rsync_l", validate_rsync_l},
    {"validate_rsync_o", validate_rsync_o},
    {"validate_rsync_p", validate_rsync_p},
    {"validate_rsync_progress", validate_rsync_progress},
    {"validate_rsync_r", validate_rsync_r},
    {"validate_rsync_stats", validate_rsync_stats},
    {"validate_rsync_t", validate_rsync_t},
    {"validate_rsync_v", validate_rsync_v},
    {NULL, NULL}};

int (*get_validator_function(const char *validator_name))(void)
{
    if (!validator_name)
        return NULL;

    for (int i = 0; validator_registry[i].name != NULL; i++)
    {
        if (strcmp(validator_registry[i].name, validator_name) == 0)
        {
            return validator_registry[i].func;
        }
    }
    return NULL;
}

// Utility function to trim whitespace from strings
static void trim_string(char *str)
{
    if (!str)
        return;

    // Trim leading whitespace
    char *start = str;
    while (*start && isspace((unsigned char)*start))
        start++;

    // Trim trailing whitespace
    char *end = start + strlen(start) - 1;
    while (end >= start && isspace((unsigned char)*end))
    {
        *end = '\0';
        end--;
    }

    // Move trimmed string back to original
    memmove(str, start, strlen(start) + 1);
}

// Parse a single .conf file
static Exercise parse_conf_file(const char *filepath)
{
    Exercise ex = {0};
    FILE *fp = fopen(filepath, "r");

    if (!fp)
    {
        perror("Failed to open config file");
        return ex;
    }

    char line[MAX_LINE];
    char *validator_name = NULL;

    while (fgets(line, sizeof(line), fp))
    {
        // Remove newline
        line[strcspn(line, "\n")] = '\0';

        // Skip empty lines and comments
        if (line[0] == '\0' || line[0] == '#' || line[0] == ';')
        {
            continue;
        }

        // Parse key=value pairs
        char *eq = strchr(line, '=');
        if (!eq)
            continue;

        *eq = '\0';
        char *key = line;
        char *value = eq + 1;

        trim_string(key);
        trim_string(value);

        if (strcmp(key, "id") == 0)
        {
            ex.id = strdup(value);
        }
        else if (strcmp(key, "title") == 0)
        {
            ex.title = strdup(value);
        }
        else if (strcmp(key, "description") == 0)
        {
            ex.description = strdup(value);
        }
        else if (strcmp(key, "lab_dir") == 0)
        {
            ex.lab_dir = strdup(value);
        }
        else if (strcmp(key, "hint") == 0)
        {
            ex.hint = strdup(value);
        }
        else if (strcmp(key, "target_file") == 0)
        {
            ex.target_file = strdup(value);
        }
        else if (strcmp(key, "validator") == 0)
        {
            validator_name = strdup(value);
        }
        else if (strcmp(key, "completed") == 0)
        {
            ex.is_completed = atoi(value);
        }
        else if (strcmp(key, "preserve_sandbox") == 0) {
            ex.preserve_sandbox = atoi(value);
        }
        else if (strcmp(key, "requires_interaction") == 0) {
            ex.requires_interaction = atoi(value);
        }
    }

    fclose(fp);

    // Get validator function pointer from registry
    if (validator_name)
    {
        ex.validate = get_validator_function(validator_name);
        free(validator_name);
    }
    ex.is_enabled = 1; // enabled by default;
    return ex;
}

ExerciseList load_exercises_from_all(void)
{
    ExerciseList list = {0};
    struct dirent **subdirs;
    int n;

    // Scan the labs/ directory for subdirectories (grep, cut, cat, etc.)
    n = scandir("labs/", &subdirs, NULL, alphasort);
    if (n < 0)
    {
        perror("scandir");
        return list;
    }

    // Allocate space for exercises
    list.exercises = malloc(sizeof(Exercise) * MAX_EXERCISES);
    if (!list.exercises)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return list;
    }

    list.count = 0;

    // Iterate through subdirectories
    for (int i = 0; i < n; i++)
    {
        // Skip . and .. entries
        if (subdirs[i]->d_name[0] == '.')
        {
            free(subdirs[i]);
            continue;
        }

        // Build path to config subdirectory
        char config_path[2048];
        int written = snprintf(config_path, sizeof(config_path), "labs/%s/config",
                               subdirs[i]->d_name);

        if (written < 0 || written >= (int)sizeof(config_path))
        {
            free(subdirs[i]);
            continue;
        }

        // Scan the config subdirectory for .conf files
        struct dirent **configs;
        int config_count = scandir(config_path, &configs, NULL, alphasort);

        if (config_count > 0)
        {
            // Process each .conf file in this config directory
            for (int j = 0; j < config_count && list.count < MAX_EXERCISES; j++)
            {
                if (configs[j]->d_name[0] != '.' &&
                    strlen(configs[j]->d_name) > 5)
                {

                    char *ext = configs[j]->d_name + strlen(configs[j]->d_name) - 5;
                    if (strcmp(ext, ".conf") == 0)
                    {
                        char filepath[2048];
                        written = snprintf(filepath, sizeof(filepath), "%s/%s",
                                           config_path, configs[j]->d_name);

                        if (written >= 0 && written < (int)sizeof(filepath))
                        {
                            Exercise ex = parse_conf_file(filepath);
                            if (ex.id)
                            {
                                list.exercises[list.count++] = ex;
                            }
                        }
                    }
                }
                free(configs[j]);
            }
            free(configs);
        }

        free(subdirs[i]);
    }
    free(subdirs);

    return list;
}

ExerciseList load_exercises_from_config(const char *config_dir)
{
    ExerciseList list = {0};
    struct dirent **namelist;
    int n;

    n = scandir(config_dir, &namelist, NULL, NULL);

    if (n < 0)
    {
        perror("scandir");
        return list;
    }

    // Allocate space for exercises
    list.exercises = malloc(sizeof(Exercise) * MAX_EXERCISES);
    if (!list.exercises)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return list;
    }

    list.count = 0;

    // Process each .conf file
    for (int i = 0; i < n && list.count < MAX_EXERCISES; i++)
    {
        if (namelist[i]->d_name[0] != '.' &&
            strlen(namelist[i]->d_name) > 5)
        {

            char *ext = namelist[i]->d_name + strlen(namelist[i]->d_name) - 5;
            if (strcmp(ext, ".conf") == 0)
            {
                char filepath[512];
                snprintf(filepath, sizeof(filepath), "%s/%s",
                         config_dir, namelist[i]->d_name);

                Exercise ex = parse_conf_file(filepath);
                if (ex.id)
                {
                    list.exercises[list.count++] = ex;
                }
            }
        }
        free(namelist[i]);
    }
    free(namelist);

    return list;
}

ExerciseList test_exercises_from_config(const char *config_dir)
{
    ExerciseList list = {0};
    struct dirent **namelist;
    int n;

    n = scandir(config_dir, &namelist, NULL, alphasort);

    if (n < 0)
    {
        perror("scandir");
        return list;
    }

    // Allocate space for exercises
    list.exercises = malloc(sizeof(Exercise) * MAX_EXERCISES);
    if (!list.exercises)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return list;
    }

    list.count = 0;

    // Process each .conf file
    for (int i = 0; i < n && list.count < MAX_EXERCISES; i++)
    {
        if (namelist[i]->d_name[0] != '.' &&
            strlen(namelist[i]->d_name) > 5)
        {

            char *ext = namelist[i]->d_name + strlen(namelist[i]->d_name) - 5;
            if (strcmp(ext, ".conf") == 0)
            {
                char filepath[512];
                snprintf(filepath, sizeof(filepath), "%s/%s",
                         config_dir, namelist[i]->d_name);

                Exercise ex = parse_conf_file(filepath);
                if (ex.id)
                {
                    list.exercises[list.count++] = ex;
                }
            }
        }
        free(namelist[i]);
    }
    free(namelist);

    return list;
}

void free_exercise_list(ExerciseList list)
{
    for (int i = 0; i < list.count; i++)
    {
        if (list.exercises[i].id)
            free((void *)list.exercises[i].id);
        if (list.exercises[i].title)
            free((void *)list.exercises[i].title);
        if (list.exercises[i].description)
            free((void *)list.exercises[i].description);
        if (list.exercises[i].lab_dir)
            free((void *)list.exercises[i].lab_dir);
        if (list.exercises[i].hint)
            free((void *)list.exercises[i].hint);
        if (list.exercises[i].target_file)
            free((void *)list.exercises[i].target_file);
    }
    free(list.exercises);
}

void modify_exercise_data(Exercise *ex, const enum PersistentOption option)
{
    // reset to lab directory
    if (chdir(project_root) != 0)
    {
        perror("chdir to project root");
        return;
    }

    if (!ex || !ex->id)
    {
        fprintf(stderr, "Invalid exercise provided\n");
        return;
    }
    // find the configuration file for this exercise
    // we need to search through the labs directory structure
    char config_path[2048];
    char temp_path[2048];
    FILE *fp = NULL;
    FILE *temp_fp = NULL;

    // Try to find the config file by scanning labs subdirectories
    struct dirent **subdirs;
    int n = scandir("labs/", &subdirs, NULL, alphasort);

    if (n < 0)
    {
        perror("scandir failed");
        return;
    }

    int found = 0;

    for (int i = 0; i < n && !found; i++)
    {
        if (subdirs[i]->d_name[0] == '.')
        {
            free(subdirs[i]);
            continue;
        }

        // Build path to config subdirectory
        char search_path[2048];
        int written = snprintf(search_path, sizeof(search_path),
                               "labs/%s/config", subdirs[i]->d_name);

        if (written < 0 || written >= (int)sizeof(search_path))
        {
            free(subdirs[i]);
            continue;
        }

        // Scan for .conf files
        struct dirent **configs;
        int config_count = scandir(search_path, &configs, NULL, alphasort);

        if (config_count > 0)
        {
            for (int j = 0; j < config_count; j++)
            {
                if (configs[j]->d_name[0] != '.' && strlen(configs[j]->d_name) > 5)
                {
                    char *ext = configs[j]->d_name + strlen(configs[j]->d_name) - 5;
                    if (strcmp(ext, ".conf") == 0)
                    {
                        int config_bytes = snprintf(config_path, sizeof(config_path),
                                                    "%s/%s", search_path, configs[j]->d_name);
                        if (config_bytes < 0 || config_bytes >= (int)sizeof(config_path))
                        {
                            perror("Config path is too long");
                            continue; // or return error
                        }
                        // Check if this is the right config file by reading the id
                        FILE *check_fp = fopen(config_path, "r");
                        if (check_fp)
                        {
                            char line[MAX_LINE];
                            while (fgets(line, sizeof(line), check_fp))
                            {
                                line[strcspn(line, "\n")] = '\0';
                                if (line[0] == '\0' || line[0] == '#' || line[0] == ';')
                                {
                                    continue;
                                }

                                char *eq = strchr(line, '=');
                                if (eq)
                                {
                                    *eq = '\0';
                                    char *key = line;
                                    char *value = eq + 1;
                                    trim_string(key);
                                    trim_string(value);

                                    if (strcmp(key, "id") == 0 && strcmp(value, ex->id) == 0)
                                    {
                                        found = 1;
                                        fclose(check_fp);
                                        break;
                                    }
                                }
                            }
                            if (!found)
                            {
                                fclose(check_fp);
                            }
                        }
                    }
                }
                free(configs[j]);
                if (found)
                    break;
            }
            free(configs);
        }
        free(subdirs[i]);
    }
    free(subdirs);

    if (!found)
    {
        fprintf(stderr, "Configuration file not found for exercise: %s\n", ex->id);
        return;
    }

    // Open the original config file for reading
    fp = fopen(config_path, "r");
    if (!fp)
    {
        perror("Failed to open config file");
        return;
    }

    // Create a temporary file for writing
    int bytes = snprintf(temp_path, sizeof(temp_path), "%s.tmp", config_path);
    if (bytes < 0 || bytes >= (int)sizeof(temp_path))
    {
        perror("temp_path gathering failed");
        return;
    }
    temp_fp = fopen(temp_path, "w");
    if (!temp_fp)
    {
        perror("Failed to create temporary file");
        fclose(fp);
        return;
    }

    // Determine the new value based on the option
    int new_value = (option == MARK_EX_COMPLETE) ? 1 : 0;
    int completed_found = 0;

    // Read line by line and modify the completed attribute
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp))
    {
        // Check if this is the completed line
        char temp_line[MAX_LINE];
        strncpy(temp_line, line, sizeof(temp_line) - 1);
        temp_line[sizeof(temp_line) - 1] = '\0';
        temp_line[strcspn(temp_line, "\n")] = '\0';

        if (temp_line[0] != '\0' && temp_line[0] != '#' && temp_line[0] != ';')
        {
            char *eq = strchr(temp_line, '=');
            if (eq)
            {
                *eq = '\0';
                char *key = temp_line;
                trim_string(key);

                if (strcmp(key, "completed") == 0)
                {
                    fprintf(temp_fp, "completed=%d\n", new_value);
                    completed_found = 1;
                    continue;
                }
            }
        }

        // Write the original line
        fputs(line, temp_fp);
    }

    // If completed attribute was not found, add it at the end
    if (!completed_found)
    {
        fprintf(temp_fp, "completed=%d\n", new_value);
    }

    fclose(fp);
    fclose(temp_fp);

    // Replace the original file with the temporary file
    if (rename(temp_path, config_path) != 0)
    {
        perror("Failed to replace config file");
        remove(temp_path);
    }
}

StudySet get_default_study_set(void)
{
    StudySet set = {0};
    char set_name[64] = {0};
    char set_path[256] = {0};
    char line[256];

    if (chdir(project_root) != 0)
    {
        perror("chdir to project root");
        return;
    }

    FILE *fp = fopen("saves/sets/default.conf", "r");
    if (!fp)
    {
        perror("Failed to open saves/sets/default.conf");
        return set;
    }

    while (fgets(line, sizeof(line), fp))
    {
        line[strcspn(line, "\n")] = '\0';
        trim_string(line);
        if (strncmp(line, "default=", 8) == 0)
        {
            strncpy(set_name, line + 8, sizeof(set_name) - 1);
            trim_string(set_name);
            break;
        }
    }
    fclose(fp);

    if (set_name[0] == '\0')
    {
        perror("No default set");
        return set;
    }

    snprintf(set_path, sizeof(set_path), "saves/sets/%s.set", set_name);

    fp = fopen(set_path, "r");
    if (!fp)
    {
        perror("Failed to open study set file");
        return set;
    }

    // Allocate space for exercise paths
    set.exercise_paths = malloc(sizeof(char *) * MAX_EXERCISES);
    set.exercise_count = 0;
    strncpy(set.name, set_name, sizeof(set.name) - 1);

    // Read each exercise path
    while (fgets(line, sizeof(line), fp) && set.exercise_count < MAX_EXERCISES)
    {
        line[strcspn(line, "\n")] = '\0';
        trim_string(line);
        if (line[0] == '\0' || line[0] == '#' || line[0] == ';')
            continue;
        set.exercise_paths[set.exercise_count++] = strdup(line);
    }
    fclose(fp);

    return set;
}

static void strip_set_extension(char *name) {
    char *ext = strrchr(name, '.');
    if (ext && strcmp(ext, ".set") == 0) {
        *ext = '\0';
    }
}

StudySet get_study_set_by_name(char *name)
{
    StudySet set = {0};
    char set_path[256];
    char line[MAX_LINE];

    // Build the path to the study set file
    snprintf(set_path, sizeof(set_path), "saves/sets/%s", name);

    FILE *fp = fopen(set_path, "r");
    if (!fp)
    {
        perror("Failed to open study set file");
        return set;
    }

    // Set the study set name (strip extension if you want, or keep as is)
    strncpy(set.name, name, sizeof(set.name) - 1);
    set.name[sizeof(set.name) - 1] = '\0';
    strip_set_extension(set.name);

    set.exercise_paths = malloc(sizeof(char *) * MAX_EXERCISES);
    set.exercise_count = 0;

    while (fgets(line, sizeof(line), fp) && set.exercise_count < MAX_EXERCISES)
    {
        line[strcspn(line, "\n")] = '\0';
        trim_string(line);
        if (line[0] == '\0' || line[0] == '#' || line[0] == ';')
            continue;
        set.exercise_paths[set.exercise_count++] = strdup(line);
    }
    fclose(fp);

    return set;
}



StudySetList get_study_set_list(void)
{
    StudySetList list = {0};
    struct dirent **namelist;
    int n;

    n = scandir("saves/sets/", &namelist, NULL, alphasort);
    if (n < 0)
    {
        perror("scandir");
        return list;
    }

    list.study_sets = malloc(sizeof(StudySet) * 64); // up to 64 sets
    if (!list.study_sets)
    {
        fprintf(stderr, "Memory allocation failed\n");
        for (int i = 0; i < n; i++)
            free(namelist[i]);
        free(namelist);
        return list;
    }

    list.count = 0;

    for (int i = 0; i < n; i++)
    {
        // skip . and ..
        if (namelist[i]->d_name[0] == '.')
        {
            free(namelist[i]);
            continue;
        }
        // Only include .set files
        char *ext = strrchr(namelist[i]->d_name, '.');
        if (!ext || strcmp(ext, ".set") != 0)
        {
            free(namelist[i]);
            continue;
        }

        // Fill in the StudySet struct
        StudySet *set = &list.study_sets[list.count];
        memset(set, 0, sizeof(StudySet));
        strncpy(set->name, namelist[i]->d_name, sizeof(set->name) - 1);
        set->name[sizeof(set->name) - 1] = '\0';
        strip_set_extension(set->name);

        // Now, load exercise paths from the .set file
        char set_path[256];
        snprintf(set_path, sizeof(set_path), "saves/sets/%s", namelist[i]->d_name);
        FILE *fp = fopen(set_path, "r");
        if (fp)
        {
            set->exercise_paths = malloc(sizeof(char *) * MAX_EXERCISES);
            set->exercise_count = 0;
            char line[MAX_LINE];
            while (fgets(line, sizeof(line), fp) && set->exercise_count < MAX_EXERCISES)
            {
                line[strcspn(line, "\n")] = '\0';
                trim_string(line);
                if (line[0] == '\0' || line[0] == '#' || line[0] == ';')
                    continue;
                set->exercise_paths[set->exercise_count++] = strdup(line);
            }
            fclose(fp);
        } else {
            set->exercise_paths = NULL;
            set->exercise_count = 0;
        }

        list.count++;
        free(namelist[i]);
    }
    free(namelist);

    return list;
}

void save_study_set_disk(StudySet *set) {
    if (!set || !set->name[0] || !set->exercise_paths) {
        fprintf(stderr, "Invalid StudySet provided to save_study_set_disk\n");
        return;
    }

    char set_path[256];
    snprintf(set_path, sizeof(set_path), "saves/sets/%s.set", set->name);

    FILE *fp = fopen(set_path, "w");
    if (!fp) {
        perror("Failed to open study set file for writing");
        return;
    }

    for (int i = 0; i < set->exercise_count; i++) {
        if (set->exercise_paths[i] && set->exercise_paths[i][0]) {
            fprintf(fp, "%s\n", set->exercise_paths[i]);
        }
    }

    fclose(fp);
}