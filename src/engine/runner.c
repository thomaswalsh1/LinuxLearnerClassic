#include "runner.h"
#include "helpers.h"
#include "screens.h"
#include "app_state.h"
#include "config_parser.h"
#include "exercises.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ncurses.h>
#include <errno.h>
#include <ncurses.h>

char project_root[512];

void init_project_root(void)
{
    if (!getcwd(project_root, sizeof(project_root)))
    { // assign to the project root buffer
        perror("getcwd");
        exit(1);
    }
}

void mark_complete(Exercise *ex)
{
    // change in memory
    ex->is_completed = 1;
    // now change on disk
    modify_exercise_data(ex, MARK_EX_COMPLETE);
};

void mark_incomplete(Exercise *ex)
{
    // change in memory
    ex->is_completed = 0;
    // now change on disk
    modify_exercise_data(ex, MARK_EX_INCOMPLETE);
}

void reset_all_output_files(void)
{
    if (chdir(project_root) != 0)
    {
        perror("reset_all_output_files: chdir project_root");
        return;
    }
    for (int i = 0; i < exercise_count; i++)
    {
        Exercise *ex = &exercises[i];

        mark_incomplete(ex);

        // reduntant checking for target file on object
        if (!ex->target_file || strlen(ex->target_file) == 0)
        {
            continue;
        }
        // check for lab directory
        if (chdir(ex->lab_dir) != 0)
        {
            perror("reset_all_output_files: chdir lab_dir");
            chdir(project_root);
            continue;
        }
        // attempt to remove
        if (remove(ex->target_file) == 0)
        {
            printf("%s : target file %s removed. \n", ex->title, ex->target_file);
        }
        else
        {
            printf("%s : failed to remove target file %s \n", ex->title, ex->target_file);
        }
        chdir(project_root);
    }
}

void reset_single_exercise(Exercise *ex)
{
    if (chdir(project_root) != 0)
    {
        perror("reset_all_output_files: chdir project_root");
        return;
    }
    mark_incomplete(ex);

    if (!ex->target_file || strlen(ex->target_file) == 0)
        return;
    // check for lab directory
    if (chdir(ex->lab_dir) != 0)
    {
        perror("reset_all_output_files: chdir lab_dir");
        chdir(project_root);
        return;
    }
    // attempt to remove
    if (remove(ex->target_file) == 0)
        printf("%s : target file %s removed. \n", ex->title, ex->target_file);
    else
        printf("%s : failed to remove target file %s \n", ex->title, ex->target_file);
    chdir(project_root);
}

ExerciseResult run_exercise(Exercise *ex)
{
    if (ex->is_enabled == 0)
    {
        return ACTION_SKIP;
    }

    int ch;

    // reset to lab directory
    if (chdir(project_root) != 0)
    {
        perror("chdir to project root");
        return ACTION_EXIT;
    }

    // // change to exercise sandbox
    // if (chdir(ex->lab_dir) != 0)
    // {
    //     perror("chdir to correct exercise");
    //     return ACTION_EXIT;
    // }

    // write the instructions
    show_instructions(ex);

    // track input
    while (1)
    {
        ch = getch();
        if (ch == KEY_RESIZE)
        {
            show_instructions(ex);
        }
        if (ch == 's' || ch == 'S')
            launch_sandbox_shell(ex);
        if (ch == 27)
            return ACTION_EXIT;
        if (ch == 127 || ch == KEY_BACKSPACE)
            return ACTION_RETURN;
        if (ch == '\n' || ch == KEY_ENTER)
        {
            if (ex->validate(ex))
            {
                // success
                mark_complete(ex);
                return show_success();
            }
            else
            {
                // fail
                return show_failure(ex->hint);
            }
        }
    }
}

int create_sandbox(char *out_path, size_t size)
{
    snprintf(out_path, size, "/tmp/lab_%d_XXXXXX", getpid());
    return mkdtemp(out_path) != NULL;
}

void populate_sandbox(const char *src, const char *dst)
{
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "cp -a %s/. %s/", src, dst);
    system(cmd); // OK here since YOU control inputs
}

void launch_sandbox_shell(Exercise *ex)
{
    char sandbox[512];

    if (!create_sandbox(sandbox, sizeof(sandbox)))
    {
        perror("sandbox");
        return;
    }

    populate_sandbox(ex->lab_dir, sandbox);

    printf("Current assignment -> %s\n", ex->lab_dir);

    def_prog_mode();
    endwin(); // temporarily exit ncurses

    printf("\n--- LAB SHELL (SANDBOXED) ---\n");
    printf("Type commands. Type 'exit' to return.\n\n");

    char captured_output[1024] = {0}; // store last command's stdout

    while (1)
    {
        char *cmd = NULL;
        size_t len = 0;
        printf("sandbox $ ");
        fflush(stdout);

        if (getline(&cmd, &len, stdin) == -1)
        {
            free(cmd);
            break;
        }

        // remove trailing newline
        cmd[strcspn(cmd, "\n")] = 0;

        if (strncmp(cmd, "exit", 4) == 0)
        {
            free(cmd);
            break;
        }

        int pipefd[2];
        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            free(cmd);
            continue;
        }

        pid_t pid = fork();
        if (pid == 0)
        {
            // Child process
            chdir(sandbox);
            clearenv();
            setenv("PATH", "/bin:/usr/bin", 1);

            setuid(65534); // nobody
            setgid(65534);

            // redirect stdout/stderr to pipe
            close(pipefd[0]); // close read end
            dup2(pipefd[1], STDOUT_FILENO);
            dup2(pipefd[1], STDERR_FILENO);
            close(pipefd[1]);

            execl("/bin/sh", "sh", "-c", cmd, NULL);
            perror("exec");
            _exit(1);
        }

        // Parent process
        close(pipefd[1]); // close write end
        ssize_t n;
        size_t offset = 0;
        char buffer[4096];

        // Clear captured_output for this command
        memset(captured_output, 0, sizeof(captured_output));

        // Read child's stdout/stderr
        while ((n = read(pipefd[0], buffer, sizeof(buffer)-1)) > 0)
        {
            buffer[n] = '\0';
            printf("%s", buffer); // show output live

            // store in captured_output
            if (offset + n < sizeof(captured_output)-1)
            {
                memcpy(captured_output + offset, buffer, n);
                offset += n;
                captured_output[offset] = '\0';
            }
        }
        close(pipefd[0]);

        // Wait for child to finish
        waitpid(pid, NULL, 0);

        // Store the last user command and its output for validation
        strncpy(ex->last_user_command, cmd, sizeof(ex->last_user_command)-1);
        ex->last_user_command[sizeof(ex->last_user_command)-1] = '\0';
        strncpy(ex->last_command_output, captured_output, sizeof(ex->last_command_output)-1);
        ex->last_command_output[sizeof(ex->last_command_output)-1] = '\0';

        free(cmd);
    }

    // cleanup sandbox
    char rm[600];
    snprintf(rm, sizeof(rm), "rm -rf %s", sandbox);
    system(rm);

    reset_prog_mode();
    refresh();
}

void launch_shell(void)
{
    def_prog_mode(); // prog mode
    endwin();        // close out

    // introduce the shell to the user
    system("clear");
    printf("\n--- LAB SHELL ---\n");
    printf("\n\nYou are now using your shell within the exercise's directory.");
    printf("\nOnly use the commands needed for the exercise.\n\n");
    printf("Type your commands. Type 'exit' to return.\n\n");
    printf("If the command doesn't respond, use ctrl + c to stop it and try again.\n\n");
    system(getenv("SHELL") ? getenv("SHELL") : "/bin/sh");

    reset_prog_mode();
    refresh();
}

Exercise *run_exercise_list_and_select(int *selected_index)
{
    int ch;
    int top_index = last_top_index;
    int border_top = 4;
    int border_bottom = LINES - 5;
    int visible_spots = border_bottom - border_top - 1;
    int current_index = *selected_index;
    
    // Initial draw - draw everything once
    show_exercise_list_commentary(border_top, border_bottom);
    show_exercise_list_contents(exercises, border_top,
                                current_index, top_index, visible_spots);

    int needs_redraw = 0;
    while (1)
    {
        ch = getch();

        if (ch == KEY_UP || ch == 'w' || ch == 'W')
        {
            if (current_index > 0)
            {
                current_index--;
                needs_redraw = 1;
            }
            if (current_index < top_index)
            {
                top_index--;
                needs_redraw = 1;
            }
        }
        else if (ch == KEY_DOWN || ch == 's' || ch == 'S')
        {
            if (current_index < exercise_count - 1)
            {
                current_index++;
                needs_redraw = 1;
            }
            if (current_index >= top_index + visible_spots)
            {
                top_index++;
                needs_redraw = 1;
            }
        }
        else if (ch == '\n' || ch == KEY_ENTER)
        {
            *selected_index = current_index;
            last_top_index = top_index;
            return &exercises[current_index];
        }
        else if (ch == 127 || ch == KEY_BACKSPACE)
        {
            last_top_index = top_index;
            return NULL;
        }
        else if (ch == KEY_RESIZE)
        {
            border_bottom = LINES - 5;
            visible_spots = border_bottom - border_top - 1;
            show_exercise_list_commentary(border_top, border_bottom);
            show_exercise_list_contents(exercises, border_top,
                                        current_index, top_index, visible_spots);
        }

        // Only redraw what changed
        if (needs_redraw)
        {
            // Only redraw the contents, not the commentary
            show_exercise_list_contents(exercises, border_top,
                                        current_index, top_index, visible_spots);
        }
    }
}