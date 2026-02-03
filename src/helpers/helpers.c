// helpers.c
#include "helpers.h"
#include <string.h>
#include "app_state.h"
#include <stdio.h>

void print_center(WINDOW *win, int y, const char *text)
{
    int x = (COLS - strlen(text)) / 2;
    mvwprintw(win, y, x, "%s", text);
}

void print_center_auto(WINDOW *win, int start_y, const char *text)
{
    int max_width = COLS - 4; // margins
    int y = start_y;

    char line[1024] = "";
    int line_len = 0;

    const char *p = text;

    while (*p)
    {
        // Skip spaces
        while (*p == ' ')
            p++;

        // Extract a word
        char word[256];
        int wlen = 0;
        while (*p && *p != ' ')
        {
            word[wlen++] = *p++;
        }
        word[wlen] = '\0';

        if (wlen == 0)
            break;

        // If word won't fit on the current line, print line first
        if (line_len > 0 && line_len + 1 + wlen > max_width)
        {
            print_center(win, y++, line);
            line[0] = '\0';
            line_len = 0;
        }

        // If word is longer than max width, force-break it
        if (wlen > max_width)
        {
            if (line_len > 0)
            {
                print_center(win, y++, line);
                line[0] = '\0';
                line_len = 0;
            }

            for (int i = 0; i < wlen; i += max_width)
            {
                char chunk[256];
                int len = (wlen - i > max_width) ? max_width : wlen - i;
                strncpy(chunk, word + i, len);
                chunk[len] = '\0';
                print_center(win, y++, chunk);
            }
            continue;
        }

        // Append word to line
        if (line_len > 0)
        {
            strcat(line, " ");
            line_len++;
        }
        strcat(line, word);
        line_len += wlen;
    }

    // Print remaining text
    if (line_len > 0)
    {
        print_center(win, y, line);
    }
}

void print_left_auto(WINDOW *win, int start_y, const char *text)
{
    int margin = 2;
    int max_width = COLS - (margin * 2);
    int y = start_y;

    char line[1024] = "";
    int line_len = 0;

    const char *p = text;

    while (*p)
    {
        // Skip spaces
        while (*p == ' ')
            p++;

        // Extract word
        char word[256];
        int wlen = 0;
        while (*p && *p != ' ')
        {
            word[wlen++] = *p++;
        }
        word[wlen] = '\0';

        if (wlen == 0)
            break;

        // If word doesn't fit on current line, print line
        if (line_len > 0 && line_len + 1 + wlen > max_width)
        {
            mvwprintw(win, y++, margin, "%s", line);
            line[0] = '\0';
            line_len = 0;
        }

        // Force-break very long words
        if (wlen > max_width)
        {
            if (line_len > 0)
            {
                mvwprintw(win, y++, margin, "%s", line);
                line[0] = '\0';
                line_len = 0;
            }

            for (int i = 0; i < wlen; i += max_width)
            {
                char chunk[256];
                int len = (wlen - i > max_width) ? max_width : wlen - i;
                strncpy(chunk, word + i, len);
                chunk[len] = '\0';
                mvwprintw(win, y++, margin, "%s", chunk);
            }
            continue;
        }

        // Append word
        if (line_len > 0)
        {
            strcat(line, " ");
            line_len++;
        }
        strcat(line, word);
        line_len += wlen;
    }

    // Print leftover
    if (line_len > 0)
    {
        mvwprintw(win, y, margin, "%s", line);
    }
}

void print_center_multiline(WINDOW *win, int start_y, const char *text)
{
    // Split text on \n and call print_center for each line
    int y = start_y;
    char buffer[1024];
    int i = 0, j = 0;
    while (text[i])
    {
        if (text[i] == '\n')
        {
            buffer[j] = '\0';
            print_center(win, y++, buffer);
            j = 0;
        }
        else
        {
            buffer[j++] = text[i];
        }
        i++;
    }
    if (j > 0)
        buffer[j] = '\0', print_center(win, y, buffer);
}

void print_topleft(WINDOW *win, int y, int x, const char *text)
{
    mvwprintw(win, y, x, "%s", text);
}

void print_topright(WINDOW *win, int y, const char *text)
{
    int x = COLS - strlen(text) - 1;
    mvwprintw(win, y, x, "%s", text);
}

void print_bottomleft(WINDOW *win, int y, const char *text)
{
    mvwprintw(win, LINES - y - 1, 0, "%s", text);
}

void print_bottomright(WINDOW *win, int y, const char *text)
{
    int x = COLS - strlen(text) - 1;
    mvwprintw(win, LINES - y - 1, x, "%s", text);
}

void print_options(WINDOW *win, const enum Option options[])
{
    int margin = 2;
    int y = LINES - 2; // always start at the bottom of the screen
    for (int i = 0; options[i] != OPTIONS_END; i++)
    {
        const char *text = "";

        switch (options[i])
        {
        case SHELL:
            text = "Press S for shell";
            break;
        case EXIT:
            text = "Press ESC to exit";
            break;
        case CONTINUE:
            text = "Press ENTER to continue";
            break;
        case RESET_ALL:
            text = "Press R to reset all exercises";
            break;
        case VALIDATE:
            text = "Press ENTER to test your solution";
            break;
        case RETURN_INSTRUCTIONS:
            text = "Press BACKSPACE to return to instructions";
            break;
        case RETURN_MENU:
            text = "Press BACKSPACE to return to the menu";
            break;
        case SELECT_EXERCISE:
            text = "Press ENTER to select an exercise";
            break;
        case SELECT_STUDY_SET:
            text = "Press ENTER to select a study set";
            break;
        case CONFIRM_STUDY_SET:
            text = "Press ENTER to use this study set";
            break;
        case GENERATE_RANDOM:
            text = "Press R to start creating a randomized study set";
            break;
        case VIEW_EXERCISES:
            text = "Press V to view exercises and study sets";
            break;
        case VIEW_INSTRUCTIONS:
            text = "Press H to for instructions";
            break;
        case VIEW_SETTINGS:
            text = "Press S to go to settings";
            break;
        case RUN_CURRENT_EXERCISES:
            text = "Press ENTER to run current exercises";
            break;
        case ABLE_EXERCISE:
            text = "Press E to enable/disable the exercise";
            break;
        case RUN_SINGULAR_EXERCISE:
            text = "Press ENTER to run this exercise";
            break;
        case NEXT_EXERCISE:
            text = "Press ENTER to go the next exercise";
            break;
        case RETRY_EXERCISE:
            text = "Press ENTER to retry the exercise";
            break;
        case GET_HINT:
            text = "Press H to get a hint";
            break;
        case STUDY_SETS:
            text = "Press M to manage study sets";
            break;
        case RETURN_SETTINGS:
            text = "Press BACKSPACE to return to settings";
            break;
        case RESET_EXERCISE:
            text = "Press R to reset this exercise";
            break;
        case CONFIRM_RESET:
            text = "Press ENTER to reset all exercises";
            break;
        case MENU:
            text = "Press M for menu";
            break;
        default:
            break;
        }

        mvwprintw(win, y--, margin, "%s", text);
    }
    return_cursor(win);
}

void print_border_line(WINDOW *win, int y) {
    int x = 0;
    for (int i = 0; i < COLS; i++) {
        mvwprintw(win, y, x++, "-");
    }
}

void return_cursor(WINDOW *win)
{
    wmove(win, LINES - 1, 0);
}

void print_page_title(char *title) {
    wattron(stdscr, COLOR_PAIR(COLOR_TITLE) | A_BOLD);
    print_center_auto(stdscr, 2, title);
    wattroff(stdscr, COLOR_PAIR(COLOR_TITLE) | A_BOLD);
}