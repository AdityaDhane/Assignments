/*
	command line shell
	System: Linux
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>			// exit()
#include <unistd.h>			// fork(), getpid(), exec()
#include <sys/wait.h>		// wait()
#include <signal.h>			// signal()
#include <fcntl.h>			// close(), open()

#define MAX_COMMANDS 20     //maximum allowed commands

typedef enum Type {quit,parallel,sequential,redirection,simple,invalid} cmd_type;	//type of command 
typedef struct Command 
{
	int argc;				//number of arguments of command
	char *argv[100];		//arguments of command
	char *outfile;			//Ouput file if NULL the output to stdout
}command;

command cmd[MAX_COMMANDS];	//Array for storing parsed commands
int cmd_count;				//number of commands parsed
char *cmdline;				//char pointer for command line input

cmd_type parseInput()		//returns command type
{
	// This function will parse the input string into multiple commands or a single command with arguments depending on the delimiter (&&, ##, >, or spaces).
	while(*cmdline==' ')cmdline++;				//skip spaces
	cmd_type type=simple;						//default command type
	cmd_count=-1;								//number of commands parsed - 1 , incremented at second last line of funtion 
	while(type!=invalid && cmdline)
	{
		cmd_count++;							//new command 
		cmd[cmd_count].argc=0;					//command initializations
		cmd[cmd_count].outfile=NULL;
		char *token;							//to store return value of strsep()
		while(token = strsep(&cmdline," \n"))	//next word
		{
			// printf("%s\n",token);
			if(strcmp(token,"exit")==0)			//if exit
			{
				if(type==simple)			
					type=quit;
				else
					type=invalid;
			}
			if(strcmp(token,"&&")==0)			//parallel commands
			{
				if(type==simple  || type==parallel)
				{
					type=parallel;
					cmd[cmd_count].argv[cmd[cmd_count].argc]=NULL;
					break;
				}
				else 
					type=invalid;
			}
			if(strcmp(token,"##")==0)			//sequential commmands
			{
				if(type==simple  || type==sequential)
				{
					type=sequential;
					cmd[cmd_count].argv[cmd[cmd_count].argc]=NULL;
					break;
				}
				else 
					type=invalid;
			}
			else if(strcmp(token,">")==0)		//redirection
			{
				if(type==simple)
					type=redirection;
				else 	
					type=invalid;
			}
			else								
			{
				if(type==redirection && cmd[cmd_count].outfile==NULL)		
				{
					cmd[cmd_count].outfile=token;							//set output file for redirection
				}
				else
					cmd[cmd_count].argv[cmd[cmd_count].argc++]=token;		//get command arguments
			}
		}
	}
	cmd[cmd_count].argv[cmd[cmd_count].argc-1]=NULL;						//set argument array end to NULL
	cmd_count++;															//adjust command count
	return type;
}

void executeCommand()
{
	// This function will fork a new process to execute a command
	if(strcmp(cmd[0].argv[0],"cd")==0)						//cd command
			chdir(cmd[0].argv[1]);
	else
	{
		int rc=fork();										//invoke child process
		if(rc<0)
		{
			printf("Command failed\n");
		}
		else if(rc==0)
		{
			signal(SIGINT,SIG_DFL);							//set default behaviour  for SIGINT i.e. Ctrl-c
			signal(SIGTSTP,SIG_DFL);						//set default behaviour  for SIGTSTP i.e. Ctrl-z
			if(execvp(cmd[0].argv[0],cmd[0].argv)==-1)		//execute command
			{
				printf("Shell: Incorrect command\n");
				exit(0);									//exit if execvp returns -1
			}
		}
		else 
		{
			wait(&rc);										//wait for child process to finish
		}
	}
}

void executeParallelCommands()
{
	// This function will run multiple commands in parallel
	int rc;
	for(int i=0;i<cmd_count;i++)								//loop over all commands
	{
		if(strcmp(cmd[i].argv[0],"cd")==0)						//cd command
			chdir(cmd[i].argv[1]);
		else
		{
			rc=fork();											//invoke child process
			if(rc<0)
			{
				printf("Command failed\n");
			}
			else if(rc==0)
			{
				signal(SIGINT,SIG_DFL);							//set default behaviour  for SIGINT i.e. Ctrl-c
				signal(SIGTSTP,SIG_DFL);						//set default behaviour  for SIGTSTP i.e. Ctrl-z
				if(execvp(cmd[i].argv[0],cmd[i].argv)==-1)		//execute command
				{
					printf("Shell: Incorrect command\n");
					exit(0);									//exit if execvp returns -1
				}
			}
		}
	}
	int status=0;
	while(wait(&status)>0);										//wait for all child processes to finish
}

void executeSequentialCommands()
{	
	// This function will run multiple commands in parallel
	for(int i=0;i<cmd_count;i++)								//loop over all commands
	{
		if(strcmp(cmd[i].argv[0],"cd")==0)						//cd command
				chdir(cmd[i].argv[1]);
		else
		{
			int rc=fork();										//invoke child process
			if(rc<0)
			{
				printf("Command failed\n");
			}
			else if(rc==0)
			{
				signal(SIGINT,SIG_DFL);							//set default behaviour  for SIGINT i.e. Ctrl-c
				signal(SIGTSTP,SIG_DFL);						//set default behaviour  for SIGTSTP i.e. Ctrl-z
				if(execvp(cmd[i].argv[0],cmd[i].argv)==-1)		//execute command
				{
					printf("Shell: Incorrect command\n");
					exit(0);									//exit if execvp returns -1
				}	
			}
			else 
			{
				wait(&rc);										//wait for child process to finish
			}	
		}
	}
}

void executeCommandRedirection()
{
	// This function will run a single command with output redirected to an output file specificed by user
	if(strcmp(cmd[0].argv[0],"cd")==0)										//cd command
		chdir(cmd[0].argv[1]);
	else
	{
		int rc=fork();
		if(rc<0)
		{
			printf("Command failed\n");
		}
		else if(rc==0)
		{
			signal(SIGINT,SIG_DFL);											//set default behaviour  for SIGINT i.e. Ctrl-c
			signal(SIGTSTP,SIG_DFL);										//set default behaviour  for SIGTSTP i.e. Ctrl-z
			int fd=close(STDOUT_FILENO);									//close STDOUT
			open(cmd[0].outfile,O_CREAT | O_WRONLY | O_TRUNC,S_IRWXU);		//open output file
			if(execvp(cmd[0].argv[0],cmd[0].argv)==-1)						//execute command
			{
				printf("Shell: Incorrect command\n");
				exit(0);													//exit if execvp returns -1
			}
		}
		else 
		{
			wait(&rc);														//wait for child to finish
		}
	}
}

int main()
{
	signal(SIGINT,SIG_IGN);														//ignore SIGINT i.e. Ctrl-c
	signal(SIGTSTP,SIG_IGN);													//ignore SIGTSTP i.e.	Ctrl-z

	size_t len=0;																//used in getline()
	char current_dir[100];														//used for storing current directory for prompt

	while(1)		//infinite loop
	{
		printf("%s$",getcwd(current_dir,sizeof(current_dir)));		//prompt
		int x=getline(&cmdline,&len,stdin);										//command line input

		cmd_type type = parseInput(cmdline);									//parse input
		if(type==invalid)														//invalid input
		{
			printf("Shell: Incorrect command\n");
			continue;
		}
		if(type==quit)															//exit	
		{
			printf("Exiting shell...\n");
			break;
		}

		if(type==parallel)														//parallel commands
			executeParallelCommands();
		else if(type==sequential)												//sequential commands
			executeSequentialCommands();
		else if(type==redirection)												//output redirection
			executeCommandRedirection();
		else
			executeCommand();													//only 1 command
	}
	return 0;
}
