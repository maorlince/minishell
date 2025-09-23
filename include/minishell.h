/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:21:04 by manon             #+#    #+#             */
/*   Updated: 2025/09/23 16:36:55 by manon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//flag makefile -lreadline pour readline  et -ltermcap ou -lncurses pour term.h

# include <unistd.h>// read close fork execve getcwd chdir dup2 pipe access
# include <stdlib.h>// exit malloc free getenv
# include <stdio.h>// printf perror
# include <string.h>// strerror
# include <sys/types.h>// wait waitpid pid_t size_t
# include <sys/wait.h>// wait?
# include <signal.h>// kill signal
# include <fcntl.h>// open
# include <readline/readline.h>// readline et rl[...]
# include <readline/history.h>// [...]history
# include "../libft/libft.h"

# define UNKNOWN		0	//unknown token
# define INPUT			1	//"<"redirection infile
# define HEREDOC		2	//"<<"redirection infile multi-line
# define OUTPUT			3	//">"redirection outfile
# define APPEND			4	//">>"redirection outfile multi-line
# define PIPE			5	//"|"redirige stdout cmd1 vers un stdin cmd2
# define WORD			6	//in quotes
# define CMD			7	//commandes
# define ARG			8	//arguments
# define ENV			9	//environnement

# define CYAN			"\033[1;36m"
# define RESET 			"\033[0m"

typedef struct s_token
{
	char			*value;
	int				type; //0: unknown, 1: input, 2: heredoc, 3: output etc
	int				quotes; //0: no quotes, 1: single quotes, 2: double quotes
	int				size; // size of the token
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	int				type; //1: input, 2: heredoc, 3: output, 4: append
	char			*file; // filename for redirection
	char			*heredoc_content; // content for heredoc
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**argv;
	char			*infile; // "exemple" < "exemple_infile"
	char			*outfile; // "exemple" > "exemple_outfile"
	//char			*heredoc_content; // "exemple" << "exemple_heredoc"
	int				content; //1: append, 0: truncate
	t_redir			*redirections; // list of redirections
	struct s_cmd	*next; // next command, after a pipe
}	t_cmd;

typedef struct s_env
{
	char			*type; // can be : USER, HOME, PATH, etc.
	char			*value;
	int				last_exit; // to store the last exit status ($?)
	struct s_env	*next;
}	t_env;

//free.c
void	free_tokens(t_token *tokens);
//static void	free_redir(t_redir *redir);
void	free_cmds(t_cmd *cmds);
void	free_tab(char **tabs);
void	free_env_list(t_env *env);

//exec_utils.c
char	*build_full_path(char *dir, const char *name);
int		is_parent_builtin(t_cmd *cmd);
void	update_fds(int *in_fd, int fd[2], t_cmd *cmd);
void	wait_all(pid_t *pids, int count, t_env **env);

//exec.c : fonctions d'exécution
//static char	*find_command(const char *name, t_env *env);
//static void	exec_external(t_cmd *cmd, t_env **env, t_cmd *cmd_list, ...);
//static void	child_process(t_cmd *cmd, t_env **env, t_cmd *cmd_list);
//static pid_t	launch_child(t_cmd *cmd, t_env **env, int in_fd, int *fd);
int		execute_commands(t_cmd *cmd_list, t_env **env, int i);

//builtins.c : fonctions des builtins
int		builtin_echo(char **argv, int i, int j, int new_line);
int		builtin_pwd(void);
int		builtin_cd(char **argv);
int		builtin_env(t_env *env);

//builtins_utils.c
//static int	builtin_export(char **argv, t_env **env);
//static int	builtin_unset(char **argv, t_env **env);
//static int	builtin_exit(char **argv, t_env **env);
int		ft_strcmp(const char *s1, const char *s2);
int		exec_builtin(t_cmd *cmd, t_env **env);

//env.c : fonctions liées à l'environnement
t_env	*env_new(char *type, char *value);
void	env_add_back(t_env **env, t_env *new);
char	*env_get(t_env *env, char *type);
void	env_set(t_env **env, char *type, char *value);
void	env_unset(t_env **env, char *type);

//env_utils.c : fonctions utilitaires pour l'environnement
//static int env_count(t_env *env);
t_env	*build_env_list(char **envp);
char	**env_list_to_tab(t_env *env);
void	ft_lstadd_back_redir(t_redir **lst, t_redir *new);
t_redir	*ft_lstnew_redir(char *file, int type);

//expand.c : gere les expansion des variables d'environnement
char	*get_variable_value(char *str, t_env *env, char *result);
int		get_variable_length(char *str);
//static char join_char(char *result, char c);
char	*expand_variable(t_token *token, t_env *env);
int		expand_tokens(t_token *tokens, t_env *env);

//lexer_utils.c : fonctions utilitaires pour le lexer
int		is_token(char c);
int		get_type(char *line);
char	*strip_quotes(const char *src, size_t i);
int		get_size(char *line, int i);

//lexer.c : tokenize la ligne de commande
//static int	add_new_token(t_token **head, t_token **current...);
t_token	*create_token(char *line, int i, int size);
int		lexer(t_token **token_list, t_env *env, char *line);

//parser_utils.c : fonctions utilitaires
int		count_args(t_token *token);
char	**fill_argv(t_token *token, int argc);
void	copy_redirections(t_token *token, t_cmd *cmd);
void	append_line(char **content, char *line);
char	*get_heredoc_content(char *delimiter);

//utils.c : fonctions utilitaires diverses
t_redir	*ft_lstnew_redir(char *file, int type);
void	ft_lstadd_back_redir(t_redir **lst, t_redir *new);

//parser.c : crée les commandes à partir des tokens
t_cmd	*create_command(t_token **token_list);
int		setup_heredoc(char *content);
int		setup_redirections(t_cmd *cmd);
t_cmd	*parse_tokens(t_token *tokens);

#endif
