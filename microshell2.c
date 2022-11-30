#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int	err(char *str, char *av)
{
	while (*str)
		write(2, str++, 1);
	if (av)
		while (*av)
			write(2, av++, 1);
	write(2, "\n", 1);
	return (1);
}

int cdir(char **av, int i)
{
	if (i != 2)
		return err("error: cd: bad arguments", NULL);
	if (chdir(av[i]) != 0)
		return err("error: cd: cannot change directory to ", av[i] );
	return (0);
}

int	ex(char **av, int i, int fd, char **envp)
{
	av[i] = NULL;
	dup2(fd, 0);
	close(fd);
	execve(av[0], av, envp);
	return (err("error: cannot execute ", av[0]));
}

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	int	i = 0;
	int	fds[2];
	int	fd = dup(0);

	while (av[i] && av[i + 1])
	{
		av = &av[i + 1];
		i = 0;
		while (av[i] && strcmp(av[i], ";") && strcmp(av[i], "|"))
			i++;
		if (strcmp(av[0], "cd") == 0)
			cdir(av, i);
		else if (i != 0 && (av[i] == NULL || strcmp(av[i], ";") == 0))
		{
			if (fork() == 0)
			{
				if (ex(av, i, fd, envp))
					return (1);
			}
			else
			{
				close(fd);
				while (waitpid(-1, NULL, WUNTRACED) != -1)
					;
				fd = dup(0);
			}
		}
		else if (i != 0 && strcmp(av[i], "|") == 0)
		{
			pipe(fds);
			if (fork() == 0)
			{
				dup2(fds[1], 1);
				close(fds[0]);
				close(fds[1]);
				if (ex(av, i, fd, envp))
					return (1);
			}
			else
			{
				close(fds[1]);
				close(fd);
				fd = fds[0];
			}
		}
	}
	close(fd);
	return (0);
}
