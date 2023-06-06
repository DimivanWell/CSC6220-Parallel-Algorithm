/*_______Dimitri van Well || MPI_______*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#define N 2048

double start, end;
int A[N][N], B[N][N], C[N][N];
int npes, myrank, i, j;
int val = 1;
int nlocal;


main (int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	nlocal = N/npes;
	/*create local matrices */
	int AL[nlocal][N], BL[nlocal][N], CL[nlocal][N];
	/* Process 0*/
	if(myrank == 0)
	{
		start = MPI_Wtime();
		for (i = 0; i < N; i++)
		{
			for (j = 0; j < N; j++)
			{
				A[i][j] = val;
				B[i][j] = val;
				val += 1;
				/*here you intialize the elements of the matrix...*/
			}
			val = 1;
		}
	}
	/* Process 0 sends parts of the matrices*/
	MPI_Scatter(A, N*N/npes, MPI_INT, AL, N*N/npes, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(B, N*N/npes, MPI_INT, BL, N*N/npes, MPI_INT, 0, MPI_COMM_WORLD);
	
	/*add local matrices*/
	for (i = 0; i < nlocal; i++)
	{
		for (j = 0; j < N; j++)
		{
			CL[i][j] = AL[i][j] + BL[i][j];
		}	
	}
	/* Process 0 gatters locals C's*/
	MPI_Gather(CL, N*N/npes, MPI_INT, C, N*N/npes, MPI_INT, 0, MPI_COMM_WORLD);	
	printf("Process %i: Done\n", myrank);
	//end = MPI_Wtime();

	if (myrank == 0)
	{	
		end = MPI_Wtime();
		printf("Time for execution: %f\n", (end-start));
		FILE *fp=fopen("result.txt","w+");
	        int col, row;
	        for(row=0;row<N;row++)
	        {
	                for(col=0;col<N;col++)
	                {
	                        fprintf(fp," %d ",C[row][col]);
	                }
	                fprintf(fp,"\n");	
	        }
		fclose(fp);
	}
	MPI_Finalize();
}

