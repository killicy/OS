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
	char *cmd, *arg;
	char cmd_line[MAX];
	char **cmd_list;
	int cmd_i = 0, i = 0, pid = 0;

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
		cmd = strtok(cmd_line, DELIMS);

		if (cmd)
		{
			arg = strtok(0, DELIMS);

			if (!strcmp(cmd, "changedir"))
			{
				
				if (!arg)
					fprintf(stderr, "missing argument after changedir\n");
				else
					chdir(arg);
			}
			else if (!strcmp(cmd, "whereami"))
			{
				if (getcwd(cmd_line, sizeof(cmd_line)) != NULL)
					fprintf(stderr, "%s\n", cmd_line); 
  				else 
       				fprintf(stderr, "bad path\n");
			}
			else if (!strcmp(cmd, "lastcommands"))
			{
				if (!arg)
				{
					i = 0;
					while (cmd_list[i] != NULL)
					{
						fprintf(stderr, "%s", cmd_list[i++]); 
					}
				}
				else if (!strcmp(arg, "-c"))
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
			else if (!strcmp(cmd, "quit"))
			{
				exit(0);
			}
			else if (!strcmp(cmd, "run"))
			{
				pid = fork();
			}
		}
	}

	return 0;
}

