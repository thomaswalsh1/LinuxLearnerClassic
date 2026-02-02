// screens.c
#include "screens.h"
#include "helpers.h"
#include <ncurses.h>
#include "exercises.h"
#include "app_state.h"
#include <string.h>
#include "runner.h"
#include <stdlib.h>
#include <errno.h>

#define GRID_COLS 3
#define CELL_WIDTH 25
#define CELL_HEIGHT 3

ExerciseResult show_success(void)
{
    const enum Option options[] = {EXIT, RETURN_MENU, NEXT_EXERCISE, OPTIONS_END};

    while (1)
    {
        clear();
        print_left_auto(stdscr, 5, "Success!");
        print_left_auto(stdscr, 7, "You completed the exercise correctly.");
        print_options(stdscr, options);
        refresh();

        int ch = getch();
        if (ch == KEY_RESIZE)
        {
            continue; // redraw on resize
        }
        if (ch == 27)
        {
            return ACTION_EXIT;
        }
        if (ch == 127 || ch == KEY_BACKSPACE)
        {
            return ACTION_RETURN;
        }
        if (ch == '\n' || ch == KEY_ENTER)
        {
            return ACTION_CONTINUE;
        }
    }
}

ExerciseResult show_failure(const char *hint)
{
    const enum Option options[] = {EXIT, RETURN_MENU, GET_HINT, RETRY_EXERCISE, OPTIONS_END};
    int show_hint = 0;
    while (1)
    {
        clear();
        print_left_auto(stdscr, 5, "Not quite.");
        print_left_auto(stdscr, 7, "Check your command reference and try again.");
        char hint_str[256] = "Hint: ";
        strcat(hint_str, hint);
        if (show_hint)
            print_left_auto(stdscr, 9, hint_str);
        print_options(stdscr, options);
        refresh();
        int ch = getch();
        if (ch == KEY_RESIZE)
        {
            continue; // redraw on resize
        }
        if (ch == 27)
        { // ESC
            return ACTION_EXIT;
        }
        if (ch == 'h' || ch == 'H')
        {
            show_hint = 1;
            continue;
        }
        if (ch == 127 || ch == KEY_BACKSPACE)
        {
            return ACTION_RETURN;
        }
        if (ch == '\n' || ch == KEY_ENTER)
        {
            return ACTION_RETRY;
        }
    }
}

void show_instructions(const Exercise *ex)
{
    const enum Option options[] = {EXIT, RETURN_MENU, VALIDATE, SHELL, OPTIONS_END};
    clear();

    mvhline(4, 2, ACS_HLINE, COLS - 4);

    print_left_auto(stdscr, 4, ex->lab_dir);
    print_left_auto(stdscr, 5, ex->title);
    mvhline(6, 2, ACS_HLINE, COLS - 4);

    print_left_auto(stdscr, 7, ex->description);
    print_options(stdscr, options);
    refresh();
}

void show_title(void)
{
    const enum Option options[] = {EXIT, CONTINUE, OPTIONS_END};
    clear();
    mvhline(1, 2, ACS_HLINE, COLS - 4);
    print_page_title("LinuxLearner v0.1.1");
    mvhline(3, 2, ACS_HLINE, COLS - 4);
    print_center_auto(stdscr, 5, "CompTIA Linux+ text and file exercises");
    print_center_auto(stdscr, 7, "By Thomas Walsh");
    print_options(stdscr, options);
    refresh();
}

int show_outputs_reset(void)
{
    while (1)
    {
        clear();
        print_left_auto(stdscr, 5, "All output files have been reset.");
        print_left_auto(stdscr, 7, "You can now go through all of the exercises again.");
        print_bottomleft(stdscr, 0, "ENTER when done, ESC to quit");
        refresh();

        int ch = getch();
        if (ch == KEY_RESIZE)
        {
            continue; // redraw on resize
        }
        if (ch == 27)
        { // ESC
            return 1;
        }
        return 2;
    }
}

void show_exercise_menu(void)
{
    const enum Option options[] = {RETURN_INSTRUCTIONS, VIEW_EXERCISES, OPTIONS_END};
    clear();
    print_left_auto(stdscr, 5, "This is the exercise menu.");
    print_left_auto(stdscr, 7, "Here you can jump to an exercise by viewing the exercise list.");
    print_options(stdscr, options);
    refresh();
}

void show_exercise_list_commentary(int top_window_border, int bottom_window_border)
{
    const enum Option options[] = {RETURN_MENU, STUDY_SETS, SELECT_EXERCISE, OPTIONS_END};

    clear();
    print_left_auto(stdscr, 1, "This is the exercise list.");
    print_left_auto(stdscr, 2, "Use WASD or the arrow keys to navigate through the exercises.");
    print_border_line(stdscr, top_window_border);
    print_border_line(stdscr, bottom_window_border);
    print_options(stdscr, options);
    refresh();
}

void show_exercise_list_contents(
    Exercise *viewable_exercises,
    int top_window_border,
    int selected_index,
    int top_index,
    int per_page)
{
    int y = top_window_border + 1;
    int end = top_index + per_page - 1;
    if (end > exercise_count)
        end = exercise_count;

    // Clear only the content area (not the whole screen)
    for (int i = top_index; i <= end; i++)
    {
        move(y, 0);
        clrtoeol(); // Clear only this line

        if (i == selected_index)
        {
            attron(A_REVERSE);
            mvwhline(stdscr, y, 0, ' ', COLS - 1);
            mvwprintw(stdscr, y, 0, ">");
        }

        mvwprintw(stdscr, y, 2, "%s", viewable_exercises[i].title);
        mvwprintw(stdscr, y, 40, "%s", viewable_exercises[i].lab_dir);
        mvwprintw(stdscr, y, 60, "%s", viewable_exercises[i].is_enabled == 1 ? "on" : "off");
        mvwprintw(stdscr, y, 65, "  |  ");
        mvwprintw(stdscr, y, 70, "%s", viewable_exercises[i].is_completed == 1 ? "done" : "    ");

        // Turn off highlight
        if (i == selected_index)
        {
            attroff(A_REVERSE);
        }

        y++;
    }

    return_cursor(stdscr);
    refresh(); // Single refresh after all updates
}

void show_exercise_selected_menu(Exercise *ex)
{
    const enum Option options[] = {RETURN_MENU, RESET_EXERCISE, ABLE_EXERCISE, RUN_SINGULAR_EXERCISE, OPTIONS_END};

    clear();
    char description_line[256];
    char enabled_line[256];
    char directory_line[256];
    char completed_line[256];
    snprintf(description_line, sizeof(description_line), "Description: %s", ex->description);
    snprintf(enabled_line, sizeof(enabled_line), "Enabled: %s", ex->is_enabled == 1 ? "On" : "Off");
    snprintf(directory_line, sizeof(directory_line), "Directory: %s", ex->lab_dir);
    snprintf(completed_line, sizeof(completed_line), "Completed: %s", ex->is_completed == 1 ? "Yes" : "Not yet finished.");

    mvhline(1, 2, ACS_HLINE, COLS - 4);
    print_center_auto(stdscr, 2, ex->lab_dir);
    mvhline(3, 2, ACS_HLINE, COLS - 4);

    print_left_auto(stdscr, 5, ex->title);
    print_left_auto(stdscr, 7, description_line);
    print_left_auto(stdscr, 9, enabled_line);
    print_left_auto(stdscr, 11, directory_line);
    print_left_auto(stdscr, 13, completed_line);
    print_options(stdscr, options);

    refresh();
}

// road-to-v0.2.0
void show_main_menu(void)
{
    const enum Option options[] = {EXIT, VIEW_INSTRUCTIONS, VIEW_SETTINGS, VIEW_EXERCISES, RUN_CURRENT_EXERCISES, OPTIONS_END};
    clear();
    mvhline(1, 2, ACS_HLINE, COLS - 4);
    print_center_auto(stdscr, 2, "Main Menu");
    mvhline(3, 2, ACS_HLINE, COLS - 4);
    print_left_auto(stdscr, 7, "Welcome to the main menu of LinuxLearner.");
    char study_set_string[256];
    snprintf(study_set_string, sizeof(study_set_string), "Current study set: %s", current_study_set != NULL ? current_study_set->name : "None");
    print_left_auto(stdscr, 9, study_set_string);
    print_options(stdscr, options);
    refresh();
}

void show_explanation(void)
{
    const enum Option options[] = {RETURN_MENU, OPTIONS_END};
    clear();
    mvhline(1, 2, ACS_HLINE, COLS - 4);
    print_center_auto(stdscr, 2, "How this works:");
    mvhline(3, 2, ACS_HLINE, COLS - 4);
    print_left_auto(stdscr, 4, "Follow the instructions and enter the corresponding commands to complete exercises.");
    print_options(stdscr, options);
    refresh();
}

void show_settings(void)
{
    const enum Option options[] = {RETURN_MENU, RESET_ALL, OPTIONS_END};
    clear();
    mvhline(1, 2, ACS_HLINE, COLS - 4);
    print_center_auto(stdscr, 2, "Settings");
    mvhline(3, 2, ACS_HLINE, COLS - 4);
    print_options(stdscr, options);
    refresh();
}

void show_reset_confirmation_screen(void)
{
    const enum Option options[] = {RETURN_SETTINGS, CONFIRM_RESET, OPTIONS_END};
    clear();
    print_left_auto(stdscr, 4, "Are you sure you want to reset all exercises?");
    print_options(stdscr, options);
    refresh();
}

void show_reset_done(void)
{
    const enum Option options[] = {RETURN_SETTINGS, OPTIONS_END};
    clear();
    print_left_auto(stdscr, 4, "All exercises have been reset and marked incomplete.");
    print_options(stdscr, options);
    refresh();
}

void show_all_exercises_completed(void)
{
    const enum Option options[] = {RETURN_MENU, OPTIONS_END};
    clear();
    print_left_auto(stdscr, 4, "All exercises in your set have been completed. Go to settings to reset them.");
    print_options(stdscr, options);
    refresh();
}

// study sets

void show_study_set_list_commentary(int top_window_border, int bottom_window_border)
{
    const enum Option options[] = {RETURN_MENU, SELECT_STUDY_SET, OPTIONS_END};

    clear();
    print_left_auto(stdscr, 1, "This is the study set list.");
    print_left_auto(stdscr, 2, "Use W and S or the arrow keys to navigate through the study sets.");
    print_border_line(stdscr, top_window_border);
    print_border_line(stdscr, bottom_window_border);
    print_options(stdscr, options);
    refresh();
}

void show_study_set_list_contents(StudySet *study_sets, int top_window_border, int selected_index, int top_index, int per_page, int set_count)
{
    int y = top_window_border + 1;
    int end = top_index + per_page - 1;
    if (end > set_count)
        end = set_count;

    // Clear only the content area (not the whole screen)
    for (int i = top_index; i <= end; i++)
    {
        move(y, 0);
        clrtoeol(); // Clear only this line

        if (i == selected_index)
        {
            attron(A_REVERSE);
            mvwhline(stdscr, y, 0, ' ', COLS - 1);
            mvwprintw(stdscr, y, 0, ">");
        }

        if (i == 0) {
            mvwprintw(stdscr, y, 2, "[NONE] Run all exercises");
        } else {
            mvwprintw(stdscr, y, 2, "%s", study_sets[i].name);
        }

        // Turn off highlight
        if (i == selected_index)
        {
            attroff(A_REVERSE);
        }

        y++;
    }

    return_cursor(stdscr);
    refresh(); // Single refresh after all updates
}