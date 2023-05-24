#include "shell.h"

/**
 * _history - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @dat: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _history(dat_t *dat)
{
	print_list(dat->history);
	return (0);
}

/**
 * unset_alias - sets alias to string
 * @dat: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias(dat_t *dat, char *str)
{
	char *p, c;
	int ret;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_nd_at_index(&(dat->alias),
		get_nd_index(dat->alias, nd_startswith(dat->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - sets alias to string
 * @dat: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(dat_t *dat, char *str)
{
	char *p;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(dat, str));

	unset_alias(dat, str);
	return (add_nd_end(&(dat->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @nd: the alias nd
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(lst_t *nd)
{
	char *p = NULL, *a = NULL;

	if (nd)
	{
		p = _strchr(nd->str, '=');
		for (a = nd->str; a <= p; a++)
		_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * _alias - mimics the alias builtin (man alias)
 * @dat: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _alias(data *dat)
{
	int i = 0;
	char *p = NULL;
	lst_t *nd = NULL;

	if (dat->argc == 1)
	{
		nd = dat->alias;
		while (nd)
		{
			print_alias(nd);
			nd = nd->next;
		}
		return (0);
	}
	for (i = 1; dat->argv[i]; i++)
	{
		p = _strchr(dat->argv[i], '=');
		if (p)
			set_alias(dat, dat->argv[i]);
		else
			print_alias(nd_startswith(dat->alias, dat->argv[i], '='));
	}

	return (0);
}
