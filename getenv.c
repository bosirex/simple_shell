#include "shell.h"

/**
 * get_environ - returns the string array copy of our environ
 * @dat: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **get_environ(data *dat)
{
	if (!dat->environ || dat->env_changed)
	{
		dat->environ = list_to_string(dat->env);
		dat->env_changed = 0;
	}

	return (dat->environ);
}

/**
 * _unsetenv - Remove an env variable
 * @dat: Structure having potential arguments.
 * @var: the string env var property
 * 
 *  Return: 1 on delete, 0 otherwise
 */
int _unsetenv(data *dat, char *var)
{
	lst_t *nd = dat->env;
	size_t i = 0;
	char *p;

	if (!nd || !var)
		return (0);

	while (nd)
	{
		p = _startswith(nd->str, var);
		if (p && *p == '=')
		{
			dat->env_changed = delete_nd_at_index(&(dat->env), i);
			i = 0;
			nd = dat->env;
			continue;
		}
		nd = nd->next;
		i++;
	}
	return (dat->env_changed);
}

/**
 * _setenv - Initialize a new environment variable,
 *             or modify an existing one
 * @dat: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int _setenv(data *dat, char *var, char *value)
{
	char *buf = NULL;
	lst_t *nd;
	char *b;

	if (!var || !value)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	nd = dat->env;
	while (nd)
	{
		b = _startswith(nd->str, var);
		if (b && *b == '=')
		{
			free(nd->str);
			nd->str = buf;
			dat->env_changed = 1;
			return (0);
		}
		nd = nd->next;
	}
	add_nd_end(&(dat->env), buf, 0);
	free(buf);
	dat->env_changed = 1;
	return (0);
}
