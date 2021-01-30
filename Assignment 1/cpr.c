/* ------------------------------------------------ ------------
File: cpr.c

Last name #1: Zhang
Student number #1: 8589976

Last name #2: Xu
Student number #2: 7881937

Description: This program contains the code for creation
 of a child process and attach a pipe to it.
	 The child will send messages through the pipe
	 which will then be sent to standard output.

Explanation of the zombie process
(point 5 of "To be completed" in the assignment):

	(please complete this part);

------------------------------------------------------------- */
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

char **args;

/* Prototype */
void createChildAndRead (int);

/* -------------------------------------------------------------
Function: main
Arguments: 
	int ac	- number of command arguments
	char **av - array of pointers to command arguments
Description:
	Extract the number of processes to be created from the
	Command line. If an error occurs, the process ends.
	Call createChildAndRead to create a child, and read
	the child's data.
-------------------------------------------------- ----------- */

int main (int ac, char **av)
{
 int processNumber; 

 if (ac == 2)
 {
 if (sscanf (av [1], "%d", &processNumber)== 1)
 {
	args = av;
    createChildAndRead(processNumber);
 }
    else fprintf(stderr, "Cannot translate argument\n");
 }
    else fprintf(stderr, "Invalid arguments\n");
    return (0);
}


/* ------------------------------------------------ -------------
Function: createChildAndRead
Arguments: 
	int prcNum - the process number
Description:
	Create the child, passing prcNum-1 to it. Use prcNum
	as the identifier of this process. Also, read the
	messages from the reading end of the pipe and sends it to 
	the standard output (df 1). Finish when no data can
	be read from the pipe.
-------------------------------------------------- ----------- */

void createChildAndRead(int prcNum)
{

 /* Please complete this function according to the
Assignment instructions. */

	int result, pid;
	int fd[2];			//fd[0] read, fd[1] write
	char buff[32];

	int *read_fd = &fd[0];
	int *write_fd = &fd[1];
	
	/* Pipe */
	result = pipe(fd);
	if (result == -1)
	{
		fprintf(stderr, "Fail to create pipe.\n");
		exit(-1);
	}


	if (prcNum < 1)
	{
		fprintf(stderr, "Input should be a positive number, please enter again.\n");
		fflush(stdout);
		exit(-1);
	}

	else if (prcNum == 1)
	{
		sprintf(buff, "Process %d begins\n", prcNum);
		write(1, buff, strlen(buff));
		sleep(5);
		sprintf(buff, "Process %d ends\n", prcNum);
		write(1, buff, strlen(buff));
		sleep(10);
	}

	else
	{
		sprintf(buff, "Process %d begins\n", prcNum);
		write(1, buff, strlen(buff));
		
		pid = fork();

		if (pid < 0)
		{
			fprintf(stderr, "Fail to fork.\n");
			exit(-1);
		}

		/* Child */
		else if (pid == 0)
		{
			close(*write_fd);
			dup2(*write_fd, 1);

			char output[8];
			sprintf(output, "%d", prcNum - 1);
			execlp(args[0], args[0], output, NULL);
		}

		/* Parent */
		else
		{
			close(*write_fd);
			memset(buff, 0, sizeof(buff));

			read(*read_fd, buff, strlen(buff));
			write(1, buff, strlen(buff));

			wait(NULL);
			memset(buff, 0, sizeof(buff));

			sprintf(buff, "Process %d ends\n", prcNum);
			write(1, buff, strlen(buff));

			sleep(10);

		}

	}
	
}



