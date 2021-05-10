#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>


//thread
pthread_t *students_thread; 
pthread_t TA_thread; 

pthread_mutex_t chair_mutex;

//semaphores
sem_t ta;       
sem_t student;  
sem_t chairs[Max_Chair];
sem_t counter;  
sem_t counter_incremented; 

int used = -1;  
int next = 0;   

// count the program
int count = 0;

#define Max_Chair 3
#define Max_Help 20

void *TA_functions(){	
	for(;;){
        sem_wait(&ta);  

        printf("\n-------------------- TA awakened. --------------------\n");

        for(;;){
            pthread_mutex_lock(&chair_mutex);
            // check the number of chair used
            if(used == 0){
                printf("\nThere is no student waiting for the TA.\n");
                printf("\n> The TA is helping the student who have just awakened him.\n");
                int delay = (rand() % 5) + 1 ; 
                sleep(delay);
                sem_post(&student);
                sem_wait(&counter);
                count++;
                printf("\n-------------------- TA helped students %d time(s). --------------------\n",count);
                if(count == Max_Help)
                {
                    printf("\n-------------------- END OF PROGRAM --------------------\n");
                    exit(0);
                }
                sem_post(&counter_incremented);
                pthread_mutex_unlock(&chair_mutex);
                break;
            }
            else{
                sem_post(&chairs[next]);
                used = used > -1 ? used - 1 : -1;

                printf("\nThere are now %d chair(s) available. A Student left a chair.\n", Max_Chair - used);
                // move to the next index for the chairs.
                next =  (next + 1 ) % Max_Chair;
                pthread_mutex_unlock(&chair_mutex);

                printf("The TA is currently helping a student.\n");
                int delay = (rand() % 10) + 1 ; 
                sleep(delay);
                sem_post(&student);
                pthread_mutex_lock(&chair_mutex);
                sem_wait(&counter);
                count++;
                printf("\n-------------------- TA helped students %d time(s). --------------------\n",count);
                if(count == Max_Help){
                    printf("\n-------------------- END OF PROGRAM --------------------n");
                    exit(0);
                }
                sem_post(&counter_incremented);
                pthread_mutex_unlock(&chair_mutex);
            } 
        }
        printf("\nThe TA is asleep again\n");
	}
}

void *students_functions(void *student_id) {
	for(;;){
        printf("\n> Student %ld is programming.\n", (long) student_id);
        int delay =  rand() % 10 + 1;
        sleep(delay);

        pthread_mutex_lock(&chair_mutex);
        int chairs_currently_used =  used;
        pthread_mutex_unlock(&chair_mutex);

        if(chairs_currently_used < Max_Chair){

            printf("\n> Student %ld is trying to get help from the TA.\n", (long) student_id);

            pthread_mutex_lock(&chair_mutex);
            int current_chair =  (next + used) % Max_Chair; 
            used = used < Max_Chair && used >= -1 ? used + 1 : Max_Chair; 

            printf("\n> There are currently %d chair(s) left available.\n", Max_Chair - used);
            pthread_mutex_unlock(&chair_mutex);
        
            if( chairs_currently_used > 0 ) {
                printf("\nStudent %ld is sitting on a chair when he/she is waiting for the TA.\n", (long) student_id);
                sem_wait(&chairs[current_chair]);
                sem_wait(&student);
                printf("\n> The Student %ld have got the help from the TA and is programming.\n",(long) student_id);
                sem_post(&counter);
                sem_wait(&counter_incremented);
            }
            else {
                sem_post(&ta);
                sem_wait(&student);
                printf("\n> The Student %ld have got the help from the TA and is programming.\n",(long) student_id);
                sem_post(&counter);
                sem_wait(&counter_incremented);

            } 
        }
        else{
            printf("\n> Student %ld  is trying to get help from the TA.\n", (long) student_id);
            printf("there is no chairs availble for waiting for the TA. The student is back to programming")
        }
	}
}


/*
 * the default value of the numebr of students is 5
 * enter in the command to set the number of students by user
 */

int main(int ac, char **av){

    int n_students; 

    if(ac == 2) {
       if(sscanf(av[1],"%d",&n_students) == 1){
           printf("\nNumber of students set to %d\n",n_students);
       }
       else fprintf(stderr,"You must enter an integer for the number of students.\n");
    }
    else {
        printf("\nYou did not specify how many times TA is going to help the students");
        n_students = 5 ;
        printf("\nNumber of students is set to %d by default.\n",n_students);
    }

    printf("\n This TA-helping program will stop after the TA have helped students %d times. \n", Max_Help);

    // semaphores
    sem_init(&student, 0, 0); 
	sem_init(&ta, 0, 0);     
    sem_init(&counter, 0, 0); 
	sem_init(&counter_incremented, 0, 0);  

	for(int sem = 0; sem < Max_Chair; sem++)	{
        sem_init(&chairs[sem], 0, 0);
    }		
		
    // mutex
    if (pthread_mutex_init(&chair_mutex, NULL) != 0) { 
        printf("\nFailed to initialized chair_mutex\n"); 
        return 1; 
    } 
    srand(time(NULL));

    students_thread = (pthread_t*) malloc(n_students * sizeof(pthread_t));

    // TA
    pthread_create(&TA_thread, NULL, TA_functions, NULL);
    // students
    for(int student_id = 0; student_id < n_students; student_id++){
        pthread_create(&students_thread[student_id], NULL, students_functions, (void*) (long) student_id);
    }

    // TA thread
    pthread_join(TA_thread,NULL);

    // Students threads
    for(int student_id = 0; student_id < n_students; student_id++){
        pthread_join(students_thread[student_id],NULL);
    }
    
	return 0;
}
