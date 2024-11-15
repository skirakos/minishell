#include "minishell.h"

void	check_num_is_valid(const char *str, unsigned long long nbr, int sign)
{
	if (nbr > (unsigned long long)LLONG_MAX + (unsigned long long)sign)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		g_exit_status = 255;
		exit (g_exit_status);
	}
}

long	result(char *str, size_t i, char sign)
{
	unsigned long long	n;

	n = 0;
	if (str[i] >= 48 && str[i] <= 57)
	{
		while (str[i] >= 48 && str[i] <= 57 && str[i] != '\0')
		{
			n = n * 10 + str[i] - '0';
			if (n > LLONG_MAX)
				check_num_is_valid(str, n, sign);
			i++;
		}
		if (sign == '-')
			n = -1 * n;
		return (n);
	}
	return (-1);
}

int	ft_atoi(char *str)
{
	int		i;
	char	sign;

	sign = 0;
	i = 0;
	while (str[i] != '\0' && (str[i] == 32 || str[i] == '\t' || str[i] == '\n'
			|| str[i] == '\r' || str[i] == '\v' || str[i] == '\f'))
		i++;
	if (str[i] == '+')
	{
		sign = '+';
		i++;
	}
	else if (str[i] == '-')
	{
		sign = '-';
		i++;
	}
	return (result((char *)str, i, sign));
}
