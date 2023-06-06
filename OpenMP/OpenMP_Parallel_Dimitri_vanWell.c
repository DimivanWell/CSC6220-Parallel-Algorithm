//Dimitri van Well's || OMP Program
//Compiler Command for this program: gcc -fopenmp -o HW4 vanWell_Dimitri_CSC6220_HW4.c -lm

#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

//_______Global Definitions______________
	int bnd = 1024; //Matrix Bound
	int thrd; //thread amount
	int i; //interation counter
	int matA[1024][1024]; //Matrix A (all values 1)
	int matB[1024][1024]; //Matrix B (all values 2)
	int matC[1024][1024]; //Matrix C the product of A and B

	void ompFunc(int threadIn) //A function that will call the parallel matrix product algorithm with changing thread_num pass
	{
	int r,c,m; //Row, Column, multiplier counter
	double start, end = 0; //values for the timer
	bnd = 1024;
	double omp_get_wtime(void);
	
	for (r=0;r<bnd;r++) //fill Matrix A and B with their respective values
	{
		for (c=0;c<bnd;c++)
		{
			matA[r][c] = 1;
			matB[r][c] = 2;
		}
	}
	int tid,nthread;	
	start = omp_get_wtime(); //collect time before the algorithm starts
	/* Fork a team of threads giving them their own copies of variables */
	#pragma omp parallel private(r,c,m) shared(matA,matB,matC,bnd) num_threads(threadIn)
	{
		tid = omp_get_num_threads();
		#pragma omp for schedule(guided) 
				for (r=0;r<bnd;r++)
				{
					for (c=0;c<bnd;c++)
					{
						matC[r][c] = 0;
						for (m=0;m<bnd;m++)
						{
							matC[r][c]=matC[r][c] + matA[r][m] * matB[m][c];
						}
					}
				}
	}
	end = omp_get_wtime(); //collect time at the end of the algorithm
	printf("Schedule type: Guided, Time: %f second @ %0d thread used\n", (end-start),tid); //print output for the time elapsed and the thread_num
	if(threadIn == 1) //main thread to print to a result file
	{
		FILE *fp=fopen("result.txt","w+");
		for(r=0;r<bnd;r++)
		{
        		for(c=0;c<bnd;c++)
        		{
                		fprintf(fp," %d ",matC[r][c]);
        		}
        		fprintf(fp,"\n");
		}
		fclose(fp);
	}
	/* All threads join master thread and disband */
	
	}
int main() //Main function will call the parallel function at different threads being passed
{
int thrdIncAmt = 0;
for(i=0;i<6;i++)
{
        thrd = pow(2,thrdIncAmt);
	ompFunc(thrd);	

	thrdIncAmt += 1;
}
}
