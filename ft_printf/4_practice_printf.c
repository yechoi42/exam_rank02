#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#define DIGIT "0123456789"
#define HEXA "0123456789abcdef"
#define SPEC "dxs"

typedef struct
{
	int width;
	int dot;
	int prec;
	int len;
	int spec;
}t_struct;

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
			return(str + i);
		i++;
	}
	return (NULL);
}

int		ft_numlen(long long c, t_struct *info)
{
	int	share;
	int ret;

	share = info->spec == 'd' ? 10 : 16;
	if (c == 0 && info->dot == 1 && info->spec == 0)
		return (0);
	else if (c == 0)
		return (1);
	ret = c < 0 ? 1 : 0;
	c = c < 0 ? -c : c;
	while (c >= 1)
	{
		c /= share;
		ret++;
	}
	return (ret);
}

void	ft_putnbr_base(long long num, t_struct *info)
{
	int share;
	char *arr;

	share = info->spec == 'd' ?  10 : 16;
	arr = info->spec == 'd' ? DIGIT : HEXA;
	if (num == 0)
	{
		if (info->dot && !(info->prec))
			return;
		ft_putchar('0');
		return;
	}
	num = num < 0 ? -num : num;
	if (num >= share)
		ft_putnbr_base(num / share, info);
	ft_putchar(arr[num % share]);
	return;
}

int		ft_strlen(char *str, t_struct *info)
{
	int i;

	if (!str || (info->dot && !(info->prec)))
		return (0);
	i = 0;
	while (str[i] != '\0' && ((info->prec && i < info->prec) || !(info->prec)))
		i++;
	return (i);
}

void	ft_putstr(char *str, t_struct *info)
{
	int i;

	if (!str || (info->dot && !(info->prec)))
		return ;
	i = 0;
	while (str[i] && ((info->prec && i < info->prec) || !(info->prec)))
		ft_putchar(str[i++]);
}

void	init_struct(t_struct *info)
{
	info->width = 0;
	info->dot = 0;
	info->prec = 0;
	info->len = 0;
	info->spec = 0;
}

int		printing(va_list ap, t_struct *info)
{
	int i;
	int ret;
	long long num;
	char *str;

	ret = 0;
	if (info->spec == 'd' || info->spec == 'x')
	{
		if (info->spec == 'd')
			num = va_arg(ap, int);
		else
			num = va_arg(ap, unsigned int);
		info->len = ft_numlen(num, info);
		i = num < 0 ? 1 : 0;
		if (info->len < info->prec)
			while (i++ <  info->width - info->prec)
			{
				ft_putchar(' ');
				ret++;
			}
		else
			while (i++ < info->width - info->len)
			{
				ft_putchar(' ');
				ret++;
			}
		i = 0;
		if (info->len < info->prec)
		{
			while (i++ < info->prec - info->len)
			{
				ft_putchar('0');
				ret++;
			}
		}
		num < 0 ? ft_putchar('-') : 0;
		ft_putnbr_base(num, info);
		ret += info->len;
	}
	else
	{
		str = va_arg(ap, char *);
		str = str == NULL ? "(null)" : str;
		info->len = ft_strlen(str, info);
		i = 0;
		if (info->prec && info->prec < info->len)
			while (i++ < info->width - info->prec)
			{
				ft_putchar(' ');
				ret++;
			}
		else 
			while (i++ < info->width - info->len)
			{
				ft_putchar(' ');
				ret++;
			}
		ft_putstr(str, info);
		ret += info->len;
	}
	return (ret);
}

int		parse_format(va_list ap, const char *fmt)
{
	int ret;
	t_struct *info;

	ret = 0;
	while (*fmt)
	{
		while (*fmt && *fmt != '%')
		{
			ft_putchar(*fmt);
			ret++;
			fmt++;
		}
		if (*fmt == '%')
		{
			info = (t_struct *)malloc(sizeof(t_struct));
			init_struct(info);
			fmt++;
			if (ft_strchr(DIGIT, *fmt))
			{
				while (ft_strchr(DIGIT, *fmt))
				{
					info->width = info->width * 10 + *fmt - 48;
					fmt++;
				}
			}
			if (*fmt == '.')
			{
				info->dot = 1;
				fmt++;
			}
			if (ft_strchr(DIGIT, *fmt))
			{
				while (ft_strchr(DIGIT, *fmt))
				{
					info->prec = info->prec * 10 + *fmt - 48;
					fmt++;
				}
			}
			if (ft_strchr(SPEC, *fmt))
			{
				info->spec = *fmt;
				fmt++;
			}
			ret += printing(ap, info);
			free(info);
		}
	}
	return (ret);
}

int		ft_printf(const char *fmt, ...)
{
	int ret;
	va_list ap;

	va_start(ap, fmt);
	ret = parse_format(ap, fmt);
	va_end(ap);
	return (ret);
}
