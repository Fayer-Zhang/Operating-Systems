/**
 * CSI 3131 
 * 
 * Assignment 4
 * Last name: Xu
 * Student number #: 7881937
 * 
 * Last name: Zhang
 * Student number #: 8589976
 * 
 * Description: This program contains the code for generate
 * the Catalan sequence and write it to a shared memory object
 * 
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

struct memory {
   long cat_num; 
   int state;         
   long data[SIZE];    
};

void catalan_process(long *pt_data, long cat_num); 
long factorial(long value);  



int main(int argc, char *argv[]) {

    long cat_val; 

    if(argc == 2) 
    {
       if(sscanf(argv[1],"%ld",&cat_val) == 1)
       {
           printf("\n the number of Catalan numbers is set to %ld\n",cat_val);
       }
       else 
       {
           fprintf(stderr,"please specify the number of Catalan number.\n");
       }
    }
    else 
    {
        cat_val = 5 ;
        printf("\nnumber of Catalan number is not specified, the number is set to %ld by 		default now\n",cat_val);
    }

    
   int id;
   struct memory *pt_memo;
   long *pt_data;

   id = shmget(KEY, sizeof(struct memory), 0666|IPC_CREAT);
   if (id == -1) {
      perror("failed to create shared memory");
      return 1;
   }
   
   pt_memo = shmat(id, NULL, 0);
   if (pt_memo == (void *) -1) {
      perror("failed to attach shared memory");
      return 1;
   }

    // write to memory
   pt_memo -> cat_num = cat_val;
   pt_data = pt_memo->data;
   catalan_process(pt_data, cat_val);  
   printf("%ld catalan numbers is written in the memory.\n", cat_val);
   
   pt_memo -> state = 1;
   if (shmdt(pt_memo) == -1) {
      perror("Error removing");
      return 1;
   }
   
   printf("producer process is completed\n");
   return 0;
}

//  n factorial
long factorial(long val)
{
    long ans = 1; 
    for(long i = 1; i <= val; i++)
    {
        ans *= i;  
    }
    return ans;
}

// C_n = (2n)!/(n+1)!n!
void catalan_process(long *pt_data, long cat_num) 
{
    long catalan = 0;
    long temp = 0;
    for(int i = 1; i <= cat_num; i++)
    {
    	temp = ( factorial(i+1) * factorial(i));
        catalan = factorial(2*i)/ temp;
        *pt_data = (long) catalan;
        pt_data++;
        printf("%d number added\n",i);
        sleep(3); 
    }
    printf("\ncatalan_process is completed\n");

}

