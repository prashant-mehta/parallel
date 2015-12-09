/*/////////////////////////////////////////////////////////////////////////////
//
// File name : sieve3.c
// Author    : Prashant Mehta
// Desc      : Finds the primes up to N
//
/////////////////////////////////////////////////////////////////////////////*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"

#define BLKSIZE 1000000
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))


int main(int argc, char **argv) {

    long N  = 100;

    char *mark;

    long   size;
    long   curr;
    long   i, j, n, ii, start;
    long   count;
    long rootN;
    long sizerootN;

    long *primes;

    long blockStart, numPrimes;

    /* Time */

    double time;

    if ( argc > 1 ) N  = atoi(argv[1]);

    /* Start Timer */

    initialize_timer ();
    start_timer();

    size = ((N-3) / 2) + 1 ; // Number of odd numbers between 3 and N
    mark = (char *) malloc( size * sizeof(char));


    // Mark all odd numbers upto N as prime
    for ( i = 0; i < size; i++ ){
        mark[i] = 0;
    }

    curr=3;       /* first prime */

    // Square root of N and number of odds upto square root of N
    rootN = sqrt(N);
    sizerootN = ((rootN-3) / 2) + 1;

    // Array to store primes upto square root of N
    primes = (long *) malloc(sizerootN * sizeof(long));

    while ( curr*curr <= rootN ) {
    /* Do until curr is less than or equal to square root of rootN because 
     * primes after those will not have any multiples before root N
     * */

        for ( i = curr*curr; i <= rootN; i += curr*2 ) {
        /* Mark multiples of current prime as non-prime from curr^2 until root N with increments of curr*2.
         * From curr^2 since, all non-primes before curr^2 will be already sieved.
         */
            mark[i/2 - 1]=1;

        }
        
        curr += 2;
        
        while ( mark[curr/2 - 1] ) { 
        // Increment curr to next prime (unsieved) number
            curr += 2;
        }
    }


    /* Count number of primes until square root of N*/
    count = 0;
    for ( i = 0; i < sizerootN; i++ ) {
        
        if ( mark[i] == 0 ) {
            primes[count] = i*2 + 3;
            count++;
        }
    }

    /* Block start should be next odd number after rootN
     * If rootN is even, then it is rootN + 1. Otherwise rootN + 2
     **/
    blockStart = ( rootN + 1 ) + rootN%2;
    register long curr_prime;

    for ( ii = blockStart; ii <= MIN (ii+BLKSIZE, N) ; ii += BLKSIZE ) {
    /* Each block goes from blockStart till minimum of ii+BLKSIZE and N
     * */

        for ( j = 0; j < count; j++ ){
        /* In current block sieve multiples of primes less than rootN
         * */

            curr_prime = primes[j];

            if ( curr_prime * curr_prime > ii+BLKSIZE ) {
            /* In case square of current prime number is greater than end of block,
             * move to next block because all non-primes in current block are already
             * marked as non-prime.
             * */
                break;
            }

            start = ii;

            if ( ii % curr_prime != 0 ) {
            /* Start sieveing at odd multiple of current prime in current block
             * viz. after ii.
             */
                start = curr_prime * ( ( ii / curr_prime ) + ( ( ii / curr_prime ) & 1) + 1);
                /* ii/curr_prime - Floor of ii/curr_prime
                 * If it is even, next multiple of curr_prime will be odd --> curr_prime*(ii/curr_prime + 1)
                 * If it is odd, start = curr_prime * ( ii / curr_prime + 2) 
                 */
            }

            for (i = start; i <= MIN(ii+BLKSIZE, N); i += curr_prime*2 ){
            /* Mark multiples of curr_prime from start until end of block as non-prime
             * */
                mark[i/2 - 1] = 1;
            } 			

        }
    }

    //printf("c\n");

    /* stop timer */
    stop_timer();
    time=elapsed_time ();



    count = 1;
    for(i = 0; i < size; i++){
        if(mark[i] == 0) {
            ++count;
        }

    }


    //for(i = 0; i < count-1; i++)
    //	printf("prime[%d] is %d",i, primes[i]);

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
    free(primes);
    return 0;
}


