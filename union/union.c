#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
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

void	ft_union(char *s1, char *s2)
{
	int i;
	int j;
	char buff[1024] = {0, };

	i = 0;
	j = 0;
	while (s1[i] != '\0')
	{
		if (!(ft_strchr(buff, s1[i])))
			buff[j++] = s1[i];
		i++;
	}
	i = 0;
	while (s2[i] != '\0')
	{
		if (!(ft_strchr(buff, s2[i])))
			buff[j++] = s2[i];
		i++;
	}
	write(1, buff, j);
}

int		main(int argc, char **argv)
{
	if (argc == 3)
		ft_union(argv[1], argv[2]);
	ft_putchar('\n');
	return (0);
}
