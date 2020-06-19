#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

#define	DIGIT	"0123456789"
#define	HEXA	"0123456789abcdef"
#define SPEC	"dxs"

int		ret;
int		len;
int		width;
int		dot;
int		prec;
int		spec;


void	ft_putchar(char c)
{
	write(1, &c,1);
	ret++;
}

char	*ft_strchr(char *str, int c)
{
	int i;

	if (!str)
		return (NULL);
	i = -1;
	while (str[++i])
		if (str[i] == c)
			return (str + i);
	return (NULL);
}

void	init_info(void)
{
	len = 0;
	width = 0;
	dot = 0;
	prec = 0;
	spec = 0;
}

int		ft_strlen(char *str)
{
	int i;

	i = 0;
	if (!str || (dot == 1 && prec == 0))
		return (0);
	else if (prec)
	{
		while (i < prec && str[i] != '\0')
			i++;
		return (i);
	}
	else
	{
		while (str[i])
			i++;
		return (i);
	}
}

void	ft_putstr(char *str)
{
	int i;

	i = 0;
	if (dot == 1 && prec == 0)
		return;
	else
		while (str[i] && ((prec && i < prec) || !(prec)))
			ft_putchar(str[i++]);
}

int		ft_numlen(long long num)
{
	int	i;
	long long share;

	i = 1;
	share = spec == 'd' ? 10 : 16;
	if (num == 0 && dot == 1 && prec == 0)
		return (0);
	while (num >= share)
	{
		num /= share;
		i++;
	}
	return (i);
}

void	ft_putnbr(long long num)
{
	long long share;
	char *arr;

	share = spec == 'd' ? 10 : 16;
	arr = spec == 'd' ? DIGIT : HEXA;
	if (num == 0 && dot == 1 && prec == 0)
		return;
	if (num >= share)
		ft_putnbr(num / share);
	ft_putchar(arr[num % share]);
}

void	printing(va_list ap)
{
	char		*str;
	long long	num;
	int			i;

	if (spec == 's')
	{
		str = va_arg(ap, char *);
		str = str == NULL ? "(null)" : str;
		len = ft_strlen(str);
		i = 0;
		if (prec && prec < len)
		{
			while (i++ < width - prec)
				ft_putchar(' ');
		}
		else
		{
			while (i++ < width - len)
				ft_putchar(' ');
		}
		ft_putstr(str);
	}
	else if (spec == 'd' || spec == 'x')
	{
		if (spec == 'd')
			num = va_arg(ap, int);
		else
			num = va_arg(ap, unsigned int);
		len = num < 0 ? ft_numlen(-num) : ft_numlen(num);
		i = num < 0 ? 1 : 0;
		if (len < prec)
		{
			while (i++ < width - prec)
				ft_putchar(' ');
		}
		else
		{
			while (i++ < width - len)
				ft_putchar(' ');
		}
		i = 0;
		if (len < prec)
		{
			while (i++ < prec - len)
				ft_putchar('0');
		}
		num < 0 ? ft_putchar('-') : 0;
		num < 0 ? ft_putnbr(-num) : ft_putnbr(num);
	}
}

int		ft_printf(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	ret = 0;
	while (*fmt)
	{
		init_info();
		if (*fmt != '%')
		{
			ft_putchar(*fmt);
			fmt++;
		}
		else
		{
			++fmt;
			while (ft_strchr(DIGIT, *fmt))
			{
				width = width * 10 + *fmt - 48;
				fmt++;
			}
			if (*fmt == '.')
			{
				dot = 1;
				fmt++;
			}
			while (ft_strchr(DIGIT, *fmt))
			{
				prec = prec * 10 + *fmt - 48;
				fmt++;
			}
			if (ft_strchr(SPEC, *fmt))
			{
				spec = *fmt;
				fmt++;
			}
			printing(ap);
		}
	}
	va_end(ap);
	return ret;
}
