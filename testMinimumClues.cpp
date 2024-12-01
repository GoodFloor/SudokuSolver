#include <stdio.h>

int main(int argc, char const *argv[])
{
    for (int i = 0; i < 14; i++) {
        for (int j = i + 1; j < 15; j++) {
            for (int k = j + 1; k < 16; k++) {
                for (int l = 0; l < 16; l++) {
                    if (l == i) {
                        printf("1");
                    } else if (l == j) {
                        printf("2");
                    } else if (l == k) {
                        printf("3");
                    } else {
                        printf("0");
                    }
                }
                printf("\n");                    
            }
        }
    }
    return 0;
}
