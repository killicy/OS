#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX 1024
#define DELIMS " \n"

int main(int argc, char *argv[]) 
{
	char *cmd, **args, *arg;
	char cmd_line[MAX];
	char **cmd_list;
	int cmd_i = 0, i = 0, pid = 0, argsi = 0;
	int *pidlist, n = 0;

	cmd_list = (char **)malloc(sizeof(char *) * 100);

	while (1) 
	{
		printf("#");
		
		if (!fgets(cmd_line, MAX, stdin)) 
			break;
		

		cmd_list[cmd_i] = malloc((sizeof(char) * strlen(cmd_line)) + 1);
		strcpy(cmd_list[cmd_i], cmd_line);
		// printf("%s", cmd_list[cmd_i]);
		cmd_i++;

		argv[0] = cmd_line;
		args = (char **)malloc(sizeof(char *) * 10);
		arg = strtok(cmd_line, DELIMS);
		argsi = 0;

		while(arg != NULL)
		{
			args[argsi] = malloc((sizeof(char) * strlen(arg))+1);
			strcpy(args[argsi++], arg);
			arg = strtok(0, DELIMS);
		}

		if (args[0])
		{
			if (!strcmp(args[0], "changedir"))
			{
				if (!args[1])
					fprintf(stderr, "missing argument after changedir\n");
				else
					chdir(args[1]);
			}
			else if (!strcmp(args[0], "whereami"))
			{
				if (getcwd(cmd_line, sizeof(cmd_line)) != NULL)
					fprintf(stderr, "%s\n", cmd_line); 
  				else 
       				fprintf(stderr, "bad path\n");
			}
			else if (!strcmp(args[0], "lastcommands"))
			{
				if (!args[1])
				{
					i = 0;
					while (cmd_list[i] != NULL)
					{
						fprintf(stderr, "%s", cmd_list[i++]); 
					}
				}
				else if (!strcmp(args[1], "-c"))
				{
					i = 0;
					while (cmd_list[i] != NULL)
					{
						free(cmd_list[i]);
						cmd_list[i] = NULL;
						i++;
					}

					cmd_i = 0;
				}
				else
				{
					fprintf(stderr, "unknown argument\n");
				}

				
			}
			else if (!strcmp(args[0], "quit"))
			{
				exit(0);
			}
			else if (!strcmp(args[0], "run"))
			{
				pid = fork();

				if (!pid)
				{
					if (execvp(args[1], args) < 0) {
						fprintf(stderr,"Could not execute program %s", args[1]);
					}
					exit(0);
				}
				else {
					waitpid(-1, NULL, 0);
				}
			}
			else if (!strcmp(args[0], "background")) {
				pid = fork();

				if (!pid) {
					if (execvp(args[1], args) < 0) {
						fprintf(stderr, "Could not execute background program %s", args[1]);
					}
					exit(0);
				}
				else {
					fprintf(stderr,"Running in background, PID %d \n", pid);
				}
			}

			else if (!strcmp(args[0], "exterminate"))
			{
				kill(atoi(args[1]), SIGKILL);

				if (kill(atoi(args[1]), SIGKILL) == -1)
				{
					fprintf(stderr, "%s\n", "Kill failure");
				}
				else
				{
					fprintf(stderr, "%s\n", "Kill success");
				}	
			}
			else if (!strcmp(args[0], "repeat")) {
				if (!args[1])
				{
					fprintf(stderr, "missing argument after repeat\n");
				}
				else
				{
					n = atoi(args[1]);
					pidlist = malloc(sizeof(int) * n);

					for (i = 0; i < atoi(args[1]); i++) {
						
						pid = fork();

						if (!pid) {
							if (execvp(args[2], args) < 0) {
								fprintf(stderr,"Could not execute background program %s", args[2]);
							}
							exit(0);
						}
						else {
							fprintf(stderr,"Running in background, PID %d \n", pid);
							pidlist[i] = pid;
						}
					}
				}
			}

			else if (!strcmp(args[0], "exterminateall")) 
			{

				fprintf(stderr, "Murdering %d processes: ", n);

				for (i = 0; i < n; i++)
				{
					fprintf(stderr, "%d ", pidlist[i]);
					kill(pidlist[i], SIGKILL);
				}
			}
			else
			{
				fprintf(stderr, "%s\n", " ");
			}
		}
	}

	return 0;
}

