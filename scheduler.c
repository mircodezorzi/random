#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ioctl.h>

#define INITCAPACITY 4
//#define CLEARSCHEDULE

#define CHIGH printf("\033[0;30;41m");
#define CNORM printf("\033[0;30;0m");

#define PRINTCMD(key, desc) printf("\033[0;30;41m%s\033[0;30;0m %s ", key, desc);

#define cursorup(x)    printf("\033[%dA", x);
#define cursordown(x)  printf("\033[%dB", x);
#define cursorright(x) printf("\033[%dC", x);
#define cursorleft(x)  printf("\033[%dD", x);
#define cursorto(x, y) printf("\033[%d;%dH", y, x);

#define SIZE(vec) (sizeof(vec)/sizeof(vec[0]))

typedef struct Process Process;
typedef struct Queue Queue;
typedef struct Core Core;
typedef struct Cpu Cpu;

struct Process {
    char *name;
    int pid;

    int arrival;
    int length;

    int stay;
    int ended;
    int waited;
    int started;
    int ellapsed;
};

struct Queue {
    Process *processes;
    int capacity;
    int total;
    enum { FCFS, SJF, RR } type;
};

struct Core {
    Process process;
    int executing;
    int quantummed;
};

struct Cpu {
    int ncores;
    int tick;
    int quantum;

    Core *cores;
    Queue *ready;
    int nready;
    Queue done;
    Process *schedule;
    int nschedule;
    int nprocesses;
};

Queue newqueue();
void appendqueue(Queue *q, Process p);
void removequeue(Queue *q, int index);
void resizequeue(Queue *q, int capacity);
Process nextqueue(Queue *q);
Cpu *newcpu(int ncores, int quantum, Queue *ready, int nready, Process *schedule, int nschedule);
void freecpu(Cpu *cpu);
void updatecores(Cpu *cpu);
void updateready(Cpu *cpu);
Process averageprocesses(Process *p, int n);
void drawstack(int x, int y, int w, int h, Process *stack, int nstack);
void drawcores(int x, int y, int w, int h, Core *core, int ncores, int selected);
void drawinfo(int x, int y, int w, int h, Process process);
void drawprocess(int x, int y, int w, int h, Process p);
void drawaverage(int x, int y, Process p);
void clear(int x, int y, int h, int w);

Queue
newqueue()
{
    Queue q;
    q.capacity = INITCAPACITY;
    q.total = 0;
    q.processes = malloc(sizeof(Process) * 128);
    q.type = FCFS;
    return q;
}

void
resizequeue(Queue *q, int capacity)
{
    return;
    Process *new = realloc(q->processes, capacity);
    if(new != NULL) {
        q->processes = new;
        q->capacity = capacity;
    }
}

void
removequeue(Queue *q, int index)
{
    q->total--;
    for(int i = index; i < q->total; i++)
        q->processes[i] = q->processes[i + 1];
    if(q->total > 4 && q->total <= q->capacity / 2)
        resizequeue(q, q->capacity / 2);
}

void
appendqueue(Queue *q, Process p)
{
    q->total++;
    if(q->total >= q->capacity)
        resizequeue(q, q->capacity * 2);
    q->processes[q->total - 1] = p;
}

Process
nextqueue(Queue *q)
{
    Process p;

    switch(q->type) {
    case FCFS:
    case RR:
        p = q->processes[0];
        removequeue(q, 0);
        return p;
    case SJF:
        p = q->processes[0];
        for(int i = 1; i < q->total; i++)
            if(q->processes[i].length < p.length)
                p = q->processes[i];
        return p;
    }
}

Cpu *
newcpu(int ncores, int quantum, Queue *ready, int nready, Process *schedule, int nschedule)
{
    Cpu *cpu = malloc(sizeof(Cpu));
    cpu->ncores = ncores;
    cpu->tick = 0;
    cpu->done = newqueue();
    cpu->nprocesses = 0;
    cpu->quantum = quantum;

    cpu->cores = malloc(sizeof(Core) * ncores);
    for(int i = 0; i < ncores; i++) {
        cpu->cores[i].executing = 0;
        cpu->cores[i].quantummed = quantum;
    }

    cpu->schedule = malloc(sizeof(Process) * nschedule);
    cpu->nschedule = nschedule;
    for(int i = 0; i < nschedule; i++){
        cpu->schedule[i] = schedule[i];
        cpu->schedule[i].started = -1;
    }

    cpu->nready = nready;
    cpu->ready = malloc(sizeof(Queue) * nready);
    for(int i = 0; i < nready; i++)
        cpu->ready[i] = newqueue();

    return cpu;
}

void
freecpu(Cpu *cpu)
{
    for(int i = 0; i < cpu->nready; i++)
        for(int j = 0; j < cpu->ready[i].total; j++)
            free(cpu->ready[i].processes);
    free(cpu->ready);
    free(cpu->done.processes);
    free(cpu->schedule);
    free(cpu->cores);
    free(cpu);
}

void
updateready(Cpu *cpu)
{
    for(int i = 0; i < cpu->nschedule; i++) {
        if(cpu->schedule[i].arrival == cpu->tick) {
            appendqueue(&cpu->ready[0], cpu->schedule[i]);
            #ifdef CLEARSCHEDULE
            for(int j = i; j < cpu->nschedule - 1; j++)
                cpu->schedule[j] = cpu->schedule[j + 1];
            cpu->nschedule--;
            i--;
            #endif
        }
    }
}

void
updatecores(Cpu *cpu)
{
    for(int i = 0; i < cpu->ncores; i++) {
        if(cpu->cores[i].executing) {
            cpu->cores[i].process.ellapsed++;
            if(!(cpu->cores[i].process.length - cpu->cores[i].process.ellapsed)) {
                cpu->cores[i].process.ended = cpu->tick;
                appendqueue(&cpu->done, cpu->cores[i].process);
                cpu->cores[i].process.name = NULL;
                cpu->cores[i].executing = 0;
            } else {
                if(cpu->ready[0].type == RR) {
                    cpu->cores[i].quantummed--;
                    if(!cpu->cores[i].quantummed) {
                        for(int j = 0; j < cpu->nready; j++) {
                            cpu->cores[i].quantummed = cpu->quantum;
                            appendqueue(&cpu->ready[0], cpu->cores[i].process);
                            cpu->cores[i].executing = 0;
                        }
                    }
                }
            }
        }

        if(!cpu->cores[i].executing) {
            for(int j = 0; j < cpu->nready; j++) {
                if(cpu->ready[j].total) {
                    cpu->cores[i].process = nextqueue(&cpu->ready[j]);
                    cpu->cores[i].executing = 1;
                    if(cpu->cores[i].process.started == -1)
                        cpu->cores[i].process.started = cpu->tick;
                    cpu->cores[i].process.pid = ++cpu->nprocesses;
                    break;
                }
            }
        }

    }
    for(int i = 0; i < cpu->nready; i++) {
        for(int j = 0; j < cpu->ready[i].total; j++) {
            cpu->ready[i].processes[j].waited += 1;
        }
    }

}

Process
averageprocesses(Process *p, int n)
{
    Process avg;
    avg.waited = 0;
    avg.stay = 0;
    for(int i = 0; i < n; i++) {
        avg.waited += p[i].waited;
        avg.stay += p[i].ended - p[i].started;
    }
    avg.waited /= n;
    avg.stay /= n;
    return avg;
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
colorhash(char *str)
{
    int hash = 0;
    for(int i = 0; i < strlen(str); i++) {
        hash += str[i];
    }
    return hash;
}

void
drawstack(int x, int y, int w, int h, Process *stack, int nstack)
{
    cursorto(x, y);

    printf("\u250C");
    for(int i = 0; i < w - 2; i++) printf("\u2500");
    printf("\u2510");

    cursorto(x, y + 1);

    for(int i = 0; i < (nstack > h ? h : nstack); i++) {
        char format[32];
        printf("\u2502");
        sprintf(format, " %%%d.%ds ", w - 4, w - 4);
        printf("\033[48;5;%dm", colorhash(stack[i].name) % 232);
        printf(format, stack[i].name);
        printf("\033[0;0m");
        printf("\u2502");
        cursorto(x, y + i + 2);
    }

    for(int i = 0; i < h - nstack; i++) {
        printf("\u2502");
        for(int j = 0; j < w - 2; j++) printf(" ");
        printf("\u2502");
        cursorto(x, y + nstack + i + 2);
    }

    printf("\u2514");
    for(int i = 0; i < w - 2; i++) printf("\u2500");
    printf("\u2518");
}

void
drawcores(int x, int y, int w, int h, Core *cores, int ncores, int selected)
{
    cursorto(x, y);

    printf("\u250C");
    for(int i = 0; i < w + 6 ; i++) printf("\u2500");
    printf("\u2510");

    for(int i = 0; i < ncores; i++) {
        cursorto(x, y + i + 1);
        printf("\u2502");
        if (selected == i) printf("\033[48;5;245m");
        printf(" CPU %d ", i);
        if (selected == i) printf("\033[0m");
        printf("\u2502");
        if(cores[i].executing) {
            char format[32];
            sprintf(format, " %%%d.%ds ", w - 4, w - 4);
            printf("\033[48;5;%dm", colorhash(cores[i].process.name) % 232);
            printf(format, cores[i].process.name);
            printf("\033[0m");
            printf("\u2502");
        } else {
            for(int j = 0; j < w - 2; j++) printf(" ");
            printf("\u2502");
        }
    }
    cursorto(x, y + ncores + 1);
    printf("\u2514");
    for(int i = 0; i < w + 6; i++) printf("\u2500");
    printf("\u2518");
    cursorto(x + 8, y);
    printf("\u252C");
    cursorto(x + 8, y + ncores + 1);
    printf("\u2534");
}

void
drawinfo(int x, int y, int w, int h, Process process)
{
    clear(x, y, h, w + 10);
    if(process.name == NULL)
        return;

    cursorto(x, y);
    printf("name: %s", process.name);
    cursorto(x, y + 1);
    printf("pid: %d", process.pid);
    cursorto(x, y + 2);
    printf("progress: %.0f%%", (float)process.ellapsed / (float)process.length * 100);


}

void
drawprocess(int x, int y, int w, int h, Process p)
{
    clear(x, y, h, w + 10);
    if(p.name == NULL)
        return;

    cursorto(x, y);
    printf("name: %s", p.name);

    cursorto(x, y + 1);
    printf("pid: %d", p.pid);
    cursorto(x, y + 2);
    printf("arrival: %d", p.arrival);
    cursorto(x, y + 3);
    printf("length: %d", p.length);
    cursorto(x, y + 4);
    printf("waited: %d", p.waited);
    cursorto(x, y + 5);
    printf("started: %d", p.started);
    cursorto(x, y + 6);
    printf("ended: %d", p.ended);
    cursorto(x, y + 7);
    printf("stay: %d", p.ended - p.started);
}

void
drawstatus(int w, int h)
{
    cursorto(0, h);
    PRINTCMD("q", "quit");
    PRINTCMD("h", "right");
    PRINTCMD("j", "down");
    PRINTCMD("k", "up");
    PRINTCMD("l", "left");
}

void
drawaverage(int x, int y, Process p)
{
    cursorto(x, y);
    printf("waited: %d", p.waited);
    cursorto(x, y + 1);
    printf("stay: %d", p.stay);
}

void
clear(int x, int y, int h, int w)
{
    for(int i = 0; i < h; i++) {
        cursorto(x, y + i);
        for(int j = 0; j < w; j++)
            putchar(' ');
    }
}

int
main()
{

    Process schedule[] = {
        { .name = "runit", .arrival = 0,  .length = 15 },
        { .name = "udev",  .arrival = 5,  .length = 10 },
        { .name = "tty0",  .arrival = 10, .length = 10 },
        { .name = "dwm",   .arrival = 15, .length = 15 },
        { .name = "xterm", .arrival = 20, .length = 20 },
        { .name = "bash",  .arrival = 25, .length = 10 },
        { .name = "top",   .arrival = 31, .length = 15 },
        { .name = "vim",   .arrival = 30, .length = 10 },
        { .name = "gcc",   .arrival = 35, .length = 70 },
        { .name = "surf",  .arrival = 35, .length = 15 },
        { .name = "dd",    .arrival = 50, .length = 15 },
                                                 // 205
    };

    /*
    Process schedule[] = {
        { .name = "p1", .arrival = 0,   .length = 45 },
        { .name = "p2", .arrival = 10,  .length = 30 },
        { .name = "p3", .arrival = 25,  .length = 40 },
        { .name = "p4", .arrival = 30,  .length = 30 },
        { .name = "p5", .arrival = 60,  .length =  5 },
    };
    */

    Queue ready[] = {
        { .type = RR },
    };

    int cores = 1;
    int quantum = 10;

    struct winsize win;
    ioctl(0, TIOCGWINSZ, &win);

    Cpu *cpu = newcpu(cores, quantum, ready, SIZE(ready), schedule, SIZE(schedule));

    int si = 0;
    int sp = 0;

    initwin();

    while(1) {

        cursorto(11, 9);
        printf("\033[1mSchedule\033[0;0m");
        drawstack(10, 10, 15, 10, cpu->schedule, cpu->nschedule);

        for(int j = 0; j < cpu->nready; j++) {
            cursorto(28 + j * 16, 9);
            printf("\033[1mReadyList %d\033[0;0m", j);
            drawstack(27 + j * 16, 10, 15, 10, cpu->ready[j].processes, cpu->ready[j].total);
        }

        drawcores(10, 22, 24, 10, cpu->cores, cpu->ncores, sp ? -1 : si);
        if(!sp)
            drawinfo(47, 22, 10, 10, cpu->cores[si].process);
        else
            clear(47, 22, 10, 15);

        drawstack(10, 28, 32, 16, cpu->done.processes, cpu->done.total);
        if(sp) {
            drawprocess(47, 29, 10, 10, cpu->done.processes[si]);
            if(cpu->done.total) drawaverage(47, 40, averageprocesses(cpu->done.processes, cpu->done.total));
            clear(9, 29, 10, 1);
            cursorto(9, si + 29);
            printf(">");
        } else {
            clear(47, 29, 10, 15);
        }

        drawstatus(win.ws_col, win.ws_row);

        cursorto(23, 7);
        printf("tick: %d", cpu->tick);

        switch(getchar()){
        case 'h':
            if(!sp && si < cpu->ncores - 1)         si++;
            else if(sp && si < cpu->done.total - 1) si++;
            break;
        case 't':
            if(!sp && si > 0) si--;
            else if(sp && si > 0) si--;
            break;
        case 'd':
        case 'n':
            sp = !sp;
            break;
        case 'q':
            endwin();
            free(cpu);
            return 0;
        default:
            updateready(cpu);
            updatecores(cpu);
            cpu->tick++;
            break;
        }

    }

    endwin();
    freecpu(cpu);
    return 0;
}
