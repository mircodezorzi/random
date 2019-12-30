#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>

#include<stdlib.h>
#include<time.h>
#include<math.h>

typedef uint8_t bool;
#define true 1
#define false 0

#define ARRAY_SIZE 145
#define RECT_WIDTH 8
#define PADDING    1

#define WIDTH      ARRAY_SIZE * (RECT_WIDTH + PADDING)
#define HEIGHT     720.0

#define SWAP(a, b)\
    if(a != b){\
        typeof(a) tmp;\
        tmp = a;\
        a = b;\
        b = tmp;\
    }

#define SWAP_ELEM(a, b)\
    if(a != b){\
        typeof(a) tmp;\
        tmp = v->elements[a];\
        v->elements[a] = v->elements[b];\
        v->elements[b] = tmp;\
    }

#define DELAY SDL_Delay(10)

#define SET_COLOR(renderer, r, g, b)\
     SDL_SetRenderDrawColor(renderer, r, g, b, 255)

#define SET_COLOR_BY_VALUE(renderer, v)\
    int r = sin(    v / 50.0)   * 128 + 128;\
    int g = cos(tan(v / 500.0)) * 128 + 128;\
    int b = cos(    v / 50.0)   * 128 + 128;\
    SDL_SetRenderDrawColor(renderer, r, g, b, 255)

typedef struct{
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Color color;
    SDL_Texture* text;
    SDL_Rect rect;
} text_t;

text_t* text_new(SDL_Renderer*, char*, char*, int, int, int);
void text_render(text_t*);
void text_update(text_t*, char*);

text_t*
text_new(SDL_Renderer* r, char* font, char* text, int x, int y, int s){
    text_t *t = malloc(sizeof(text_t));
    t->renderer = r;
    t->font = TTF_OpenFont(font, s);

    t->color.r = 255;
    t->color.g = 255;
    t->color.b = 255;

    SDL_Surface* surface = TTF_RenderText_Solid(t->font, text, t->color);
    t->text = SDL_CreateTextureFromSurface(r, surface);
    SDL_FreeSurface(surface);

    t->rect.x = x;
    t->rect.y = y;
    t->rect.w = surface->w;
    t->rect.h = surface->h;

    return t;
}

void
text_render(text_t* t){
    SDL_RenderCopy(t->renderer, t->text, NULL, &t->rect);
    SDL_RenderPresent(t->renderer);
}

void
text_update(text_t* t, char* text){
    SDL_Surface* surface = TTF_RenderText_Solid(t->font, text, t->color);
    t->text = SDL_CreateTextureFromSurface(t->renderer, surface);
    t->rect.w = surface->w;
    t->rect.h = surface->h;
    SDL_FreeSurface(surface);
}

typedef struct{
    SDL_Renderer* renderer;
    int *elements;
    int size;
} visual_array_t;

visual_array_t* visual_array_generate(SDL_Renderer*, int);
void visual_array_draw(visual_array_t*);
void visual_array_scramble(visual_array_t*);
void visual_array_reverse(visual_array_t*);

visual_array_t*
visual_array_generate(SDL_Renderer* r, int size){
    visual_array_t* v = malloc(sizeof(visual_array_t));
    v->renderer = r;

    v->elements = malloc(sizeof(int) * size);
    v->size = size;

    for(int i = 0; i < size; i++)
        v->elements[i]= -(i + 1) * (HEIGHT / size);

    return v;
}

void
visual_array_scramble(visual_array_t *v){
    int index;
    for(int i = 0; i < v->size; i++){
        index = rand() % v->size;
        SWAP_ELEM(i, index);
    }
}

void
visual_array_reverse(visual_array_t *v){
    int index;
    for(int i = 0; i < v->size / 2 + 1; i++){
        SWAP_ELEM(i, v->size - i);
    }
}

void
visual_array_nearly_sorted(visual_array_t *v){
    int index;
    for(int i = 0; i < v->size / 10; i++){
        index = rand() % v->size;
        SWAP_ELEM(i, index);
    }
}

void
visual_array_draw(visual_array_t* v){
    SDL_Rect rect;
    rect.y = HEIGHT;
    rect.w = RECT_WIDTH;

    SET_COLOR(v->renderer, 0, 0, 0);
    SDL_RenderClear(v->renderer);
    for(int i = 0; i < v->size; i++){
        SET_COLOR_BY_VALUE(v->renderer, v->elements[i]);
        rect.x = i * (WIDTH / v->size);
        rect.h = v->elements[i];
        SDL_RenderFillRect(v->renderer, &rect);
    }
}

void sort_bubble(visual_array_t*);
void sort_selection(visual_array_t*);
void sort_quick(visual_array_t*, int, int);
void sort_merge(visual_array_t*, int, int);
void merge(visual_array_t*, int, int, int);

void
sort_bubble(visual_array_t* v){
    int swaps = 0;
    int accesses = 0;
    int compares = 0;

    bool swapped = true;
    int size = v->size;
    char stats[100];

    text_t* t = text_new(
        v->renderer,
        "/home/mirco/.fonts/Hack-Regular.ttf",
        " ",
        10, 10, 20);

    for(int i = 0; i < v->size && swapped; i++){
        swapped = false;
        for(int j = 0; j < size - 1; j++){
            if(v->elements[j] < v->elements[j + 1]){
                SWAP_ELEM(j, j + 1);
                swapped = true;
                compares++;
                swaps++;
            }
        }
        sprintf(stats, "swaps: %d compares: %d", swaps, compares);
        text_update(t, stats);
        visual_array_draw(v);
        text_render(t);
        DELAY;
        size--;
    }
    free(t);
}

void
sort_selection(visual_array_t* v){
    int swaps = 0;
    int accesses = 0;
    int compares = 0;

    int pos_min;
    char stats[100];

    text_t* t = text_new(
        v->renderer,
        "/home/mirco/.fonts/Hack-Regular.ttf",
        " ",
        10, 10, 20);

    for(int i = 0; i < v->size; i++){
        pos_min = i;
        for(int j = i; j < v->size; j++){
            if(v->elements[j] > v->elements[pos_min]){
                pos_min = j;
            }
        }
        if(i != pos_min){
            SWAP_ELEM(i, pos_min);
            swaps++;
        }
        sprintf(stats, "swaps: %d compares: %d", swaps, compares);
        text_update(t, stats);
        visual_array_draw(v);
        text_render(t);
        DELAY;
        compares++;
    }
    free(t);
}

void
sort_quick(visual_array_t* v, int first, int last){
    static int swaps = 0;
    static int accesses = 0;
    static int compares = 0;

    static char stats[100];
    int pivot;
    int j, i;

    text_t* t;

    if(first == 0){
        t = text_new(
            v->renderer,
            "/home/mirco/.fonts/Hack-Regular.ttf",
            " ",
            10, 10, 20);
        swaps = 0;
        accesses = 0;
        compares = 0;
    }

    if(first < last){
        pivot = first;
        i = first;
        j = last;
        while(i < j){
            while(v->elements[i] >= v->elements[pivot] && i < last){
                i++;
                compares++;
            }
            while(v->elements[j] < v->elements[pivot]){
                j--;
                compares++;
            }
            if(i < j){
                SWAP_ELEM(i, j);
                swaps++;
            }
            sprintf(stats, "swaps: %d compares: %d", swaps, compares);
            text_update(t, stats);
            visual_array_draw(v);
            DELAY;
            text_render(t);
        }
        SWAP_ELEM(pivot, j);
        sort_quick(v, first, j - 1);
        sort_quick(v, j + 1, last);
    }
    if(first == v->size - 1) free(t);
}

void sort_merge(visual_array_t* v, int l, int r){
    if (l < r){
        int m = (r + l) / 2;

        sort_merge(v, l, m);
        sort_merge(v, m + 1, r);

        merge(v, l, m, r);
    }
}

void merge(visual_array_t* v, int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = v->elements[l + i];
    for (j = 0; j < n2; j++)
        R[j] = v->elements[m + 1+ j];

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2){
        if (L[i] >= R[j])
            v->elements[k++] = L[i++];
        else
            v->elements[k++] = R[j++];
        visual_array_draw(v);
        SDL_RenderPresent(v->renderer);
        DELAY;
    }

    while(i < n1){
        v->elements[k++] = L[i++];
        visual_array_draw(v);
        SDL_RenderPresent(v->renderer);
        DELAY;
    }

    while(j < n2){
        v->elements[k++] = R[j++];
        visual_array_draw(v);
        SDL_RenderPresent(v->renderer);
        DELAY;
    }

}

int
main (int argc, char** argv){

    srand(time(NULL));

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow(
        "Sort Visualizer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    visual_array_t* v = visual_array_generate(renderer, ARRAY_SIZE);

    visual_array_scramble(v);
    sort_bubble(v);
    visual_array_reverse(v);
    sort_bubble(v);
    visual_array_nearly_sorted(v);
    sort_bubble(v);

    visual_array_scramble(v);
    sort_selection(v);
    visual_array_reverse(v);
    sort_selection(v);
    visual_array_nearly_sorted(v);
    sort_selection(v);

    visual_array_scramble(v);
    sort_quick(v, 0, ARRAY_SIZE - 1);
    visual_array_reverse(v);
    sort_quick(v, 0, ARRAY_SIZE - 1);
    visual_array_nearly_sorted(v);
    sort_quick(v, 0, ARRAY_SIZE - 1);

    visual_array_scramble(v);
    sort_merge(v, 0, ARRAY_SIZE - 1);
    visual_array_reverse(v);
    sort_merge(v, 0, ARRAY_SIZE - 1);
    visual_array_nearly_sorted(v);
    sort_merge(v, 0, ARRAY_SIZE - 1);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
