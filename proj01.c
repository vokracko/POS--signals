#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


sig_atomic_t character = 'A'; 
sig_atomic_t usr1 = 0;
int parent = 1;

void sighandler_usr1(int sig)
{
	usr1 = 1;
}

void sighandler_usr2(int sig)
{
	character = 'A';
}

int main(void)
{
	pid_t fork_pid;
	pid_t other_process;
	char * name;
	int first_run = 1;
	struct sigaction sigact_usr1, sigact_usr2;
	sigset_t empty_set;

	if(sigemptyset(&empty_set) > 0)
		return EXIT_FAILURE;

	sigact_usr1.sa_handler = sighandler_usr1;
	sigact_usr1.sa_flags = 0;
	sigemptyset(&sigact_usr1.sa_mask);
	sigact_usr2.sa_flags = 0;
	sigact_usr2.sa_handler = sighandler_usr2;
	sigemptyset(&sigact_usr2.sa_mask);

	if(sigaction(SIGUSR1, &sigact_usr1, NULL) > 0) 
		return EXIT_FAILURE;

	if(sigaction(SIGUSR2, &sigact_usr2, NULL) > 0)
		return EXIT_FAILURE;

	fork_pid = fork();

	if(fork_pid == 0) // child process
	{
		parent = 0;
		name = "Child";
		other_process = getppid();

		kill(other_process, SIGUSR1); // let parent know child is ready to listen
	}
	else if(fork_pid > 0) // parent process
	{
		parent = 1;
		name = "Parent";
		other_process = fork_pid;
	}
	else // fail
	{
		return EXIT_FAILURE;
	}

	while(1)
	{
		while (!usr1)
			sigsuspend(&empty_set);

		usr1 = 0;

		if(parent && !first_run)
		{
			puts("Press enter...");
			while(getchar() != '\n');
		}

		first_run = 0;

		printf("%s (%d): '%c'\n", name, getpid(), character);

		if(character == 'Z')
			character = 'A';
		else 
			character++;

		kill(other_process, SIGUSR1);
	}
	return EXIT_SUCCESS;
}

