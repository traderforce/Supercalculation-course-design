#include <stdio.h>
#include <sys/time.h>
#include <athread.h>

extern SLAVE_FUN(func)();

#define X 512
#define Y 512
#define Z 64

double A[X+2][Y+2][Z+2], B[X+2][Y+2][Z+2];

int main(){
    int i,j,k;
    //init A,B,C
    for(i = 0; i < X+2; i++){
    	for(j = 0; j < Y+2; j++){
            for(k = 0; k < Z+2; k++){
                B[i][j][k] = A[i][j][k] = 0;
            }
	}
    }
    for(i = 1; i < X+1; i++){
    	for(j = 1; j < Y+1; j++){
            for(k = 1; k < Z+1; k++){
                A[i][j][k] = i + j + k;
            }
	}
    }
    athread_init();		// init athread
    struct timeval start, end;//add time_control
    gettimeofday(&start, NULL);
    int time = 10;
	while(time--)
	{
		athread_spawn(func,0);	// pass the computation task to slave-cores
		athread_join();		// result collection, syn inside this func
		for(i = 1; i < X+1; i++)
            		for(j = 1; j < Y+1; j++)
                		for(k = 1; k < Z+1; k++){
                    			A[i][j][k] = B[i][j][k];
                		}
	} 
    	
	
    
   

/*
    for(i = 0; i < X; i++)
		for(j = 0; j < Y; j++)
			C[i][j] = A[i][j] + B[i][j];*/

    //以下为记录整个代码运行所需时间，不需要改动
    gettimeofday(&end, NULL);
    athread_halt();		// shut down athread
    double time_use = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
    printf("Time usage is %lf us\n", time_use);

    //printf("(C[0], C[999]) = (%d, %d)\n", C[0], C[999]);
    return 0;
}
