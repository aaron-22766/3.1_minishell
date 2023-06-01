#include "../../include/minishell/minishell.h"

static void	ft_print_error(int err, char *context)
{
	if (err == ERR_MEM)
		ft_eprintf("memory allocation failed while %s", context);
	else if (err == ERR_TOKEN_SYNTAX)
		ft_eprintf("syntax error near unexpected token `%s'", context);
}

void	ft_perror_exe(char *exe, int err, char *context)
{
	ft_eprintf("%s: ", exe);
	ft_print_error(err, context);
	ft_eprintf("\n");
}

void	ft_perror(int err, char *context)
{
	ft_perror_exe("minishell", err, context);
}
