#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include<math.h>
#include<sys/ioctl.h>

#define cursorup(x)    printf("\033[%dA", x);
#define cursordown(x)  printf("\033[%dB", x);
#define cursorright(x) printf("\033[%dC", x);
#define cursorleft(x)  printf("\033[%dD", x);

#define CBORDER printf("\033[0;30;41m");
#define C2      printf("\033[0;30;42m");
#define C4      printf("\033[0;30;43m");
#define C8      printf("\033[0;30;44m");
#define C16     printf("\033[0;30;45m");
#define C32     printf("\033[0;30;46m");
#define C64     printf("\033[0;30;47m");
#define C128    printf("\033[0;30;42m");
#define C256    printf("\033[0;30;43m");
#define C512    printf("\033[0;30;44m");
#define C1024   printf("\033[0;30;45m");
#define C2048   printf("\033[0;30;46m");
#define CNORMAL printf("\033[0;0;0m");

void drawfield(int [][4], int, int, int, int);
void addfield(int [][4], int, int);
int  slide(int [][4], int, int, int*);
void rotate(int [][4], int, int, int);

void initwin();
void endwin();

void
drawfield(int f[][4], int w, int h, int sx, int sy)
{
    int cs = 7;

    cursorup(sy);
    cursorleft(sx);

    CBORDER;
    cursordown(sy / 2 - (4 * w + 1) / 2);
    cursorright(sx / 2 - ((cs + 1) * w + 1) / 2);
    printf("\u250C");
    for(int j = 1; j < w * (cs + 1) + 1 - 1; j++)
        printf(j % (cs + 1) ? "\u2500" : "\u252C");
    printf("\u2510");

    for(int i = 0; i < h; i++) {
        printf("\n\r");
        cursorright(sx / 2 - ((cs + 1) * w + 1) / 2);
        for(int j = 0; j < w * (cs + 1) + 1; j++) {
            switch(f[i][j / (cs + 1)]) {
            case 2: C2; break;
            case 4: C4; break;
            case 8: C8; break;
            case 16: C16; break;
            case 32: C32; break;
            case 64: C64; break;
            case 128: C128; break;
            case 256: C256; break;
            case 512: C512; break;
            case 1024: C1024; break;
            case 2048: C2048; break;
            }
            if(j % (cs + 1))
                printf(" ");
            else {
                CBORDER;
                printf("\u2502");
            }
            CBORDER;
        }
        printf("\n\r");
        cursorright(sx / 2 - ((cs + 1) * w + 1) / 2);
        for(int j = 0; j < w; j++) {
            printf("\u2502");
            switch(f[i][j]) {
            case 2: C2; break;
            case 4: C4; break;
            case 8: C8; break;
            case 16: C16; break;
            case 32: C32; break;
            case 64: C64; break;
            case 128: C128; break;
            case 256: C256; break;
            case 512: C512; break;
            case 1024: C1024; break;
            case 2048: C2048; break;
            }
            printf(" %5d ", f[i][j]);
            CBORDER;
        }
        printf("\u2502\n\r");
        cursorright(sx / 2 - ((cs + 1) * w + 1) / 2);

        for(int j = 0; j < w * (cs + 1) + 1; j++) {
            switch(f[i][j / (cs + 1)]) {
            case 2: C2; break;
            case 4: C4; break;
            case 8: C8; break;
            case 16: C16; break;
            case 32: C32; break;
            case 64: C64; break;
            case 128: C128; break;
            case 256: C256; break;
            case 512: C512; break;
            case 1024: C1024; break;
            case 2048: C2048; break;
            }
            if(j % (cs + 1))
                printf(" ");
            else {
                CBORDER;
                printf("\u2502");
            }
            CBORDER;
        }

        printf("\n\r");
        cursorright(sx / 2 - ((cs + 1) * w + 1) / 2);
        printf("\u251C");
        for(int j = 1; j < w * (cs + 1) + 1 - 1; j++)
            printf(j % (cs + 1) ? "\u2500" : "\u253C");
        printf("\u2524");
    }

    cursorleft(sx);
    cursorright(sx / 2 - ((cs + 1) * w + 1) / 2);
    printf("\u2514");
    for(int j = 1; j < w * (cs + 1) + 1 - 1; j++)
        printf(j % (cs + 1) ? "\u2500" : "\u2534");
    printf("\u2518\n\r");

    CNORMAL;
}

void
addtile(int f[][4], int w, int h)
{
    int rx = 0;
    int ry = 0;
    do {
        rx = rand() % h;
        ry = rand() % w;
    } while(f[rx][ry] != 0);
    f[rx][ry] = rand() % 2 ? 2 : 4;
}

int
slide(int f[][4], int w, int h, int *score)
{
    int slided = 0;
    for(int i = h - 1; i > 0; i--) {
        for(int j = 0; j < w; j++) {
            if(!f[i - 1][j]) {
                f[i - 1][j] = f[i][j];
                f[i][j] = 0;
            }
        }
    }

    for(int i = 0; i < h; i++) {
        for(int j = 0; j < w; j++) {
            if(f[i][j] == f[i + 1][j]){
                f[i][j] *= 2;
                *score += f[i][j];
                f[i + 1][j] = 0;
                slided = 1;
            }
        }
    }

    for(int i = h - 1; i > 0; i--) {
        for(int j = 0; j < w; j++) {
            if(!f[i - 1][j]) {
                f[i - 1][j] = f[i][j];
                f[i][j] = 0;
                slided = 1;
            }
        }
    }
    return slided;
}

void
rotate(int m[][4], int w, int h, int a)
{
    int c[w][h];
    a /= 90;
    for(;a > 0; a--){
        for(int i = 0; i < h; i++)
            for(int j = 0; j < w; j++)
                c[i][j] = m[j][w - i - 1];

        for(int i = 0; i < h; i++)
            for(int j = 0; j < w; j++)
                m[i][j] = c[i][j];
    }
}

void
initwin()
{
    system("clear");
    system("stty raw");
    system("stty -echo");
    printf("\033[?25l");
}

void
endwin()
{
    system("clear");
    system("stty cooked");
    system("stty echo");
    printf("\033[?25h");
}

int
main()
{
    srand(time(NULL));
   struct winsize win;
    ioctl(0, TIOCGWINSZ, &win);

    int score = 0;

    int field[4][4] = {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    };

    initwin();
    addtile(field, 4, 4);
    addtile(field, 4, 4);
    drawfield(field, 4, 4, win.ws_col, win.ws_row);
    while(1){
        printf("score: %d", score);
        drawfield(field, 4, 4, win.ws_col, win.ws_row);
        switch(getchar()){
        case '\033':
            getchar();
            switch(getchar()) {
            case 'A':
                if(slide(field, 4, 4, &score))
                    addtile(field, 4, 4);
                break;
            case 'B':
                rotate(field, 4, 4, 180);
                if(slide(field, 4, 4, &score))
                    addtile(field, 4, 4);
                rotate(field, 4, 4, 180);
                break;
            case 'C':
                rotate(field, 4, 4, 90);
                if(slide(field, 4, 4, &score))
                    addtile(field, 4, 4);
                rotate(field, 4, 4, 270);
                break;
            case 'D':
                rotate(field, 4, 4, 270);
                if(slide(field, 4, 4, &score))
                    addtile(field, 4, 4);
                rotate(field, 4, 4, 90);
                break;
            }
            break;

            case 'd':
                rotate(field, 4, 4, 270);
                if(slide(field, 4, 4, &score))
                    addtile(field, 4, 4);
                rotate(field, 4, 4, 90);
                break;

            case 'h':
                rotate(field, 4, 4, 180);
                if(slide(field, 4, 4, &score))
                    addtile(field, 4, 4);
                rotate(field, 4, 4, 180);
                break;

            case 't':
                if(slide(field, 4, 4, &score))
                    addtile(field, 4, 4);
                break;

            case 'n':
                rotate(field, 4, 4, 90);
                if(slide(field, 4, 4, &score))
                    addtile(field, 4, 4);
                rotate(field, 4, 4, 270);
                break;

            case 'q':
                endwin();
                return 0;
                break;
        }
    }
}
