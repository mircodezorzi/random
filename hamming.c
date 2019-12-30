#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<time.h>

#define size(vec) (sizeof(vec)/sizeof(vec[0]))
#define print(vec)                                     \
    {                                                  \
        for(int i = 0; i < size(vec); i++)             \
            printf("%d, ", vec[i]);                    \
        printf("\b\b \n");                             \
    }

char is_parity(char n){
    return n && !(n & (n - 1)) || !n;
}

int main(){
    char input[8];
    char output[13];
    char second_error = 0;

    printf("input bytes: ");
    fgets(input, size(input) + 1, stdin);
    for(int i = 0; i < size(input); i++)
        input[i]  = input[i] - 0x30 ? 1 : 0;

    int counter = 0;
    for(int i = 1; i < size(output); i++){
        if(!is_parity(size(output) - i))
            output[i] = input[counter++];
    }

    for(int i = 1; i < size(output); i++){
        if(is_parity(size(output) - i)){
            char xored = 0;
            for(int j = 0; j < size(output); j++){
                if(!is_parity(j) && (size(output) - i) & j){
                    xored ^= output[size(output) - j];
                }
            }
            output[i] = xored;
        }
    }

    for(int i = 0; i < size(output); i++)
        if(!is_parity(size(output) - i))
            output[0] ^= output[i];

    print(output);
    printf("P ");
    for(int i = 1; i < size(output); i++){
        printf("%c%d ", !is_parity(size(output) - i) ? 'D' : 'C', size(output) - i);
    }
    printf("\n");

    char corrupted[size(output)];

    for(int i = 0; i < size(output); i++)
        corrupted[i] = output[i];

    srand(time(NULL));
    int r1 = rand() % size(corrupted);
    corrupted[r1] = !corrupted[r1];
    if(second_error){
        int r2 = rand() % size(corrupted);
        corrupted[r2] = !corrupted[r2];
    }

    printf("\ncorrupted\n");
    print(corrupted);
    print(output);

    int error = 0;
    for(int i = 1; i < size(corrupted); i++){
        if(is_parity(size(corrupted) - i)){
            char xored = 0;
            for(int j = 0; j < size(corrupted); j++){
                if(!is_parity(j) && (size(corrupted) - i) & j){
                    xored ^= corrupted[size(corrupted) - j];
                }
            }
            error += (size(corrupted) - i) * (xored ^ corrupted[i]);
        }
    }

    if(error)
        error = size(corrupted) - error;

    char parity_match = 0;
    for(int i = 1; i < size(corrupted); i++){
        if(!is_parity(size(corrupted) - i)) parity_match ^= corrupted[i];
    }

    if(error && !is_parity(size(corrupted) - error)){
        if(parity_match == corrupted[0]){
            printf("errors detected but not able to correct\n");
        }else{
            for(int i = 0; i < error; i++)
                printf("   ");
            printf("^\n");
            printf("error at position: %d, correting\n", error);
            corrupted[error] = !corrupted[error];
        }
    }else{
        printf("no error in data bits found\n");
    }

    return 0;
}
