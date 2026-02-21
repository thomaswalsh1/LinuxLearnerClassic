// helpers.h
#ifndef HELPERS_H
#define HELPERS_H

#include <ncurses.h>

// Centered printing
void print_center(WINDOW *win, int y, const char *text);
void print_center_multiline(WINDOW *win, int start_y, const char *text);
void print_center_auto(WINDOW *win, int start_y, const char *text);

// Left printing
void print_left_auto(WINDOW *win, int start_y, const char *text);

// Corner printing
void print_topleft(WINDOW *win, int y, int x, const char *text);
void print_topright(WINDOW *win, int y, const char *text);
void print_bottomleft(WINDOW *win, int y, const char *text);
void print_bottomright(WINDOW *win, int y, const char *text);

// print input options
enum Option
{
    SHELL,
    EXIT,
    CONTINUE,
    VIEW_EXERCISES,
    VALIDATE,
    MENU,
    RETURN_INSTRUCTIONS,
    RETURN_MENU,
    RESET_ALL,
    ABLE_EXERCISE,
    GENERATE_RANDOM,
    SELECT_EXERCISE,
    RESET_THESE_EXERCISES,
    MODIFY_STUDY_SET,
    CREATE_STUDY_SET,
    DELETE_STUDY_SET,
    SELECT_STUDY_SET,
    DONE_WITH_CREATING,
    CONFIRM_CREATION,
    START_TYPING_NAME,
    FINISH_TYPING_NAME,
    CONFIRM_STUDY_SET,
    VIEW_INSTRUCTIONS,
    EDIT_NAME,
    CANCEL,
    VIEW_SETTINGS,
    RUN_CURRENT_EXERCISES,
    RUN_SINGULAR_EXERCISE,
    RETRY_EXERCISE,
    NEXT_EXERCISE,
    RETURN_SETTINGS,
    CONFIRM_RESET,
    RESET_EXERCISE,
    STUDY_SETS,
    GET_HINT,
    OPTIONS_END,
};
void print_options(WINDOW *win, const enum Option options[]);

void return_cursor(WINDOW *win);

void print_border_line(WINDOW *win, int y);

void print_page_title(char *title);

#endif
