/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manon <manon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:21:04 by manon             #+#    #+#             */
/*   Updated: 2025/09/11 20:55:34 by manon            ###   ########.fr       */
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

//#define EXIT_SUCCESS 0
//#define EXIT_FAILURE 1

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
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**argv;
	char			*infile; // "exemple" < "exemple_infile"
	char			*outfile; // "exemple" > "exemple_outfile"
	char			*heredoc_content; // "exemple" << "exemple_heredoc"
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

//utils.c : fonctions utilitaires
void	free_tokens(t_token *tokens);
void	free_cmds(t_cmd *cmds);
void	free_split(char **split);

//exec.c : fonctions d'exécution
//static char	*build_full_path(char *dir, const char *name);
char	*find_command(const char *name, t_env *env);
//static int	is_parent_builtin(t_cmd *cmd);
int		execute_commands(t_cmd *cmd_list, t_env **env);

//builtins.c : fonctions des builtins
int		builtin_echo(char **argv);
int		builtin_pwd(void);
int		builtin_cd(char **argv);
int		builtin_env(t_env *env);

//builtins_utils.c
//static int	builtin_export(char **argv, t_env **env);
//static int	builtin_unset(char **argv, t_env **env);
//static int	builtin_exit(char **argv, t_env **env);
int		exec_builtin(t_cmd *cmd, t_env **env);

//env.c : fonctions liées à l'environnement
t_env	*env_new(char *type, char *value);
void	env_add_back(t_env **env, t_env *new);
char	*env_get(t_env *env, char *type);
void	env_set(t_env **env, char *type, char *value);
void	env_unset(t_env **env, char *type);

//env_utils.c : fonctions utilitaires pour l'environnement
t_env	*build_env_list(char **envp);
//static int env_count(t_env *env);
char	**env_list_to_tab(t_env *env);
void	free_str_tab(char **tabs);
void	free_env_list(t_env *env);
void	ft_lstadd_back_redir(t_redir **lst, t_redir *new);
t_redir	*ft_lstnew_redir(char *file, int type);

//expand.c : gere les expansion des variables d'environnement
char	*expand_variable(t_token *token, t_env *env, int i);
int		expand_tokens(t_token *tokens, t_env *env);

//lexer.c : tokenize la ligne de commande
int		is_token(char c);
int		get_type(char *line);
int		handle_quotes(char *line);
int		get_size(char *line);
t_token	*create_token(char *line, int i, int size);
int		lexer_loop(t_token **token_list, t_env *env, char *line);
t_token	*lexer(char *line, t_env *env);

//parser.c : crée les commandes à partir des tokens
int		count_args(t_token *token);
char	**fill_argv(t_token *token, int argc);
char	*get_heredoc_content(char *delimiter);
void	copy_redirections(t_token *token, t_cmd *cmd);
t_cmd	*create_command(t_token **token_list);
int		setup_heredoc(char *content);
int		setup_redirections(t_cmd *cmd);
t_cmd	*parse_tokens(t_token *tokens);

#endif
