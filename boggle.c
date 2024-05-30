#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX 255
#define LETTERS 16

void help(void);
char *gen_letters(void);
int get_num(int opt_num);
void play(void);
int rand_letter(void);

int main(void) {
    int num;
    while (1) {
        system("clear");
        printf("~~> W O R D  B O G G L E <~~\n"
                "\n"
                "Select a number:\n"
                "\t1) Play\n"
                "\t2) Help\n"
                "\t3) Exit\n"
                "\n");
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
        printf(">> ");
        fgets(buf, MAX, stdin);
    } while(buf[0] == '\n');
    buf[strlen(buf) - 1] = '\0';

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
    char *letters;
    time_t start_time, current_time;
    int score = 0;

    letters = gen_letters();

    start_time = time(NULL);

    while (1) {
        char buf[MAX];
        current_time = time(NULL);
        if (current_time - start_time >= 60) {
            break;
        }

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
    }
    free(letters);
    exit(0);
}

int rand_letter(void) {
    int num;
    srand(time(NULL));
    num = rand() % 26 + 65;
    return num;
}
