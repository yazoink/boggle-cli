#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/wait.h>

#define MAX 255
#define LETTERS 16
#define MAX_WORDS 512

void help(void);
char *gen_letters(void);
int get_num(int opt_num);
void play(void);
int rand_letter(void);
int verify_letters(char *letters, char *word);
int verify_word(char *word);
int verify_word_original(char **found_words, int found_words_num, char *word);

int main(void) {
    int num;
    while (1) {
        system("clear");
        num = get_num(3);

        switch(num) {
            case 1:
                play();
                break;
            case 2:
                help();
                break;
            case 3:
                return 0;
        }
    }
}

void help(void) {
    system("clear");
    printf("~~> W O R D  B O G G L E <~~\n\n");
    system("cat guide.txt");
    getchar();
    return;
}

char *gen_letters(void) {
    char *letters = (char *)malloc(LETTERS + 1);
    if (letters == NULL) {
        fprintf(stderr, "Error: memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < LETTERS; i++) {
        system("clear");
        printf("Generating letters (%d/%d)...\n", i + 1, LETTERS);
        do {
            letters[i] = rand_letter();
        } while(letters[i] == letters[i - 1]);
    }
    return letters;
}

int get_num(int opt_num) {
    int num;
    char buf[MAX];

    do {
        system("clear");
        printf("~~> W O R D  B O G G L E <~~\n"
                "\n"
                "Select a number:\n"
                "\t1) Play\n"
                "\t2) Help\n"
                "\t3) Exit\n"
                "\n"
                ">> ");
        fgets(buf, MAX, stdin);
    } while(buf[0] == '\n'); // while enter is pressed without input
    buf[strlen(buf) - 1] = '\0'; // replace newline with nul terminator

    if (strlen(buf) != 1) {
        return get_num(opt_num);
    } else {
        if (isdigit(buf[0])) {
            if (atoi(buf) <= opt_num && atoi(buf) >= 1) {
                return atoi(buf);
            } else {
                return get_num(opt_num);
            }
        } else {
            return get_num(opt_num);
        }
    }
}

void play(void) {
    char *letters, **found_words;
    time_t start_time, current_time;
    int score, len, found_words_num;
    FILE *f;
    
    score = 0;
    letters = gen_letters();
    start_time = time(NULL);
    found_words = malloc(sizeof(char *) * MAX_WORDS);
    if (found_words == NULL) {
        fprintf(stderr, "Error: memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    found_words_num = 0;

    while (1) {
        char buf[MAX];

        current_time = time(NULL);
        if (current_time - start_time >= 60) { // if game has run for over 60 seconds, stop
            break;
        }

        do {
            system("clear");
            printf("Find as many words as you can...\n");
            printf(" ___ ___ ___ ___ \n"
            "| %c | %c | %c | %c |\n"
            " --- --- --- --- \n"
            "| %c | %c | %c | %c |\n"
            " --- --- --- --- \n"
            "| %c | %c | %c | %c |\n"
            " --- --- --- --- \n"
            "| %c | %c | %c | %c |\n"
            " --- --- --- --- \n", letters[0], letters[1], letters[2], letters[3], letters[4], letters[5], letters[6], letters[7], letters[8], letters[9], letters[10], letters[11], letters[12], letters[13], letters[14], letters[15]);
            printf("Score: %d\n\n", score);
            printf(">> ");
            fgets(buf, MAX, stdin);
        } while(buf[0] == '\n');

        buf[strlen(buf) - 1] = '\0';

        for (int i = 0; i < strlen(buf); i++) { // capitalize all letters to ensure case insensitivity
            buf[i] = toupper(buf[i]);
        }

        if (verify_letters(letters, buf) == EXIT_SUCCESS) { // ensure all letters in word are in grid
            if ((len = verify_word(buf)) != EXIT_FAILURE) { // ensure word is in word list
                if (verify_word_original(found_words, found_words_num, buf) == EXIT_SUCCESS) { // ensure word has not been entered before
                    found_words[found_words_num] = malloc(sizeof(buf) + 1); // allocate memory for word in found words array
                    if (found_words[found_words_num] == NULL) {
                        fprintf(stderr, "Error: memory allocation failed.\n");
                        exit(EXIT_FAILURE);
                    }
                    strcpy(found_words[found_words_num], buf);
                    found_words_num++;
                    score = score + len;
                }
            }
        }
    }
    for (int i = 0; i < found_words_num; i++) {
        free(found_words[i]);
    }
    free(found_words);
    free(letters);

    system("clear");

    f = fopen("scores.txt", "w");
    fprintf(f, "%d\n", score); // add score to text file
    fclose(f);

    printf("Time is up!\n\nYour score was: %d\nPress any key to continue...\n", score);
    getchar();
}

int rand_letter(void) {
    int num;
    srand(time(NULL)); // initialize
    num = rand() % 26 + 65; // pick random number between 65 and 90 (ascii decimals for alphabet)
    return num;
}

int verify_letters(char *letters, char *word) {
    char *letters_copy = (char *)malloc(sizeof(letters)); // copy of letters array to be modified
    if (letters_copy == NULL) {
        fprintf(stderr, "Error: memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(letters_copy, letters);
    for (int i = 0; i < strlen(word); i++) { // for letters in word
        int found = 0;
        for (int j = 0; j < LETTERS; j++) { // for letters in grid
            if (word[i] == letters_copy[j]) {
                found = 1;
                letters_copy[j] = 0;
                continue;
            }
        }
        if (found == 0) {
            free(letters_copy);
            return EXIT_FAILURE;
        }
    }
    free(letters_copy);
    return EXIT_SUCCESS;
}

int verify_word(char *word) {
    int status, exit_status;
    char cmd[MAX];
    snprintf(cmd, MAX, "grep -i -w -q %s wordlist.txt", word);
    status = system(cmd);
    if (status == -1) {
        fprintf(stderr, "Error: could not execute grep.\n");
        exit(EXIT_FAILURE);
    } else if (WIFEXITED(status)) {
        exit_status = WEXITSTATUS(status);
        if (exit_status == 0) {
            return strlen(word); // if execution is a success, function returns length of the word
        } else {
            return EXIT_FAILURE;
        }
    } else {
        fprintf(stderr, "Error: grep returned unexpected exit code.\n");
        exit(EXIT_FAILURE);
    }
}

int verify_word_original(char **found_words, int found_words_num, char *word) {
    for (int i = 0; i < found_words_num; i++) {
        if (strcmp(found_words[i], word) == 0) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
