#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <locale.h>
#include <getopt.h>
#include <curses.h>

#define MAX_COLUMN 80
#define MAX_COLUMNS 80
#define MAX_STREAMS 5
#define MAX_STREAM_LENGTH 20

#define MATRIX 0
#define COIN 1
#define SNAKE 2

#define WHITE 0
#define BLUE 1
#define RED 2
#define GREEN 3
#define YELLOW 4
#define MAGENTA 5
#define BLACK 6

#define DOLLAR 0
#define TL 1
#define EURO 2



struct Column {
    int istherenewline;     //calculated after last repeat
    int ongoing_streams; /*how many stream elements left on the ongoing stream*/
    int stream_wait;
};

struct Column columns[MAX_COLUMNS];


void swap_int(int *a, int *b);
void matrix(int color);
void coin(int currency);
void snake();
int main(const int argc, char *argv[]) {
    int animation = MATRIX;
    int color = WHITE;
    int currency = DOLLAR;
    int option;
    if (argc == 1) {
        matrix(color);
    }
    while ((option = getopt(argc, argv, "A:c:C:")) != EOF) {
        switch (option) {
            case 'A':
                if (!strcasecmp(optarg, "matrix")) {
                    animation = MATRIX;
                } else if (!strcasecmp(optarg, "coin")) {
                    animation = COIN;
                } else if (!strcasecmp(optarg, "snake")) {
                    animation = SNAKE;
                } else {
                    printf("Unknown animation: %s\n", optarg);
                    return 1;
                }
                break;
            case 'c':
                if (!strcasecmp(optarg, "dollar")) {
                    currency = DOLLAR;
                } else if (!strcasecmp(optarg, "tl")) {
                    currency = TL;
                } else if (!strcasecmp(optarg, "euro")) {
                    currency = EURO;
                } else {
                    printf("Unknown currency: %s\n", optarg);
                    return 1;
                }
                break;

            case 'C':
                if (!strcasecmp(optarg, "black")) {
                    color = BLACK;
                } else if (!strcasecmp(optarg, "white")) {
                    color = WHITE;
                } else if (!strcasecmp(optarg, "red")) {
                    color = RED;
                } else if (!strcasecmp(optarg, "green")) {
                    color = GREEN;
                } else if (!strcasecmp(optarg, "blue")) {
                    color = BLUE;
                } else if (!strcasecmp(optarg, "yellow")) {
                    color = YELLOW;
                } else if (!strcasecmp(optarg, "magenta")) {
                    color = MAGENTA;
                } else {
                    printf("you write a wrong color or a typo");
                    return 0;
                } break;
        }
    }
    if (animation == MATRIX) {
        matrix(color);
    }
    if (animation == COIN) {
        coin(currency);
    }
    if (animation == SNAKE) {
        snake();
    }
    return 0;
}
void swap_int(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void matrix(int color) {
    struct Column column[MAX_COLUMNS];
    initscr();
    memset(column, 0, sizeof(column));
    start_color();
    use_default_colors(); // arka plan için şart
    init_pair(GREEN, COLOR_GREEN, -1);
    init_pair(WHITE, COLOR_WHITE, -1);
    init_pair(BLUE, COLOR_BLUE, -1);
    init_pair(RED, COLOR_RED, -1);
    init_pair(YELLOW, COLOR_YELLOW, -1);
    init_pair(MAGENTA, COLOR_MAGENTA, -1);
    init_pair(BLACK, COLOR_BLACK, -1);




    while (1) {
        int rows, cols;
        getmaxyx(stdscr, rows, cols);

        for (int i = 0; i < cols; i += 2) {
            float printed_characternumber = 0;
            int a = ' ';
            for (int j = 0; j < rows; j++) {
                if (column[i].istherenewline) {
                    column[i].ongoing_streams =10 + rand() % (MAX_STREAM_LENGTH - 10);
                    column[i].istherenewline = 0;
                    a = 33 + rand() % (126 - 33 + 1);
                    attron(COLOR_PAIR(WHITE));   // 1 numaralı renk çifti aktif
                    mvaddch(j, i, a);      // karakteri yaz
                    attroff(COLOR_PAIR(WHITE));// rengi kapat
                    column[i].ongoing_streams--;
                    continue;
                }
                if (column[i].ongoing_streams != 0 && j == 0) {
                    a = 33 + rand() % (126 - 33 + 1);
                    attron(COLOR_PAIR(color));   // 1 numaralı renk çifti aktif
                    mvaddch(j, i, a);      // karakteri yaz
                    attroff(COLOR_PAIR(color));  // rengi kapat
                    column[i].ongoing_streams--;
                    printed_characternumber++;
                    continue;
                } else if (j == 0) {
                    attron(COLOR_PAIR(color));   // 1 numaralı renk çifti aktif
                    mvaddch(j, i, ' ');      // karakteri yaz
                    attroff(COLOR_PAIR(color));  // rengi kapat
                    printed_characternumber++;
                    continue;
                }
                chtype curr = mvinch(j, i) & A_CHARTEXT;
                chtype prev = mvinch(j-1, i) & A_CHARTEXT;

                if (prev == ' ' && curr == ' ') {
                }else if (prev == ' ' ){
                    a = mvinch(j, i) & A_CHARTEXT;

                    attron(COLOR_PAIR(color));   // 1 numaralı renk çifti aktif
                    mvaddch(j, i, ' ');      // karakteri yaz
                    attroff(COLOR_PAIR(color));  // rengi kapat
                    printed_characternumber++;
                    j++;
                    int b = mvinch(j, i) & A_CHARTEXT;
                    attron(COLOR_PAIR(color));   // 1 numaralı renk çifti aktif
                    mvaddch(j, i, a);      // karakteri yaz
                    attroff(COLOR_PAIR(color));
                    a = b;
                }else if (curr == ' ' ) {
                    if (a == ' ') {
                        attron(COLOR_PAIR(color));   // 1 numaralı renk çifti aktif
                        mvaddch(j, i, ' ');      // karakteri yaz
                        attroff(COLOR_PAIR(color));
                        j;
                    } else {
                        attron(COLOR_PAIR(WHITE));   // 1 numaralı renk çifti aktif
                        mvaddch(j, i, a);      // karakteri yaz
                        attroff(COLOR_PAIR(WHITE));  // rengi kapat

                        printed_characternumber++;
                        a = ' ';
                    }
                }else {
                    int b = mvinch(j,i) & A_CHARTEXT;

                    attron(COLOR_PAIR(color));   // 1 numaralı renk çifti aktif
                    mvaddch(j, i, a);      // karakteri yaz
                    attroff(COLOR_PAIR(color));  // rengi kapat

                    printed_characternumber++;
                    a = b;
                }


                if (column[i].ongoing_streams == 0) {
                    if (column[i].stream_wait == 0) {
                        column[i].istherenewline = 1;
                        
                    } else {
                        column[i].stream_wait--;
                    }

                    } else {
                        column[i].stream_wait = rand() % 20;

                    }

            }

        }
        refresh();
        napms(100);
    }
}
void coin(int currency) {}
void snake(void) {} 