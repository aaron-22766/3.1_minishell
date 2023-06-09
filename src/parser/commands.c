#include "../../include/minishell/parser.h"

static void	ft_args_to_table(t_tokens *tokens, t_cmds *command)
{
	size_t	argvs;

	argvs = 0;
	while (tokens)
	{
		if (tokens->id == COMMAND || tokens->id == WORD)
			command->argv[argvs++] = tokens->content;
		else if (tokens->id == PIPE)
		{
			command = command->next;
			argvs = 0;
		}
		tokens = tokens->next;
	}
}

static void	ft_alter_tokens(t_tokens **tokens)
{
	t_tokens	*prev;
	t_tokens	*tok;
	t_tokens	*next;

	prev = NULL;
	tok = *tokens;
	while (tok)
	{
		next = tok->next;
		if ((tok->id & OPERATOR) && tok->id != PIPE)
			free(tok->content);
		if (((tok->id & OPERATOR) && tok->id != PIPE)
			|| tok->id == COMMAND || tok->id == WORD)
		{
			free(tok);
			if (prev)
				prev->next = next;
			else
				*tokens = next;
		}
		else
			prev = tok;
		tok = next;
	}
}

static void	ft_redirections_to_table(t_tokens *tokens, t_cmds *command)
{
	t_tokens	*prev;
	t_tokens	*next;

	prev = NULL;
	while (tokens)
	{
		if (tokens->id != PIPE)
			command->io_red = tokens;
		while (tokens && tokens->id != PIPE)
		{
			prev = tokens;
			tokens = tokens->next;
		}
		if (!tokens)
			return ;
		command = command->next;
		if (prev)
			prev->next = NULL;
		next = tokens->next;
		free(tokens->content);
		free(tokens);
		tokens = next;
	}
}

t_cmds	*ft_create_commands(t_tokens *tokens)
{
	t_cmds	*table;

	table = ft_allocate_command_table(tokens);
	if (!table)
		return (ft_perror(ERR_MEM, "creating command table"), NULL);
	ft_args_to_table(tokens, table);
	ft_alter_tokens(&tokens);
	ft_redirections_to_table(tokens, table);
	return (table);
}
