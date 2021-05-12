# Assignment 1 (cpr.c)

## Description
The cpr program (create process) which will have to create a child process. The child process in turn will create a child and so on to create n processes.

The command to create the processes demonstrated below is "cpr num" where num is the number n of total processes to be created (ie n-1 children). The first created process (labeled n) is created by running the command. This first process creates a child by executing the command "cpr num-1", that is, decrements its argument by 1 and creates its child with the new value. So the child will create another child by decrementing its argument. When a process receives an argument with the value 1, it will not create a child.

When a process creates a child, it must first create a pipe and attach the writing end of the pipe to the standard output of the child before executing the "cpr num-1" command. So all child processes (ie process 1 through n-1) write to pipes by writing to their standard output.

All processes that create children (ie process 2 through n) read from the read end of the pipe and write any data read to their standard output. So any output written to the pipes eventually appears on the screen (via process n) after passing through one or more pipes. Note that you do not attach the reading ends of the pipes to the standard inlets (although this is possible).

Actions taken by the processes include the following:
➢ Process 1: simply writes to its standard output "Process 1 begins", waits 5 seconds (using the sleep (5) call), and then writes to its standard output "Process 1 terminates".
➢ Process 2 to n: Creates a child as described above, also writes to its standard output the same messages as Process 1 (by substituting the number 1 with the value of the argument received by the process, 2..n ) as well as read from the read end of the pipe to write the read data to its standard output. You must write the read messages and data in the order necessary to make the messages of all the processes appear in the following order (in this example 5 processes have been created, ie cpr 5 is executed):
```
Process 5 begins
Process 4 begins
Process 3 begins
Process 2 begins
Process 1 begins
Process 1 ends
Process 2 ends
Process 3 ends
Process 4 ends
Process 5 ends
```
There should be a 5 second delay between the "Process 1 Begins" and "Process 1 Ends" messages. Note that a parent does not execute the wait call because it knows that its child is done when the pipe write end is closed (it can no longer read the pipe read end).