#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include<sys/ioctl.h>

#define AUTOSIZE
#define DVORAK

#define KNRM printf("\033[0m")
#define KRED printf("\033[31m")
#define KGRN printf("\033[32m")
#define KBLU printf("\033[34m")
#define KMAG printf("\033[35m")
#define KCYN printf("\033[36m")

#define KSEL printf("\033[43m")
#define KFLG printf("\033[42m")

#define cursorup(x)    printf("\033[%dA", x);
#define cursordown(x)  printf("\033[%dB", x);
#define cursorright(x) printf("\033[%dC", x);
#define cursorleft(x)  printf("\033[%dD", x);

#define ADDMINE(f, x, y, w, h)                                                 \
    if(x < h && y < w && x >= 0 && y >= 0 && f[x][y] != -1)                    \
        f[x][y] += 1;

#define DIGFIELD(f, m, w, h, x, y)                                             \
    if(x < h && y < w && x >= 0 && y >= 0)                                     \
        digfield(f, m, w, h, x, y);

void generatefield(int **, int, int, int, int, int);
void drawfield(int **, int **, int, int, int, int);
void redrawfield(int **, int **, int, int, int, int, int, int);
int  digfield(int **, int **, int, int, int, int);
void revealfield(int **, int **, int, int);
void toggleflag(int **, int, int);

void initwin();
void endwin();

void
generatefield(int **f, int w, int h, int k, int cx, int cy)
{
    int rx = 0;
    int ry = 0;
    for(;k > 0; k--) {
        do {
        do { rx = rand() % h; } while(rx == cx);
        do { ry = rand() % w; } while(ry == cy);
        } while(f[rx][ry] == -1);
        if(f[rx][ry] != -1) f[rx][ry] = -1;
        ADDMINE(f, rx + 1, ry + 1, w, h);
        ADDMINE(f, rx + 1, ry    , w, h);
        ADDMINE(f, rx + 1, ry - 1, w, h);
        ADDMINE(f, rx    , ry + 1, w, h);
        ADDMINE(f, rx    , ry - 1, w, h);
        ADDMINE(f, rx - 1, ry + 1, w, h);
        ADDMINE(f, rx - 1, ry    , w, h);
        ADDMINE(f, rx - 1, ry - 1, w, h);
    }
}

void
drawfield(int **f, int **m, int w, int h, int sx, int sy)
{
    cursorup(h);
    for(int i = 0; i < h; i++) {
        for(int j = 0; j < w; j++) {
            if(i == sx && j == sy)
                KSEL;

            switch(m[i][j]){
            case 1:
                switch(f[i][j]){
                case 1: KBLU; break;
                case 2: KGRN; break;
                case 3: KRED; break;
                case 4: KCYN; break;
                case 5: KMAG; break;
                }
                putchar(f[i][j] ? f[i][j] + 0x30 : ' ');
                break;
            case 2:
                KFLG;
                putchar(' ');
                break;
            case 3:
                putchar('*');
                break;
            default:
                putchar('#');
                break;
            }
            KNRM;
        }
        putchar('\n');
        putchar('\r');
    }
}

void
redrawfield(int **f, int **m, int w, int h, int x, int y, int sx, int sy)
{
    cursorup(h);
    cursorleft(w);
    cursordown(x);
    cursorright(y + 1);
    putchar('\b');
    if(x == sx && y == sy)
        KSEL;
    switch(m[x][y]){
    case 1:
        switch(f[x][y]){
        case 1: KBLU; break;
        case 2: KGRN; break;
        case 3: KRED; break;
        case 4: KCYN; break;
        case 5: KMAG; break;
        }
        putchar(f[x][y] ? f[x][y] + 0x30 : ' ');
        break;
    case 2:
        KFLG;
        putchar(' ');
        break;
    case 3:
        putchar('*');
        break;
    default:
        putchar('#');
        break;
    }
    KNRM;
    return;
}

int
digfield(int **f, int **m, int w, int h, int x, int y)
{
    if(x > h - 1 || y > w - 1) fprintf(stderr, "index out of range");

    if(f[x][y] == -1) return 1;
    if(m[x][y] == 2)  return 0;
    if(m[x][y])       return 0;

    m[x][y] = 1;

    if(!f[x][y]) {
        DIGFIELD(f, m, w, h, x + 1, y + 1);
        DIGFIELD(f, m, w, h, x + 1, y    );
        DIGFIELD(f, m, w, h, x + 1, y - 1);
        DIGFIELD(f, m, w, h, x    , y + 1);
        DIGFIELD(f, m, w, h, x    , y - 1);
        DIGFIELD(f, m, w, h, x - 1, y + 1);
        DIGFIELD(f, m, w, h, x - 1, y    );
        DIGFIELD(f, m, w, h, x - 1, y - 1);
    }

    return 0;
}

void
revealfield(int **f, int **m, int w, int h)
{
    for(int i = 0; i < h; i++)
        for(int j = 0; j < w; j++)
            if(f[i][j] == -1)
                m[i][j] = 3;
}

void
toggleflag(int **m, int x, int y)
{
    if(m[x][y] == 2)      m[x][y] = 0;
    else if(m[x][y] == 0) m[x][y] = 2;
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
    initwin();

    int diff = 5;

#ifdef AUTOSIZE
    struct winsize win;
    ioctl(0, TIOCGWINSZ, &win);
    int w = win.ws_col;
    int h = win.ws_row - 1;
#else
    int w = 80;
    int h = 20;
#endif

    int **field = malloc(sizeof(int*) * h);
    for(int i = 0; i < h; i++)
        field[i] = malloc(sizeof(int) * w);

    int **mask = malloc(sizeof(int*) * h);
    for(int i = 0; i < h; i++)
        mask[i] = malloc(sizeof(int) * w);

    int x = h / 2;
    int y = w / 2;
    int ox = x;
    int oy = y;
    int fieldgenerated = 0;
    int close_request = 0;

    while(!close_request){

        drawfield(field, mask, w, h, x, y);

        switch(getchar()) {
        case '\033':
            getchar();
            switch(getchar()) {
            case 'A': x--; break;
            case 'B': x++; break;
            case 'C': y++; break;
            case 'D': y--; break;
            }
            break;
#ifdef DVORAK
        case 't': x--; break;
        case 'h': x++; break;
        case 'n': y++; break;
        case 'd': y--; break;
#else
        case 'h': x--; break;
        case 'j': x++; break;
        case 'k': y++; break;
        case 'l': y--; break;
#endif
        case 'q':
            close_request = 1;
        case 'a':
            if(!fieldgenerated) {
                generatefield(field, w, h, w*h/diff, x, y);
                fieldgenerated = 1;
            }
            if(digfield(field, mask, w, h, x, y)) {
                revealfield(field, mask, w, h);
                drawfield(field, mask, w, h, x, y);
                close_request = 1;
            }
            break;
        case 'o':
            toggleflag(mask, x, y);
            break;
        }

    }

    getchar();
    endwin();
    for(int i = 0; i < h; i++) {
        free(field[i]);
        free(mask[i]);
    }
    free(field);
    free(mask);

    return 0;
}
