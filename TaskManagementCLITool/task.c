#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_DESCRIPTION_LENGTH 100
#define MAX_TASKS 100

struct Task {
    char description[MAX_DESCRIPTION_LENGTH];
    bool completed;
};

void displayTasks(const struct Task* tasks, size_t numTasks) {
    printf("Task List:\n");
    size_t i;
    for (i = 0; i < numTasks; ++i) {
        printf("[%zu] %s\n", i + 1, tasks[i].completed ? "[Completed]" : "[Pending]");
        printf("%s\n", tasks[i].description);
    }
}

void addTask(struct Task* tasks, size_t* numTasks, const char* description) {
    if (*numTasks < MAX_TASKS) {
        struct Task newTask;
        strncpy(newTask.description, description, MAX_DESCRIPTION_LENGTH - 1);
        newTask.description[MAX_DESCRIPTION_LENGTH - 1] = '\0';
        newTask.completed = false;
        tasks[(*numTasks)++] = newTask;
    } else {
        printf("Task list is full.\n");
    }
}

void editTask(struct Task* tasks, size_t numTasks, size_t taskIndex, const char* newDescription) {
    if (taskIndex < numTasks) {
        strncpy(tasks[taskIndex].description, newDescription, MAX_DESCRIPTION_LENGTH - 1);
        tasks[taskIndex].description[MAX_DESCRIPTION_LENGTH - 1] = '\0';
    } else {
        printf("Invalid task index.\n");
    }
}

void completeTask(struct Task* tasks, size_t numTasks, size_t taskIndex) {
    if (taskIndex < numTasks) {
        tasks[taskIndex].completed = true;
    } else {
        printf("Invalid task index.\n");
    }
}

void saveTasks(const struct Task* tasks, size_t numTasks, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file != NULL) {
        size_t i;
        for (i = 0; i < numTasks; ++i) {
            fprintf(file, "%d %s\n", tasks[i].completed, tasks[i].description);
        }
        fclose(file);
    } else {
        printf("Error: Unable to save tasks to file.\n");
    }
}

void loadTasks(struct Task* tasks, size_t* numTasks, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        *numTasks = 0;
        while ((*numTasks) < MAX_TASKS && fscanf(file, "%d ", &tasks[*numTasks].completed) != EOF) {
            fgets(tasks[*numTasks].description, MAX_DESCRIPTION_LENGTH, file);
            tasks[(*numTasks)++].description[strcspn(tasks[(*numTasks) - 1].description, "\n")] = '\0';
        }
        fclose(file);
    } else {
        printf("Error: Unable to load tasks from file.\n");
    }
}

int main() {
    struct Task tasks[MAX_TASKS];
    size_t numTasks = 0;
    const char* filename = "task_history.txt";
    loadTasks(tasks, &numTasks, filename);

    while (1) {
        printf("\nOptions:\n");
        printf("1. Display Tasks\n");
        printf("2. Add Task\n");
        printf("3. Edit Task\n");
        printf("4. Complete Task\n");
        printf("5. Save and Quit\n");

        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                displayTasks(tasks, numTasks);
                break;

            case 2:
                {
                    char description[MAX_DESCRIPTION_LENGTH];
                    printf("Enter task description: ");
                    fgets(description, MAX_DESCRIPTION_LENGTH, stdin);
                    description[strcspn(description, "\n")] = '\0';
                    addTask(tasks, &numTasks, description);
                    break;
                }

            case 3:
                {
                    size_t taskIndex;
                    char newDescription[MAX_DESCRIPTION_LENGTH];
                    printf("Enter task index to edit: ");
                    scanf("%zu", &taskIndex);
                    getchar();
                    printf("Enter new task description: ");
                    fgets(newDescription, MAX_DESCRIPTION_LENGTH, stdin);
                    newDescription[strcspn(newDescription, "\n")] = '\0';
                    editTask(tasks, numTasks, taskIndex - 1, newDescription);
                    break;
                }

            case 4:
                {
                    size_t taskIndex;
                    printf("Enter task index to mark as completed: ");
                    scanf("%zu", &taskIndex);
                    getchar();
                    completeTask(tasks, numTasks, taskIndex - 1);
                    break;
                }

            case 5:
                saveTasks(tasks, numTasks, filename);
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
