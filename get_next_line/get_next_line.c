#include <unistd.h>
#include <stdlib.h>
#include "get_next_line.h"

int		ft_strlen(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

char 	*ft_strchr(char *str, int c)
{
	int i;

	i = 0;
	if (!str)
		return(NULL);
	while (str[i])
	{
		if (str[i] == c)
			return (str + i);
		i++;
	}
	return (NULL);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	int		len1;
	int		len2;
	int		i;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	str = (char *)malloc(len1 + len2 + 1);
	i = 0;
	while (s1 && *s1)
	{
		str[i++] = *s1;
		s1++;
	}
	while (s2 && *s2)
	{
		str[i++] = *s2;
		s2++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_substr(char *str, int start, int len)
{
	char *buff;
	int i;

	if (!str)
		return (NULL);
	buff = (char *)malloc(len + 1);
	i = 0;
	if (start < ft_strlen(str))
	{
		while (i < len  && str[start + i])
		{
			buff[i] = str[start + i];
			i++;
		}
	}
	buff[i] = '\0';
	return (buff);
}

char	*ft_strdup(char *str)
{
	char *buff; 
	int	i;
	
	if (!str)
		return (NULL);
	buff = (char *)malloc(ft_strlen(str) + 1);
	i = 0;
	while (str[i])
	{
		buff[i] = str[i];
		i++;
	}
	buff[i] = '\0';
	return (buff);
}

int		get_next_line(char **line)
{
	int			pos;
	int			ret;
	char		buff[2];
	char		*tmp;
	static char *stack = 0;

	stack = stack == NULL ? ft_strdup("") : stack;
	ret = 1;
	while (ret > 0 && !(ft_strchr(stack, '\n')))
	{
		ret = read(0, buff, 1);
		buff[ret] = '\0';
		tmp = ft_strjoin(stack, buff);
		free(stack);
		stack = tmp;
	}
	if (ft_strchr(stack, '\n'))
	{
		pos = ft_strchr(stack, '\n') -stack;
		*line = ft_substr(stack, 0, pos);
		tmp = ft_substr(stack, pos + 1, ft_strlen(stack) - pos - 1);
		free(stack);
		stack = tmp;
	}
	if (ret == 0)
	{
		*line = ft_strdup(stack);
		free(stack);
		stack = NULL;
	}
	return (ret);
}

