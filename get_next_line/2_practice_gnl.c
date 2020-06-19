#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"

int		ft_strlen(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] != '\0')
		i++;
	return (i);
}

char	*ft_strchr(char *str, int c)
{
	int i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (str + i);
		i++;
	}
	return (NULL);
}

char	*ft_strjoin(char *s1, char *s2)
{
	int len_s1;
	int len_s2;
	int i;
	int j;
	char *buff;

	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	if(!(buff = (char *)malloc(sizeof(char) * (len_s1 + len_s2 + 1))))
		return (NULL);
	i = 0;
	j = 0;
	while (s1 && s1[i])
		buff[j++] = s1[i++];
	i = 0;
	while (s2 && s2[i])
		buff[j++] = s2[i++];
	buff[j] = '\0';
	return (buff);
}

char	*ft_substr(char *str, int start, int len)
{
	int i;
	char *buff; 

	i = 0;
	if(!str || !(buff = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	if (start < ft_strlen(str))
	{
		while (i < len && str[start + i] !='\0')
		{
			buff[i] = str[start + i];
			i++;
		}
	}
	buff[i] = '\0';
	return (buff);
}

int		get_next_line(char **line)
{
	char buff[2] = {0, };
	static char *stack = NULL;
	char *temp;
	int pos;
	int ret;

	ret = 1;
	if (!(ft_strchr(stack, 10)))
	{
		while (ret > 0 && !(ft_strchr(stack, 10)))
		{
			ret = read(0, buff, 1);
			temp = ft_strjoin(stack, buff);
			if (stack)
				free(stack);
			stack = temp;
		}
	}
	if (ret > 0 && ft_strchr(stack, 10))
	{
		pos = ft_strchr(stack, 10) - stack;
		*line = ft_substr(stack, 0, pos);
		temp = ft_substr(stack, pos + 1, ft_strlen(stack) - pos - 1);
		if (stack)
			free(stack);
		stack = temp;
	}
	else if (ret <= 0)
	{
		*line = ft_substr(stack, 0, ft_strlen(stack));
		if (stack)
			free(stack);
		stack = NULL;
	}
	return (ret);
}
