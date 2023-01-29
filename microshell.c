#include <libc.h>

int    ft_putstr(char *str, char *arg)
{
    while (*str)
        write(2, str++, 1);
    if (arg)
        while(*arg)
            write(2, arg++, 1);
    write(2, "\n", 1);
    return (1);
}


int ft_execute(char **argv, int i, int tmp, char **env)
{
    argv[i] = NULL;
    dup2(tmp, 0);
    close(tmp);
    execve(argv[0], argv, env);
    return (ft_putstr("error: cannot execute ", argv[0]));
}

int    main(int argc, char **argv, char **env)
{
    int    i;
    int fd[2];
    int tmp;
    (void)argc;
    i = 0;
    tmp = dup(0);
    while (argv[i] && argv[i + 1])
    {
        argv = &argv[i + 1];
        i = 0;
        while (argv[i] && strcmp(argv[i], ";") && strcmp(argv[i], "|"))
            i++;
        if (strcmp(argv[0], "cd") == 0)
        {
            if (i != 2)
                ft_putstr("error: cd: bad arguments", NULL);
            else if (chdir(argv[1]) != 0)
                ft_putstr("error: cd: cannot change directory to ", argv[1]    );
        }
        else if (i != 0 && (argv[i] == NULL || strcmp(argv[i], ";") == 0))
        {
            if ( fork() == 0)
            {
                if (ft_execute(argv, i, tmp, env))
                    return (1);
            }
            else
            {
                close(tmp);
                while(waitpid(-1, NULL, 0) != -1);
                tmp = dup(0);
            }
        }
        else if(i != 0 && strcmp(argv[i], "|") == 0)
        {
            pipe(fd);
            if ( fork() == 0)
            {
                dup2(fd[1], 1);
                close(fd[0]);
                close(fd[1]);
                if (ft_execute(argv, i, tmp, env))
                    return (1);
            }
            else
            {
                close(fd[1]);
                close(tmp);
                tmp = fd[0];
            }
        }
    }
    close(tmp);
    return (0);
}