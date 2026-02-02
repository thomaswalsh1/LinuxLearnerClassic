#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include "../exercises.h"

typedef struct {
    Exercise *exercises;
    int count;
} ExerciseList;

typedef struct {
    StudySet *study_sets;
    int count;
} StudySetList;


/**
 * Loads all the exercises from directory with config files
 * @return ExerciseList comprised of all the .conf files in labs_reprise
 */
ExerciseList load_exercises_from_all(void);

/**
 * Load all exercises from config files in a directory
 * @param config_dir Directory containing .conf files
 * @return ExerciseList with all loaded exercises
 */
ExerciseList load_exercises_from_config(const char *config_dir);

/**
 * Extra code for expirementing with the exercise loading
 * @param config_dir Directory containing.conf files
 * @return ExerciseList with all loaded exercises
 */
ExerciseList test_exercises_from_config(const char *config_dir);

/**
 * Free the exercise list allocated by load_exercises_from_config
 * @param list The ExerciseList to free
 */
void free_exercise_list(ExerciseList list);

/**
 * Get a validator function pointer by name
 * @param validator_name Name of the validator (e.g., "file_exists")
 * @return Function pointer to the validator, or NULL if not found
 */
int (*get_validator_function(const char *validator_name))(void);

enum PersistentOption {
    MARK_EX_COMPLETE,
    MARK_EX_INCOMPLETE
};

/**
 * Modify a persistent attribute on disk for an exercise
 * @param option How you want to change this exercise
 */
void modify_exercise_data(Exercise *ex, const enum PersistentOption option);

StudySet get_default_study_set(void);
StudySet get_study_set_by_name(char *name);
StudySetList get_study_set_list(void);

#endif
