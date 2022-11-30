#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int	err(char *str)
{
	while (*str)
		write(2, str++, 1);
/*	if (av)
		while (*av)
			write(2, av++, 1);
	write(2, "\n", 1);*/
	return(1);
}

int cdir(char **av, int i)
{
	if (i != 2)
		return err("error: cd: bad arguments\n");
	//	return err("error: cd: bad arguments", NULL);
	if (chdir(av[i]))
		return err("error: cd: cannot change directory to ") & err(av[i]) & err("\n");
	//	return err("error: cd: cannot change directory to ", av[i] );
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	int	i = 0;
	int	fds[2];
	int	fd = dup(0);

	while (av[i] && av[++i])
//	while (av[i] && av[i + 1])
	{
		av = av + i;
	//	av = &av[i + 1];
		i = 0;
		while (av[i] && strcmp(av[i], ";") && strcmp(av[i], "|"))
			i++;
		//if (strcmp(av[0], "cd") == 0)
		//	cdir(av, i);
		if (!strcmp(*av, "cd"))
			cdir(av, i);
	}
}