#include "shell.h"

/**
 * get_hist_file - gets the history file
 * @dat: parameter struct
 *
 * Return: allocated string containg history file
 */

char *get_hist_file(data *dat)
{
	char *buf, *dr;

	dr = get_env(dat, "HOME=");
	if (!dr)
		return (NULL);
	buf = malloc(sizeof(char) * (_strlen(dr) + _strlen(HIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	_strcpy(buf, dr);
	_strcat(buf, "/");
	_strcat(buf, HIST_FILE);
	return (buf);
}

/**
 * wrt_history - creates a file, or appends to an existing file
 * @dat: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int wrt_history(data *dat)
{
	ssize_t fd;
	char *file_name = get_hist_file(dat);
	lst_t *nd = NULL;

	if (!file_name)
		return (-1);

	fd = open(file_name, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(file_name);
	if (fd == -1)
		return (-1);
	for (nd = dat->history; nd; nd = nd->next)
	{
		_putsfd(nd->str, fd);
		_putfd('\n', fd);
	}
	_putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * history_read - reads history from file
 * @dat: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int history_read(data *dat)
{
	int y, lst = 0, line_count = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stt st;
	char *buf = NULL, *file_name = get_hist_file(dat);

	if (!file_name)
		return (0);

	fd = open(file_name, O_RDONLY);
	free(file_name);
	if (fd == -1)
		return (0);
	if (!fstt(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; y < fsize; i++)
		if (buf[y] == '\n')
		{
			buf[y] = 0;
			build_history_list(dat, buf + lst, line_count++);
			lst = i + 1;
		}
	if (lst != y)
		build_history_list(dat, buf + lst, line_count++);
	free(buf);
	dat->histcount = line_count;
	while (dat->histcount-- >= HIST_MAX)
		delete_nd_at_index(&(dat->history), 0);
	renumber_history(dat);
	return (dat->histcount);
}

/**
 * build_history_list - adds entry to a history linked list
 * @dat: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @line_count: the history line_count, histcount
 *
 * Return: Always 0
 */
int build_history_list(data *dat, char *buf, int line_count)
{
	lst_t *nd = NULL;

	if (dat->history)
		nd = dat->history;
	add_nd_end(&nd, buf, line_count);

	if (!dat->history)
		dat->history = nd;
	return (0);
}

/**
 * renumber_history - renumbers the history linked list after changes
 * @dat: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int renumber_history(data *dat)
{
	lst_t *nd = dat->history;
	int i = 0;

	while (nd)
	{
		nd->num = i++;
		nd = nd->next;
	}
	return (dat->histcount = i);
}
