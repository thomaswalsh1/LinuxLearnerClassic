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

// Global exercises array and count
extern Exercise *exercises;
extern int exercise_count;

#endif
