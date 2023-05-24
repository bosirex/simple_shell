#include "shell.h"

/**
 * main_shell - main shell loop
 * @dat: parameter.
 * @av: argument.
 *
 * Return: 0 on success, 1 on error, or error code
 */
int main_shell(data *dat, char **av)
{
	ssize_t q = 0;
	int ret_built = 0;

	while (q != -1 && ret_built != -2)
	{
		clear_dat(dat);
		if (interactive(dat))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		q = input_get(dat);
		if (q != -1)
		{
			set_dat(dat, av);
			ret_built = builtin_find(dat);
			if (ret_built == -1)
				get_cmd(dat);
		}
		else if (interactive(dat))
			_putchar('\n');
		free_dat(dat, 0);
	}
	write_history(dat);
	free_dat(dat, 1);
	if (!interactive(dat) && dat->status)
		exit(dat->status);
	if (ret_built == -2)
	{
		if (dat->err_num == -1)
			exit(dat->status);
		exit(dat->err_num);
	}
	return (ret_built);
}

/**
 * builtin_find - finds a builtin command
 * @dat: parameter
 *
 * Return: -1 if builtin not found.
 */
int builtin_find(data *dat)
{
	int p, builtinret = -1;
	table tbl[] = {
		{"exit", _exit},
		{"env", _env},
		{"help", _help},
		{"history", _history},
		{"setenv", _setenv},
		{"unsetenv", _unsetenv},
		{"cd", _cd},
		{"alias", _alias},
		{NULL, NULL}
	};

	for (p = 0; tbl[i].type; p++)
		if (_strcmp(dat->argv[0], tbl[p].type) == 0)
		{
			dat->ln_count++;
			builtinret = tbl[i].func(dat);
			break;
		}
	return (builtinret);
}

/**
 * get_cmd - finds a command in PATH
 * @dat: the parameter & return dat struct
 *
 * Return: void
 */
void get_cmd(data *dat)
{
	char *path = NULL;
	int p, n;

	dat->path = dat->argv[0];
	if (dat->linecount_flag == 1)
	{
		dat->ln_count++;
		dat->linecount_flag = 0;
	}
	for (p = 0, n = 0; dat->arg[p]; p++)
		if (!_isdelim(dat->arg[i], " \t\n"))
			n++;
	if (!n)
		return;

	path = _findpath(dat, get_env(dat, "PATH="), dat->argv[0]);
	if (path)
	{
		dat->path = path;
		_forkcmd(dat);
	}
	else
	{
		if ((interactive(dat) || get_env(dat, "PATH=")
					|| dat->argv[0][0] == '/') && _iscmd(dat, dat->argv[0]))
			_forkcmd(dat);
		else if (*(dat->arg) != '\n')
		{
			dat->status = 127;
			error_print(dat, "not found\n");
		}
	}
}

/**
 * _forkcmd - forks a an exec thread to run cmd
 * @dat: the parameter & return dat struct
 *
 * Return: void
 */
void _forkcmd(data *dat)
{
	pid_t new_pid;

	new_pid = fork();
	if (new_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (new_pid == 0)
	{
		if (execve(dat->path, dat->argv, get_environ(dat)) == -1)
		{
			free_dat(dat, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(dat->status));
		if (WIFEXITED(dat->status))
		{
			dat->status = WEXITSTATUS(dat->status);
			if (dat->status == 126)
				error_print(dat, "Permission denied\n");
		}
	}
}
