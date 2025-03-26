#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*int main(int argc, char *argv[]) {*/
/*int *p = malloc(sizeof(int));*/
/*assert(p != NULL);*/
/*printf("(%d) address pointed to by p: %p\n", getpid(), p);*/
/**p = 0;*/
/*while (1) {*/
/*sleep(1);*/
/**p = *p + 1;*/
/*printf("(%d) p: %d\n", getpid(), *p);*/
/*}*/
/*return 0;*/
/*}*/

int main(int argc, char *argv[]) {
    printf("Program attempting to access memory it doesn't own...\n");

    // Try to write to NULL pointer (address 0)
    /*int *ptr1 = NULL;*/
    /*printf("Attempting to write to NULL pointer (address 0)...\n");*/
    /**ptr1 = 42; // This will likely cause a segmentation fault*/
    /*printf("This line will not be reached if the program crashes\n");*/

    // Try to write to a very high memory address
    int *ptr2 = (int *)0xFFFFFFFF;
    printf("Attempting to write to high memory address 0xFFFFFFFF...\n");
    *ptr2 = 100; // This will likely cause a segmentation fault

    printf("If you see this, the program somehow didn't crash\n");
    return 0;
}
