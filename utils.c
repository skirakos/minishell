#include "minishell.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	while (str1[i] && i < n && (str1[i] != '\0' || str2[i] != '\0'))
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	printf("gtcvtucvtu\n");
	return (0);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*s;
	int		i;
	int		j;

	s = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!s)
		return (NULL);
	printf("oooo\n");
	i = 0;
	j = 0;
	while (s1 && s1[i])
	{
		s[i] = s1[i];
		i++;
	}
	while (s2 && s2[j])
	{
		s[i] = s2[j];
		i++;
		j++;
	}
	s[i] = '\0';
	printf("heriqaaa\n");
	return (s);
}

void	ft_strcpy(char *dst, char *src)
{
	int	i;

	i = 0;
	while (src[i] && i < (ft_strlen(src) + 1))
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

char	*ft_strdup(char *s1)
{
	int		i;
	int		len;
	char	*str_copy;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	str_copy = (char *)malloc((len + 1) * sizeof(char));
	i = 0;
	if (str_copy)
	{
		while (s1[i] != '\0')
		{
			str_copy[i] = s1[i];
			i++;
		}
		str_copy[i] = '\0';
		return (str_copy);
	}
	return (NULL);
}