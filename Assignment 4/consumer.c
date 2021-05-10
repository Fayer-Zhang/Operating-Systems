/**
 * Description: This program contains the code for read and output the sequence from shared memory
 */


#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>

#define KEY 0x1111
#define SIZE 1024 

// structure of the memory 
struct memory {
   long cat_num;     
   int state;           
   long data[SIZE];    
};

int main(int argc, char *argv[]) 
{
   int id;
   struct memory *pt_memo;
   long *pt_data;
   
   id = shmget(KEY, sizeof(struct memory), 0666|IPC_CREAT);
   if (id == -1) {
      perror(" failed to create shared memory ");
      return 1;
   }
   
   pt_memo = shmat(id, NULL, 0);
   if (pt_memo == (void *) -1) {
      perror("shared memory attachment failed ");
      return 1;
   }

   pt_data = pt_memo -> data;
   
   printf("consumer process waiting for the producer process to be completed\n");
   printf("number of Catalan numbers requested is %ld\n", pt_memo -> cat_num);
   printf("\nproducer process is completed, consumer process proceeding\n");
   printf("catalan numbers.. ");
   
   for(int n = 1; n <= pt_memo -> cat_num; n++)
    {
        long catalan = *pt_data;
        printf("%ld ",catalan);
        pt_data++;
    }
   printf("\n");

   if (shmdt(pt_memo) == -1) {
      perror("Error removing ");
      return 1;
   }

   printf("consumer process is completed\n");
   return 0;
}
