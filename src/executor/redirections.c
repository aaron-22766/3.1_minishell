#include "../../include/minishell/executor.h"

static void	ft_heredoc_handler(int sig)
{
	ft_printf("\n");
	rl_replace_line(NULL, 0);
	(void)sig;
}

static int	ft_heredoc(unsigned char status, char *eof)
{
	int		fd[2];
	char	*prompt;
	char	*line;

	if (pipe(fd) == -1)
		return (ft_perror(ERR_ERRNO, "failed to create heredoc file"), -1);
	prompt = getenv("PS2");
	if (!prompt)
		prompt = "> ";
	while (true)
	{
		signal(SIGINT, ft_heredoc_handler);
		line = readline(prompt);
		signal(SIGINT, ft_sig_handler);
		if (!line && rl_eof_found)
			ft_printf("\033[A\033[K%s", prompt);
		if (!line || !ft_strcmp(line, eof))
			return (free(line), close(fd[1]), fd[0]);
		if (ft_expand_env_vars(status, &line, "", 0) != RMV)
			ft_putstr_fd(line, fd[1]);
		ft_putstr_fd("\n", fd[1]);
		free(line);
	}
}

static char	ft_set_redirection(int status, t_tokens *red, t_cmds *cmd)
{
	int	fd;

	fd = -1;
	if (red->id == OUT_FILE || red->id == OUT_A_FILE)
		fd = open(red->content, (O_TRUNC * (red->id == OUT_FILE))
				| O_WRONLY | O_APPEND | O_CREAT, 0644);
	else if (red->id == IN_FILE)
		fd = open(red->content, O_RDONLY);
	else if (red->id == HEREDOC_EOF)
		fd = ft_heredoc(status, red->content);
	if (fd == -1 && red->id != HEREDOC_EOF)
		ft_perror(ERR_ERRNO, red->content);
	if (fd == -1)
		return (RETURN_FAILURE);
	if (red->id == OUT_FILE || red->id == OUT_A_FILE)
	{
		if (cmd->fd_out != STDOUT_FILENO)
			close(cmd->fd_out);
		cmd->fd_out = fd;
		return (RETURN_SUCCESS);
	}
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	cmd->fd_in = fd;
	return (RETURN_SUCCESS);
}

char	ft_install_redirections(int status, t_cmds *cmd)
{
	t_tokens	*red;

	red = cmd->io_red;
	while (red)
	{
		if (ft_set_redirection(status, red, cmd) == RETURN_FAILURE)
			return (RETURN_FAILURE);
		red = red->next;
	}
	if (cmd->fd_in != STDIN_FILENO && dup2(cmd->fd_in, STDIN_FILENO) == -1)
		return (ft_perror(ERR_ERRNO, "failed to redirect"), RETURN_FAILURE);
	if (cmd->fd_out != STDOUT_FILENO && dup2(cmd->fd_out, STDOUT_FILENO) == -1)
		return (ft_perror(ERR_ERRNO, "failed to redirect"), RETURN_FAILURE);
	return (RETURN_SUCCESS);
}
