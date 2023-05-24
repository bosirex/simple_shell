#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convt_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next nd
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} lst_t;

typedef struct passdat
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int ln_count;
	int err_num;
	int line_count_flag;
	char *fname;
	lst_t *env;
	lst_t *history;
	lst_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} data;

#define dat_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
		0, 0, 0}

/**
 * struct builtin - contains a builtin string and related function
 * @type: the builtin command flag
 * @func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(dat_t *);
} table;


/* toem_shloop.c */
int main_shell(dat_t *, char **);
int builtin_find(dat_t *);
void get_cmd(dat_t *);
void _forkcmd(dat_t *);

/* toem_parser.c */
int _iscmd(dat_t *, char *);
char *dup_chars(char *, int, int);
char *_findpath(dat_t *, char *, char *);

/* loopmain_shell.c */
int loopmain_shell(char **);

/* toem_errors.c */
void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* toem_string.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *_startswith(const char *, const char *);
char *_strcat(char *, char *);

/* toem_string1.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* toem_exits.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* toem_tokenizer.c */
char **_strtow(char *, char *);
char **_strtow2(char *, char);

/* toemreal_loc.c */
char *mem_set(char *, char, unsigned int);
void f_free(char **);
void *real_loc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int b_free(void **);

/* toem_atoi.c */
int interactive(dat_t *);
int _isdelim(char, char *);
int _isalpha(int);
int _atoi(char *);

/* toem_errors1.c */
int _erratoi(char *);
void error_print(dat_t *, char *);
int print_d(int, int);
char *convt_number(long int, int, int);
void remove_comments(char *);

/* toem_builtin.c */
int _myexit(dat_t *);
int _cd(dat_t *);
int _help(dat_t *);

/* toem_builtin1.c */
int _history(dat_t *);
int _alias(dat_t *);

/*toem_getline.c */
ssize_t input_get(dat_t *);
int _getline(dat_t *, char **, size_t *);
void sigintHandler(int);

/* toem_getdat.c */
void clear_dat(dat_t *);
void set_dat(dat_t *, char **);
void free_dat(dat_t *, int);

/* toem_environ.c */
char *get_env(dat_t *, const char *);
int _env(dat_t *);
int _setenv(dat_t *);
int _unsetenv(dat_t *);
int env_list_populate(dat_t *);

/* toemget_env.c */
char **get_environ(dat_t *);
int _unsetenv(dat_t *, char *);
int _setenv(dat_t *, char *, char *);

/* toem_history.c */
char *get_hist_file(dat_t *dat);
int wrt_history(dat_t *dat);
int history_read(dat_t *dat);
int build_history_list(dat_t *dat, char *buf, int line_count);
int renumber_history(dat_t *dat);

/* toem_lists.c */
lst_t *add_nd(lst_t **, const char *, int);
lst_t *add_nd_end(lst_t **, const char *, int);
size_t print_list_str(const lst_t *);
int delete_nd_at_index(lst_t **, unsigned int);
void free_list(lst_t **);

/* toem_lists1.c */
size_t length_list(const lst_t *);
char **list_to_string(lst_t *);
size_t print_list(const lst_t *);
lst_t *nd_startswith(lst_t *, char *, char);
ssize_t get_nd_index(lst_t *, lst_t *);

/* toem_vars.c */
int _ischain(dat_t *, char *, size_t *);
void check_chain(dat_t *, char *, size_t *, size_t, size_t);
int replace_alias(dat_t *);
int vars_replace(dat_t *);
int _replacestring(char **, char *);

#endif
