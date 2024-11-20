/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skirakos <skirakos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:23:51 by skirakos          #+#    #+#             */
/*   Updated: 2024/11/20 19:18:02 by skirakos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <fcntl.h>
# include <termios.h>
# include <limits.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>

# define INVALID_ARG_CNT 1
# define SYNTAX_ERR 2
# define FILEIN_ERR 3
# define QUOTE_ERR 4
# define MALLOC_ERR 5
# define PIPE_ERR 6
# define FORK_ERR 7
# define DUP_ERR 8
# define CMD_NOT_FOUND 9
# define EXECVE_ERR 10

extern int					g_exit_status;

typedef struct s_env		t_env;
typedef struct s_split		t_split;
typedef struct s_minishell	t_minishell;

typedef enum e_token_type
{
	WORD,
	S_PIPE,
	D_PIPE,
	S_AND,
	D_AND,
	D_QUOTE,
	S_QUOTE,
	IN_REDIR,
	OUT_REDIR,
	HERE_DOC,
	APPEND_REDIR,
	ERROR,
	NONE,
	FILEIN,
	LIMITER,
	FILEOUT,
	APPEND_FILEOUT
}	t_token_type;

typedef struct s_split
{
	char			*value;
	t_token_type	type;
	struct s_split	*next;
}	t_split;

typedef struct s_env
{
	char	*var;
	char	*value;
	t_env	*next;
}	t_env;

typedef struct s_merge_var
{
	int		n1;
	int		n2;
	int		k;
	int		i;
	int		j;
	char	**l;
	char	**r;
	char	**arr;
}	t_merge_var;

typedef struct s_vars
{
	int		start;
	int		end;
	char	current_quote;
	char	*input;
}	t_vars;

typedef struct s_minishell
{
	t_split	*tokens;
	t_env	*env;
	int		(*fd)[2];
	int		fd_in;
	int		fd_out;
	int		fd_heredoc;
	int		is_builtin;
	int		file_err;
	int		pipes;
	pid_t	*pid;
	char	**cmd;
}	t_minishell;

//********************************************************//
//******************MINISHELL.C***************************//
//********************************************************//

t_minishell		*minishell_init(char **env);

//********************************************************//
//******************ENV_COPY.C****************************//
//********************************************************//

int				get_value(t_env **copy, char **env, int i, int *j);
t_env			*ft_lstnew_env(char *content, char *type);
int				env_copy_helper_2(t_env *copy, char **env, int i, int *j);
int				env_copy_helper_1(t_env *copy, char **env);
t_env			*env_copy(char **env);

//********************************************************//
//******************TOKENIZATION.C************************//
//********************************************************//

char			*get_from_env(t_env *env, char *key);
void			quote_remover(t_split *item);
char			*merge(char *before_key, char *dollar_value, char *after_key);
char			*sedastan(char *str, int i, t_env *env, int end);
void			dollar_sign(t_split *item, t_env *env);
void			type_operator(t_split **item, char *input, int *i);
t_split			*remove_empty_nodes(t_split *item);
int				check_operation(t_split **item);
void			tokenization(char *input, t_minishell *minishell);

//********************************************************//
//******************UTILS_1.C*****************************//
//********************************************************//

int				ft_strncmp(const char *s1, const char *s2, size_t n);
char			*ft_strjoin(char *s1, char *s2);
void			ft_strcpy(char *dst, char *src);
char			*ft_strdup(char *s1);
int				ft_strcmp(const char *s1, const char *s2);

//********************************************************//
//******************UTILS_2.C*****************************//
//********************************************************//

void			ft_putstr_fd(char const *s, int fd);
int				isbuiltin(char *cmd_name);
int				is_builtin(char *cmd);
int				search_slash(char *str);

//********************************************************//
//******************UTILS_3.C*****************************//
//********************************************************//

t_split			*ft_lstnew(char *content);
int				ft_strlen(char *str);
void			ft_strcut(t_split *item, char *input, int start, int end);;
char			*ft_substr(char *s, int start, int end);

//********************************************************//
//******************BUILT_IN.C****************************//
//********************************************************//

t_split			*pre_execve(t_minishell *minishell);
void			builtin_helper(t_minishell *minishell);
void			initialization(t_minishell *minishell);
void			built_in(t_minishell *minishell, int curr);

//********************************************************//
//******************MATRIX.C******************************//
//********************************************************//

void			create_envp(t_minishell *minishell, char **envp);
char			**env_to_matrix(t_minishell *minishell);
void			handle_redirection(t_minishell *minishell,
					int type, char *file_name);
void			create_matrix(t_minishell *minishell,
					t_split **tmp, char **matrix);

//********************************************************//
//******************FT_SPLIT.C****************************//
//********************************************************//

char			*ft_strncpy(char *s1, char *s2, int n);
char			**start_malloc(char *str, char **out, int j, char c);
char			**ft_split(char *str, char c);

//********************************************************//
//******************ECHO.C********************************//
//********************************************************//

int				check_newline(char	*str);
void			echo(char **args);

//********************************************************//
//******************CD.C**********************************//
//********************************************************//

int				access_directory(const char	*path);
bool			is_directory(const char	*path);
void			update_env(t_env *env, char *pwd_to_set, int flag);
char			*tilda(t_minishell *minishell);
void			cd(t_minishell *minishell, char *path);

//********************************************************//
//******************PWD_ENV.C*****************************//
//********************************************************//

void			env(t_minishell *minishell);
void			pwd(void);

//********************************************************//
//******************UNSET.C*******************************//
//********************************************************//

int				unset_first_node(t_minishell *minishell, t_env *tmp, int *i);
int				unset_node(t_minishell *minishell,
					t_env *node_to_delete, t_env **prev, int i);
void			unset_helper(t_minishell *minishell, t_env *tmp,
					t_env *prev, t_env *node_to_delete);
void			unset(t_minishell *minishell);

//********************************************************//
//******************EXIT.C********************************//
//********************************************************//

int				is_valid(char	*status);
void			exit_shell(t_minishell *minihell);

//********************************************************//
//******************FT_ATOI.C*****************************//
//********************************************************//

void			check_num_is_valid(const char *str,
					unsigned long long nbr, int sign);
long			result(char *str, size_t i, char sign);
int				ft_atoi(char *str);

//********************************************************//
//******************EXPORT.C******************************//
//********************************************************//

void			export_print_helper(char **envp, int i, int j, int flag);
char			**str_divide_by_equal_sign(char *str);
void			exec_export(t_minishell *minishell, int i);
void			print_env(char **envp);
void			export_bulki(t_minishell *minishell, char **envp);

//********************************************************//
//******************EXPORT1.C*****************************//
//********************************************************//

int				check_underscore(char *cmd, int *i);
void			fook(char *str, char **res, int i);
void			for_copy(char *str, char **res);
void			export_helper(t_minishell *minishell, char **split,
					char *new_value, int i);

//********************************************************//
//******************EXPORT2.C*****************************//
//********************************************************//

int				is_digit(char c);
int				check_in_env(t_env *env, char *str);
int				contains_char(char *str, char c);
int				is_valid_var(char *var);

//********************************************************//
//******************EXPORT_HELPERS.C**********************//
//********************************************************//

void			export_helper2(t_minishell *minishell, char **split,
					t_env *tmp, char *new_value);
void			export_helper3(t_env *tmp, char **split);
void			export_helper4(t_minishell *minishell,
					t_env *tmp, char **split, int i);
void			export_helper5(t_minishell *minishell,
					t_env *tmp, char **split, int i);
void			export_helper6(t_env *tmp, char **split);

//********************************************************//
//******************MERGE_SORT.C**************************//
//********************************************************//

void			init_merge_var(t_merge_var *var, int left, int right, int mid);
void			merge_sorted_arrays(char **arr, int left, int mid, int right);
void			merge_sort(char **arr, int left, int right);

//********************************************************//
//******************FREE.C********************************//
//********************************************************//

void			free_t_split(t_split *tokens);
void			free_t_env(t_env *env);
int				matrix_len(char **matrix);
void			*free_matrix(char **matrix, int rows);
void			free_before_exit(t_minishell *minishell);

//********************************************************//
//******************HERE_DOC.C****************************//
//********************************************************//

void			here_doc_write(int fd, char *limiter);
int				here_doc_open(char *limiter);

//********************************************************//
//******************PIPEX.C*******************************//
//********************************************************//

int				init_pipe_fd(t_minishell *minishell, int pipe_count);
void			close_fd(t_minishell *minishell, int pipe_count);
void			ft_dups(t_minishell *minishell, int pipes, int curr);

//********************************************************//
//******************REDIRS.C******************************//
//********************************************************//

void			in_redir(t_minishell *minishell);
void			out_redir(t_minishell *minishell);
void			redirs(t_minishell *minishell);

//********************************************************//
//******************SIGNALS.C*****************************//
//********************************************************//

void			*ft_memset(void *b, int c, size_t len);
void			set_sig_before_rl(void);
void			set_sig_after_rl(void);

//********************************************************//
//******************SIGNALS_HELPER.C**********************//
//********************************************************//

void			disable_echoctl(void);
void			sig_newline(int signal);
void			ignore_bckslash(void);
void			signal_reset_prompt(int sig);

//********************************************************//
//******************SYNTAX.C******************************//
//********************************************************//

int				is_ctrl_op(t_split	*token);
int				is_permited(t_split *token);
void			print_err(int exit_status, char *msg1,
					char *msg2, char *msg3);
void			perror_exit(t_minishell *minishell,
					int err_code, char *msg, int fork);
int				syntax_check(t_minishell *minishell,
					t_split *tokens);

//********************************************************//
//******************FT_ITOA.C*****************************//
//********************************************************//

char			*ft_itoa(int n);

//********************************************************//
//******************CMD_1.C*******************************//
//********************************************************//

void			commands_helper(t_minishell *minishell, char **envp);
void			commands(t_minishell *minishell, char **envp);
char			**clean_cmd(char **cmd);
void			check_cmd(t_minishell *minishell);

//********************************************************//
//******************CMD_2.C*******************************//
//********************************************************//

int				check_cmd_helper_2(t_minishell *minishell,
					char **path, t_env *tmp, int i);
int				check_cmd_helper_1(t_minishell *minishell);

//********************************************************//
//******************FORK.C********************************//
//********************************************************//

void			fork_helper(t_minishell *minishell, char **envp,
					int curr, int pid);
void			wait_processes(t_minishell *minishell, int pipes);
int				pipe_count(t_minishell *minishell);
void			pipes_zero(t_minishell *minishell, char **envp);
void			end_of_fork(t_minishell *minishell, t_split *tmp, int pid);

//********************************************************//
//******************OPEN_CHECK_FILE.C*********************//
//********************************************************//

int				check_redir(char *str);
int				open_file(char *fn, int type, t_minishell *minishell, int fd);
int				check_folder(char *str);
int				check_accsess(char *str);
void			access_failed(t_minishell *minishell);

#endif