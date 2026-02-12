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
    int stream1length;     //calculated after last repeat
    int stream1head; /*how many stream elements left on the ongoing stream*/
    int stream2length;
    int stream2head;
    int stream1highlight;
    int stream2highlight;
};

struct Column columns[MAX_COLUMNS];


void swap_int(int *a, int *b);
void matrix(int color);
void coin(int currency);
void snake();
int main(const int argc, char *argv[]) {
    int animation = MATRIX;
    int color = GREEN;
    int currency = DOLLAR;
    int option;

    srand(time(NULL));

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
    //for future changes i can erase the if j < 0 function because in the long run it could be expensive
    // mvaddch(j, i, a | COLOR_PAIR(color));could be done
    struct Column column[MAX_COLUMNS];
    initscr();
    start_color();
    use_default_colors(); // arka plan için şart
    init_pair(GREEN, COLOR_GREEN, -1);
    init_pair(WHITE, COLOR_WHITE, -1);
    init_pair(BLUE, COLOR_BLUE, -1);
    init_pair(RED, COLOR_RED, -1);
    init_pair(YELLOW, COLOR_YELLOW, -1);
    init_pair(MAGENTA, COLOR_MAGENTA, -1);
    init_pair(BLACK, COLOR_BLACK, -1);
for(int i = 0; i < MAX_COLUMNS ; i++){
        column[i].stream1head =(-50) + (rand() % 50);
        column[i].stream1length = 5 + (rand() % 35);
        column[i].stream1highlight = 1;
        column[i].stream2head = column[i].stream1head -column[i].stream1length - (10 + (rand() % 40));
        column[i].stream2length = 5 + (rand() % 35);
        column[i].stream2highlight = 1;
}


    while (1) {
            int rows, cols, i, j, a, b;
            getmaxyx(stdscr, rows, cols);
            for (i = 0; i < cols; i += 2) {
                column[i].stream1head = -30 + (rand() % 30);
                column[i].stream1length = 5 + (rand() % 35);
                column[i].stream1highlight = 1;
                column[i].stream2head = column[i].stream1head - ((rand() % 10) + 20);  
                column[i].stream2length = 5 + (rand() % 35);
                column[i].stream2highlight = 1;          
            }
            // important !! when creating new stream values be carefull about its head not beign on the screen because we dont want it to appear randomly on the screen
            for (i = 0; i < cols; i += 2) {

                    if(column[i].stream1length == 0 || column[i].stream2length == 0){
                    //find the 0 one and pick rand value for its head and its length then make its highlight 1
                            if(column[i].stream1length == 0){
                                    column[i].stream1highlight = 1;
                                    column[i].stream1length = 5 + (rand() % 35);
                                    if(column[i].stream2head < 0){
                                            column[i].stream1head = column[i].stream2head - ((rand() % 10) + 20);            
                                    } else{
                                            column[i].stream1head = -30 + (rand() % 30);
                                    }
                            } else {
                                    column[i].stream2highlight = 1;
                                    column[i].stream2length = 5 + (rand() % 35);
                                    if(column[i].stream1head < 0){
                                            column[i].stream2head = column[i].stream1head - ((rand() % 10) + 20);            
                                    } else{
                                            column[i].stream2head = -30 + (rand() % 30);
                                    }
                                }
                    }

                    
                    if(column[i].stream1head > column[i].stream2head){
                            if(column[i].stream1head < 0){
                                column[i].stream1head++;
                                column[i].stream2head++;
                            } else if(column[i].stream2head < 0){
                                // print the stream1 and plus 1 both of these functions headslook if stream 1 is fully printed from the start of the screen if not create new number
                                a = ' ';
                                for(j = column[i].stream1head - column[i].stream1length +1 ; j < column[i].stream1head; j++){
                                        if(j < 0){
                                            j = 0;
                                            a = (33 + rand() % 94);
                                        }
                                        b = mvinch(j,i);
                                        mvaddch(j, i, a | COLOR_PAIR(color));
                                        a = b;
                                        
                                    //open white color than close it and print the last line
                                }
                                if(column[i].stream1highlight){
                                        mvaddch(j, i, a | COLOR_PAIR(WHITE));
                                } else  {
                                    mvaddch(j, i, a | COLOR_PAIR(color));
                                }
                                column[i].stream1head++;
                                }
                                
                             else {
                                // print both streams calculate if the stream 2 is fully printed if not then create new number
                                // check if stream 1 is on the end of rows if is then do not plus 1 the heads but -1 its length and make its highligt 0
                                // if it reaches 0 the function will create new one with stream2's values 
                                a = ' ';
                                for(j = column[i].stream1head - column[i].stream1length +1 ; j < column[i].stream1head; j++){
                                        b = mvinch(j,i);
                                        mvaddch(j, i, a | COLOR_PAIR(color));
                                        a = b;
                                        
                                    //open white color than close it and print the last line
                                }
                                if(column[i].stream1highlight){
                                        mvaddch(j, i, a | COLOR_PAIR(WHITE));
                                } else  {
                                    mvaddch(j, i, a | COLOR_PAIR(color));
                                }
                                column[i].stream1head++;

                                a = ' ';
                                for(j = column[i].stream2head - column[i].stream2length +1 ; j < column[i].stream2head; j++){
                                        if(j < 0){
                                            j = 0;
                                            a = (33 + rand() % 94);
                                        }
                                        b = mvinch(j,i);
                                        mvaddch(j, i, a | COLOR_PAIR(color));
                                        a = b;
                                        
                                    //open white color than close it and print the last line
                                }
                                if(column[i].stream2highlight){
                                        mvaddch(j, i, a | COLOR_PAIR(WHITE));
                                } else  {
                                    mvaddch(j, i, a | COLOR_PAIR(color));
                                }
                                column[i].stream2head++;
                                }

                            }
                        

                     else {
                            if (column[i].stream2head < 0) {
                            column[i].stream2head++;
                            column[i].stream1head++;
                            } else if (column[i].stream1head < 0) {
                            a = ' ';
                                    for (j = column[i].stream2head - column[i].stream2length + 1; j < column[i].stream2head; j++) {
                                            if (j < 0) {
                                            j = 0;
                                            a = (33 + rand() % 94);
                                            }
                                            b = mvinch(j, i);
                                            mvaddch(j, i, a | COLOR_PAIR(color));
                                            a = b;
                                            }
                            if (column[i].stream2highlight) {
                            mvaddch(j, i, a | COLOR_PAIR(WHITE));
                            } else {
                            mvaddch(j, i, a | COLOR_PAIR(color));
                            }
                            column[i].stream2head++;
                            } else {
                             /* --- draw stream2 --- */
                            a = ' ';
                            for (j = column[i].stream2head - column[i].stream2length + 1; j < column[i].stream2head; j++) {
                            b = mvinch(j, i);
                            mvaddch(j, i, a | COLOR_PAIR(color));
                            a = b;
                            }
                            if (column[i].stream2highlight) {
                            mvaddch(j, i, a | COLOR_PAIR(WHITE));
                            } else {
                            mvaddch(j, i, a | COLOR_PAIR(color));
                            }
                        column[i].stream2head++;

                        /* --- draw stream1 --- */
                        a = ' ';
                        for (j = column[i].stream1head - column[i].stream1length + 1; j < column[i].stream1head; j++) {
                        if (j < 0) {
                        j = 0;
                        a = (33 + rand() % 94);
                        }

                        b = mvinch(j, i);
                        mvaddch(j, i, a | COLOR_PAIR(color));
                        a = b;
                        }

                        if (column[i].stream1highlight) {
                          mvaddch(j, i, a | COLOR_PAIR(WHITE));
                        } else {
                            mvaddch(j, i, a | COLOR_PAIR(color));
                          }

                          column[i].stream1head++;
                        }
                        }


                           
                                if (column[i].stream1head == rows || column[i].stream2head == rows ){
                            if(column[i].stream1head == rows){
                                    column[i].stream1head--;
                                    column[i].stream1length--;
                                    column[i].stream1highlight = 0;
                            } else {
                                    column[i].stream2head--;
                                    column[i].stream2length--;
                                    column[i].stream2highlight = 0;
                            }
                        }
                    }
            
    

        refresh();
        napms(100);
    }
}
void coin(int currency) {}
void snake(void) {}   




/* here's the old function i wrote it doesnt work properly
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

*/