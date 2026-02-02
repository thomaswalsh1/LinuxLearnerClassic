// screens.h
#ifndef SCREENS_H
#define SCREENS_H


#include "exercises.h"
#include "app_state.h"

/**
 * UI helper function that displays the success page
 */
ExerciseResult show_success(void);

/**
 * UI helper function that displays the instructions for an assignment
 */
void show_instructions(const Exercise *ex);

/**
 * UI helper function that displays the failure page
 */
ExerciseResult show_failure(const char *hint);

void show_title(void);

void show_explanation(void);

int show_outputs_reset(void);

void show_exercise_menu(void);

void show_exercise_list_commentary(int top_window_border, int bottom_window_border);

void show_exercise_list_contents(Exercise *viewable_exercises, int top_window_border, int selected_index, int top_index, int per_page);

void show_exercise_selected_menu(Exercise *ex);

// road-to-v0.2.0

void show_main_menu(void);

void show_settings(void);

void show_reset_confirmation_screen(void);

void show_reset_done(void);

void show_all_exercises_completed(void);

void show_study_set_list_commentary(int top_window_border, int bottom_window_border);
void show_study_set_list_contents(StudySet *study_sets, int top_window_border, int selected_index, int top_index, int per_page, int set_count);

#endif
