/*/////////////////////////////////////////////////////////////////////////////
//
// File name : sieve2.c
// Author    : Prashant Mehta
// Desc      : Finds the primes up to N
//
/////////////////////////////////////////////////////////////////////////////*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"

int main(int argc, char **argv) {

    long N  = 100;

    char *mark;

    long   size;
    long   curr;
    long   i, j,n;
    long   count;

    /* Time */

    double time;

    if ( argc > 1 ) N  = atoi(argv[1]);

    /* Start Timer */

    initialize_timer ();
    start_timer();

    size = (( (N-3) / 2 ) + 1); //Number of odd numbers between 3 and N
    mark = (char *) malloc(size*sizeof(char));

    // Mark all odd numbers upto N as prime
    #pragma omp parallel for
    for (i = 0; i < size; i++ ) {
        mark[i] = 0;
    }
    
    curr = 3;       /*first prime*/


    while (curr*curr<=N) { 
    /* Do until curr is less than or equal to root N because 
     * primes after root N will not have any multiples before N
     * */
        
        #pragma omp parallel for
        for ( i = curr*curr; i <= N; i += curr * 2 ) {
        /* Mark multiples of current prime as non-prime from curr^2 until N with increments of curr*2.
         * From curr^2 since, all non-primes before curr^2 will be already sieved.
         * */
        
            mark[i/2 - 1] = 1;
        }
        
        curr += 2;
        
        while ( mark[curr/2 - 1] ) { 
        // Increment curr to next prime number
            curr += 2;
        }
    }

    /* stop timer */
    stop_timer();
    time = elapsed_time ();
    
    /* Count number of primes*/
    count = 1;
    for(i = 0; i < size; i++){
        if(mark[i] == 0) {
            //  printf("\t prime %ld  \n",i );
            ++count;
        }

    }
    printf("There are %ld primes less than or equal to %ld\n", count, N);
    /* print results */
    printf("First three primes:");
    j = 1;
    printf("%d ", 2);
    for ( i=0 ; i < size && j < 3 ; i++ ) {
        if (mark[i]==0){
            printf("%ld ", i*2 + 3);
            j++;
        }
    }
    printf("\n");

    printf("Last three primes:");
    j = 0;
    n=(N%2?N:N-1);
    for (i = size - 1; i >= 0 && j < 3 ; i--){
        if (mark[i]==0){
            printf("%ld ", i*2 + 3);
            j++;
        }
    }
    printf("\n");


    printf("elapsed time = %lf (sec)\n", time);

    free(mark);
    return 0;
}


