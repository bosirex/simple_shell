#include "shell.h"

/**
 * get_environ - returns the string array copy of our environ
 * @dat: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **get_environ(data_t *dat)
{
	if (!dat->environ || dat->env_changed)
	{
		dat->environ = lst_to_str(dat->env);
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
int _unsetenv(data_t *dat, char *var)
{
	lst_t *nd = dat->env;
	size_t k = 0;
	char *p;

	if (!nd || !var)
		return (0);

	while (nd)
	{
		p = _startswith(nd->str, var);
		if (p && *p == '=')
		{
			dat->env_changed = delete_nd_at_index(&(dat->env), k);
			k = 0;
			nd = dat->env;
			continue;
		}
		nd = nd->next;
		k++;
	}
	return (dat->env_changed);
}

/**
 * _setenv - Initialize a new environment variable,
 *             or modify an existing one
 * @dat: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @val: the string env var value
 *  Return: Always 0
 */
int _setenv(data_t *dat, char *var, char *val)
{
	char *buff = NULL;
	lst_t *nd;
	char *b;

	if (!var || !val)
		return (0);

	buff = malloc(string_len(var) + string_len(val) + 2);
	if (!buff)
		return (1);
	string_cpy(buff, var);
	string_cat(buff, "=");
	string_cat(buff, val);
	nd = dat->env;
	while (nd)
	{
		b = _startswith(nd->str, var);
		if (b && *b == '=')
		{
			free(nd->str);
			nd->str = buff;
			dat->env_changed = 1;
			return (0);
		}
		nd = nd->next;
	}
	add_nd_end(&(dat->env), buff, 0);
	free(buff);
	dat->env_changed = 1;
	return (0);
}
