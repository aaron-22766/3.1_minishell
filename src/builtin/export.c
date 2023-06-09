#include "../../include/minishell/builtin.h"

static void	ft_print_var(char *var)
{
	int	i;

	i = ft_strchr_index(var, '=');
	if (i == -1)
	{
		printf("declare -x %s\n", var);
		return ;
	}
	printf("declare -x %.*s=\"", i, var);
	while (var[++i])
	{
		if (var[i] == '\\')
			printf("\\");
		printf("%c", var[i]);
	}
	printf("\"\n");
}

void	ft_print_export(void)
{
	extern char	**environ;
	char		*prev;
	char		*current;
	size_t		i;
	size_t		j;

	if (!environ)
		exit(1);
	prev = "";
	i = -1;
	while (environ[++i])
	{
		current = NULL;
		j = -1;
		while (environ[++j])
		{
			if (ft_strcmp(environ[j], prev) > 0
				&& (!current || ft_strcmp(environ[j], current) < 0))
				current = environ[j];
		}
		ft_print_var(current);
		prev = current;
	}
	exit(0);
}

int	ft_export(t_cmds *cmd)
{
	int		status;
	size_t	i;

	if (cmd->argv[1][0] == '-')
		return (ft_eprintf("minishell: export: -%c: invalid option\n",
				cmd->argv[1][1]), 2);
	status = 0;
	i = 1;
	while (cmd->argv[i])
	{
		if (ft_valid_identifier(cmd->argv[i], '=') == false)
		{
			ft_eprintf("minishell: export: `%s': not a valid identifier\n",
				cmd->argv[i]);
			status = 1;
		}
		else if (ft_putenv(cmd->argv[i]) == -1)
			ft_eprintf("minishell: export: `%s': failed to export\n",
				cmd->argv[i]);
		i++;
	}
	return (status);
}
