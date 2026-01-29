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
} Exercise;

// Global exercises array and count
extern Exercise *exercises;
extern int exercise_count;

#endif
