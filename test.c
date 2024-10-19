#include <stdio.h>
#include <stdlib.h>


int main() {
    char s[10];
    int n;

    printf ("Digite aqui: ");

    if (fgets(s, sizeof(s), stdin) != NULL) {
        if (sscanf(s, "%d", &n) == 1) {
            printf ("%d\n", n);
        }
    }
}