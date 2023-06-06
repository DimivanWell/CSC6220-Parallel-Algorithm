#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<pthread.h>
#define NUM_THREADS	 	8 //# of threads

//Global matrix
int matrx[256][256];
int return_mat[256][256];

pthread_mutex_t mutex;

void cret_mat() //create a matrix with 256x256 elements
{	
        int val2 = 1;
        int row;
        int col;

        for(row=0;row<256;row++)
        {
                for(col=0;col<256;col++)
                        {
                              	matrx[row][col]=val2;
                                val2 = val2 + 1;
                        }
        }
}

int rtnFile(int mat[256][256]) //print arithmetic result matrix to result.txt file
{
	FILE *fp=fopen("result.txt","w+");
	int col;
	int row;
	for(row=0;row<256;row++)
	{
		for(col=0;col<256;col++)
		{
			fprintf(fp," %d ", mat[row][col]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}

void *matrix(void *threadid) //# thread function
{	
	
	int mat[256][256]; //#create 256x256 matrix
        int val1 = 1;
        int row;
        int col;

	long tid;
	tid = (long)threadid;
	
	int val;
	int parRow = 32 * (7 - tid); //set thread parse bound 1 
	int maxRow = (32 * (7 - tid)) + 31; //set thread parse bound 2
	int mat2[256][256];

	for(parRow;parRow<=maxRow;parRow++) //parse loop based on thread bounds
	{
		for(col=0;col<256;col++)
		{
			val = matrx[parRow][col] + matrx[parRow][col];
			mat2[parRow][col] = val;
			return_mat[parRow][col] = mat2[parRow][col];
			
		}
	}

	printf("Thread %d: Done\n", (tid+1));
	pthread_exit(NULL);
	int jn;

	//Join thread loop
	for(jn=0;jn<NUM_THREADS;jn++) 
	{
		pthread_join(threadid,NULL);
	}

}

int main(int argc, char *argv[]) //main program
{
	pthread_t threads[NUM_THREADS];
	pthread_mutex_init(&mutex, NULL);
	int rc;
	long i;
	cret_mat(); //matrix call

	//Thread passing loop
	for(i=7; i>=0; i--){
		printf("In main: creating thread %ld\n", (i+1));
		rc = pthread_create(&threads[i], NULL, matrix, (void *)i);
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	rtnFile(return_mat); //result.txt function call
	pthread_mutex_destroy(&mutex);
	pthread_exit(NULL);
}
