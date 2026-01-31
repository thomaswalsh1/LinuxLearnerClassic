// exercises.h
#ifndef EXERCISES_H
#define EXERCISES_H

typedef struct Exercise {
    const char *id;
    const char *title;
    const char *description;
    const char *lab_dir;
    int (*validate)(struct Exercise *ex);
    const char *hint;
    const char *target_file;
    char sandbox_dir[512];
    int is_enabled;
    int is_completed;

    char last_user_command[1024];
    char last_command_output[8192]; 
} Exercise;

typedef struct StudySet{
    char name[64];
    char **exercise_paths;
    int exercise_count;
} StudySet;

// Global exercises array and count
extern Exercise *exercises;
extern int exercise_count;
extern StudySet *study_sets;
extern StudySet *current_study_set;
#endif
