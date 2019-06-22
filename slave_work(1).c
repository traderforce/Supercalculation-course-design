#include "slave.h"

#define X 512
#define Y 512
#define Z 64
#define I 1188
//从核数据流大小3*66*18

__thread_local volatile unsigned long get_reply, put_reply;
__thread_local int my_id;
__thread_local double a_slave[I*3],b_slave[I];
extern double A[X+2][Y+2][Z+2],B[X+2][Y+2][Z+2];

void func()
{
        int i,j;
        my_id = get_row()*8 + get_col();//为了得到当前从核的ID
        int nextX = my_id*8;
        int cntX = 0;
        get_reply = 0;		//Once athread_get returns, get_reply++
        while (cntX < 8){
          int nextY = 0;
          while (nextY < 512){
            athread_get(PE_MODE,&A[nextX][nextY][0],&a_slave[0],I*8,&get_reply,0,0,0);
            athread_get(PE_MODE,&A[nextX+1][nextY][0],&a_slave[I],I*8,&get_reply,0,0,0);
            athread_get(PE_MODE,&A[nextX+2][nextY][0],&a_slave[2*I],I*8,&get_reply,0,0,0);
            //athread_get(DMA传输命令模式【这里一般不改】,主存源地址,本地目标局存地址,DMA传输数据量【字节为单位】,传输回答字地址【即为局存地址】，本题可以不该，本题可以不该，本题可以不该)
            int curx=nextX,cury= nextY;
            while(get_reply!=3);
            for (i =1;i<=16;i++){
              for (j=1;j<=64;j++){
                b_slave[i*64+j]= a_slave[i*64+j]+a_slave[2*I+i*64+j]+a_slave[I+(i+1)*64+j]+a_slave[I+(i-1)*64+j]+a_slave[I+i*64+j-1]+a_slave[I+i*64+j+1]+a_slave[I+i*64+j];
                //A[curx][i+cury][j] =
              }
            }
            athread_put(PE_MODE,&b_slave[0],&B[nextX+1][nextY][0],I*8,&put_reply,0,0);
            while(put_reply!=1);
            nextY += 16;
	    get_reply=0;
	    put_reply=0;
            //athread_put(DMA传输命令模式【这里一般不改】,本地目标局存地址,主存源地址,DMA传输数据量【字节为单位】,传输回答字地址【即为局存地址】，本题可以不该，本题可以不该，本题可以不该)
          }
          nextX++;cntX++;
        }
}
