#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "cJSON.h"

// Function to load JSON from a file
char* load_json(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* buffer = (char*)malloc(length + 1);
    if (buffer) {
        size_t read_size = fread(buffer, 1, length, file);
        if (read_size != length) {
            fprintf(stderr, "Error: Could not read the entire file, expected %ld bytes, but got %zu bytes.\n", length, read_size);
            free(buffer);
            fclose(file);
            return NULL;
        }
        buffer[length] = '\0';
    }
    fclose(file);
    return buffer;
}

// Function to shuffle the answers
void shuffle_answers(char** answers, int n) {
    srand(time(NULL));  // Seeding random generator
    for (int i = 0; i < n; i++) {
        int j = rand() % n;
        char* temp = answers[i];
        answers[i] = answers[j];
        answers[j] = temp;
    }
}

// Function to shuffle the questions
void shuffle_questions(cJSON* questions, int total_questions) {
    srand(time(NULL));  // Seeding random generator
    for (int i = 0; i < total_questions; i++) {
        int j = rand() % total_questions;
        
        // Swap the questions in the array (by value, not by reference)
        cJSON* temp = cJSON_GetArrayItem(questions, i);
        cJSON* other = cJSON_GetArrayItem(questions, j);

        // We need to use cJSON's internal structure to copy the question
        cJSON* temp_copy = cJSON_Duplicate(temp, 1);
        cJSON* other_copy = cJSON_Duplicate(other, 1);

        cJSON_ReplaceItemInArray(questions, i, other_copy);
        cJSON_ReplaceItemInArray(questions, j, temp_copy);
    }
}

// Function to get the letter index from user input
int get_answer_index(char answer) {
    return toupper(answer) - 'A';
}


void run_quiz(cJSON* questions) {
    int score = 0;
    int total_questions = cJSON_GetArraySize(questions);
    char* answer_letters[] = {"A", "B", "C", "D"};

    // Shuffle the questions first
    shuffle_questions(questions, total_questions);

    for (int i = 0; i < total_questions; i++) {
        cJSON* question_item = cJSON_GetArrayItem(questions, i);
        
        // Get the question text
        const char* question_text = cJSON_GetObjectItem(question_item, "question")->valuestring;
        cJSON* answers = cJSON_GetObjectItem(question_item, "answers");
        int num_answers = cJSON_GetArraySize(answers);
        
        if (num_answers <= 0) {
            printf("Error: No answers available for question %d\n", i);
            continue;
        }
        
        char* answer_options[num_answers];
        printf("\n%s\n", question_text);

        // Get answers from JSON and shuffle them
        for (int j = 0; j < num_answers; j++) {
            answer_options[j] = cJSON_GetArrayItem(answers, j)->valuestring;
        }

        // Shuffle the answers
        shuffle_answers(answer_options, num_answers);

        // Display shuffled answers with their corresponding letters
        for (int j = 0; j < num_answers; j++) {
            printf("%s. %s\n", answer_letters[j], answer_options[j]);
        }

        // Get user input
        char user_input;
        printf("Enter the letter of your answer: ");
        if (scanf(" %c", &user_input) != 1) {
            printf("Error: Invalid input\n");
            continue;
        }

        // Check if the answer is correct
        int user_answer_index = get_answer_index(user_input);
        if (user_answer_index >= 0 && user_answer_index < num_answers) {
            char* correct_answer = cJSON_GetObjectItem(question_item, "right_answer")->valuestring;
            if (strcmp(answer_options[user_answer_index], correct_answer) == 0) {
                printf("Correct!\n");
                score++;
            } else {
                printf("Wrong! The correct answer is: %s\n", correct_answer);
            }
        } else {
            printf("Invalid input. Please enter a valid letter (A, B, C, D).\n");
        }
    }

    // Print final score
    printf("\nYour final score is: %d out of %d\n", score, total_questions);
}

int main() {
    const char* filename = "./data/questions.json";  // Path to your JSON file
    
    char* json_data = load_json(filename);
    if (json_data == NULL) {
        return 1;
    }
    
    // Parse the JSON data and ensure that the buffer is freed after parsing
    cJSON* questions = cJSON_Parse(json_data);
    if (questions == NULL) {
        printf("Error parsing JSON data\n");
        free(json_data);  // Free json_data here since it has been parsed
        return 1;
    }

    printf("Loaded and parsed JSON successfully!\n");

    // Do not free json_data here yet, because cJSON_Parse needs it until the JSON is parsed.
    run_quiz(questions);

    // After using questions, free the json_data and cJSON object correctly.
    free(json_data);  // Free the raw JSON data after parsing
    cJSON_Delete(questions);  // Free the cJSON object

    return 0;
}
