//
// File name   : knapB.c
// Description : Dynamic Programming for the 0/1 knapsack  problem. 
//

#include <stdio.h>
#include <stdlib.h>
#include "timer.h"


#define    MAX(x,y)   ((x)>(y) ? (x) : (y))
#define    MIN(x,y)   ((x)<(y) ? (x) : (y))

#define    table(i,j)    table[(i)*(C+1)+(j)]

#define HBLOCK_FACTOR 1000
#define VBLOCK_FACTOR 10

int main(int argc, char **argv) {

    FILE   *fp;
    int    N, C;                   // # of objects, capacity 
    int    *weights, *profits;     // weights and profits
    int    verbose;
    int *table, *soln;	

    int bHeight, bWidth, numHBlocks, numVBlocks, weight, profit;



    // Temp variables
    int    i, j, count, size, r, c, jB, iB, w, b;

    // Time
    double time;

    int hblock_factor, vblock_factor;

    int bi, bj;



    // Read input file:
    //  first line:  # of objects, knapsack capacity, 
    //  next lines:   weight and profit  of next object (1 object per line)
    if ( argc > 1 ) {
        fp = fopen(argv[1], "r"); 
        if ( fp == NULL) {
            printf("[ERROR] : Failed to read file named '%s'.\n", argv[1]);
            exit(1);
        }
    } else {
        printf("USAGE : %s [filename].\n", argv[0]);
        exit(1);
    }

    if (argc > 2) verbose = 1; else verbose = 0;

    fscanf(fp, "%d %d", &N, &C);
    printf("The number of objects is %d, and the capacity is %d.\n", N, C);

    if (N < VBLOCK_FACTOR) vblock_factor = 2;

    else    
        vblock_factor = VBLOCK_FACTOR;

    if (C < HBLOCK_FACTOR) hblock_factor = 5;

    else    
        hblock_factor = HBLOCK_FACTOR;


    size    = N * sizeof(int);
    weights = (int *)malloc(size);
    profits = (int *)malloc(size);

    if ( weights == NULL || profits == NULL ) {
        printf("[ERROR] : Failed to allocate memory for weights/profits.\n");
        exit(1);
    }

    for ( i=0 ; i < N ; i++ ) {
        count = fscanf(fp, "%d %d", &(weights[i]), &(profits[i]));
        if ( count != 2 ) {
            printf("[ERROR] : Input file is not well formatted.\n");
            exit(1);
        }
    }

    fclose(fp);

    table = (int *) malloc((N+1)*(C+1)*sizeof(int));
    soln = (int *) malloc(size);   


    bHeight = (N) / vblock_factor ;
    numVBlocks = N/ bHeight + 1;
    bWidth  = (C+1)  / hblock_factor;


    numHBlocks = ((C + 1) / bWidth) + 1;
    //numVBlocks = ( (N) / bHeight ) + 1;
  //  printf("block height is %d and number of h blocks is %d\n",bHeight, numVBlocks);

//    printf("block width is %d and number of w blocks is %d\n",bWidth, numHBlocks);

    initialize_timer ();
    start_timer();


    // Solve for the optimal profit (create the table)

#pragma omp parallel for
    for (i = 0; i <= C; i++) {
        table(0,i) = 0;
    }	


  
    for ( w = 0; w < numVBlocks + numHBlocks - 1; w++) {

#pragma omp parallel for private(i, j, bi, bj)
        for (bi = MAX(0, (w - numVBlocks + 1)); bi <= MIN (w, numHBlocks-1); bi++) {

            bj = w - bi;

            // Process each block do this - 

        //    printf("w is %d, bi is %d and bj is %d\n",bi, bj,w);

            for (i = MAX(1,bj*bHeight); i < MIN((bj+1)*bHeight, N+1); i++ ) // Each row in block
            {
                weight = weights[i-1];
                profit = profits[i-1];


                for (j = bi*bWidth; j < MIN((bi+1)*bWidth, C+1); j++) {
                
    //            printf("i is %d and j is %d \n",i,j);
                    
                    table(i,j)=table(i-1,j);

                    if (weight <= j)
                    {

                        table(i,j)=MAX(table(i-1,j),profit+table(i-1,j-weight));
                    }

                }
            }


        }

    }

//printf("i is %d and j is %d %d\n",i,j, table(10,25));
/*

    for (i = 0; i < numVBlocks ; i++) {
        r = i * bHeight;

        for (iB = 1; iB <= bHeight && r < N; iB++) {

            r = iB + (i * bHeight);

            for (j = 0; j < numHBlocks; j++) {

                c = j * bWidth;
                for (jB = 0; jB < bWidth && c < C+1; jB++) {


                    c = jB + (j * bWidth) ;

                    // printf("(%d, %d)\t", r, c);

                    table(r,c)=table(r-1,c);

                    if (weights[r-1] <= c) 
                    {

                        table(r,c)=MAX(table(r-1,c),profits[r-1]+table(r-1,c-weights[r-1]));
                    }
                }
            }
        }
        // printf("\n");
    }
*/
    // printf("Last i and c are (%d,%d)\n",i,c);

    // We only time the creation of the table

    stop_timer();
    time = elapsed_time ();

    
    printf("The optimal profit is %d Time taken : %lf.\n", table(N, C), time);

    // End of "Solve for the optimal profit"


    // Find the solution (choice vector) by backtracking through the table

    j=C;
    for(i=N;i>=1;i--){
        if( (j>weights[i-1])&& (table(i-1,j) < (profits[i-1]+table(i-1,j-weights[i-1]))) ){


            soln[i-1]=1;
            j=j-weights[i-1];
        }
        else{
            soln[i-1]=0;
        }
    }


    printf("Solution vector is: \n --> ");

    if (verbose) {
        // print the solution vector 
        for( i = 0; i < N; i++ ) {
            printf("%d ",soln[i]);
        } 

    }

    printf("\n");

    return 0;
}
