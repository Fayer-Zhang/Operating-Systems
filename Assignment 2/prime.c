#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// check if the number is prime
bool isPrime(int number){
	if(number <= 1)
		return false;
	if(number < 4)
		return true;
	if(number % 2 == 0)
		return false;
	for(int i = 4; i * i <= number; i = i+6)
		if(number % i == 0)
			return false;

	return true;
}

void* printPrime(void* num){
	int number = *((int*)num);

	printf("Prime numbers smaller or equal than %d are : \n", number);
	for(int i = 1; i <= number; i++)
		if(isPrime(i))
			printf("%d\n", i);	
}


int main(int argc, char *argv[]){
	if(argc != 2){
		fprintf(stderr, "Wrong number of arguments. Please try again \n");
		exit(1);
	}

	int num = atoi(argv[1]);
	if(num <= 2 ) {
		fprintf(stderr, "Error! Please enter a number which is greater than 2  \n");
		exit(1);
	}

	// creates separate thread
    pthread_t primer;
    if(pthread_create(&primer, NULL, printPrime, &num)) {
        fprintf(stderr, "Error creating thread \n");
        exit(1);
    }

    // join thread
    if(pthread_join(primer, NULL)) {
        fprintf(stderr, "Error joining thread \n");
        exit(1);
	}	
	return 0;
}
