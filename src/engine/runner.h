#ifndef RUNNER_H
#define RUNNER_H

#include "../exercises.h"
#include "app_state.h"

// Global variable for the project root
extern char project_root[512];

/**
 * Initializes the project root
 */
void init_project_root(void);

// Generic runner for any exercise
ExerciseResult run_exercise(Exercise *ex);

/**
 * Launch a "sandbox version of the shell"
 */
void launch_shell(void);

void reset_all_output_files(void);

void reset_single_exercise(Exercise *ex);

// road-to-v0.2.0
Exercise* run_exercise_list_and_select(int *selected_index);

void launch_sandbox_shell(Exercise *ex);

void modify_by_study_set(void);

StudySet *run_study_set_menu(void);
#endif
