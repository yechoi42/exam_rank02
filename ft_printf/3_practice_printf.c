#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>

#define DIGIT "0123456789"
#define HEXA "0123456789abcdef"
#define SPEC "dxs"

typedef struct
{
	int width;
	int dot;
	int prec;
	int spec;
	int len;
}t_struct;

/*
** utils
*/

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
		if(str[i] == c)
			return (str + i);
		i++;
	}
	return (NULL);
}

int		ft_atoi(char *str)
{
	int i;
	int result;

	i = 0;
	result = 0;
	if (!str)
		return (0);
	while (str[i] != '\0' && ft_strchr(DIGIT, str[i]))
	{
		result = result * 10 + str[i] - 48;
		i++;
	}
	return (result);
}

int		ft_numlen(long long num, t_struct *info)
{
	int result;
	int	share;

	result = (num <= 0 ? 1 : 0);
	if (num == 0 && info->dot && !(info->prec)) // num == 0 
		return (0);
	share = (info->spec == 'd' ? 10 : 16);
	num = (num < 0 ? -num : num);
	while (num >=  1) // num >= 1
	{
		num = num / share;
		result++;
	}
	return (result);
}

void	ft_putnbr_base(long long num, t_struct *info)
{
	int share;
	char *arr; 

	if (num == 0)
	{
		if (info->dot && !(info->prec))
			return;
		ft_putchar('0');
		return;
	}
	num = (num < 0 ? -num : num); //make positive number
	share = (info->spec == 'd' ? 10 :16);
	arr = (info->spec == 'd' ? DIGIT : HEXA);
	if (num >= share) 
		ft_putnbr_base(num / share, info);
	ft_putchar(arr[num % share]);
	return ;
}

int		ft_strlen(char *str, t_struct *info)
{
	int i;

	i = 0;
	if (info->dot && !(info->prec)) //exception
		return (0);
	while (str[i] != '\0')
		i++;
	return (i);
}

int		ft_putstr(char *str, t_struct *info)
{
	int i;
	
	i = 0;
	if (info->dot && !(info->prec))
		return (0);
	while (str[i] != '\0' && ((info->prec && i < info->prec) || !(info->prec))) //prec condition
		ft_putchar(str[i++]);
	return (i);
}

/*
** printf
*/

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
	int result;
	long long num;
	char *str;

	result = 0;
	if(info->spec == 'd' || info->spec == 'x')
	{
		if (info->spec == 'd')
			num = va_arg(ap, int);
		else
			num = va_arg(ap, unsigned int);
		info->len = ft_numlen(num, info);
		i = (num < 0 ? 1 : 0);
		if (info->len < info->prec)
		{
			while (i++ < info->width - info->prec)
			{
				ft_putchar(' ');
				result++;
			}
		}
		else
		{
			while (i++ < info->width - info->len)
			{
				ft_putchar(' ');
				result++;
			}
		}
		i = 0;
		if (info->len < info->prec)
		{
			while(i++ < info->prec - info->len)
			{
				ft_putchar('0');
				result++;
			}
		}
		num < 0 ? ft_putchar('-') : 0;
		ft_putnbr_base(num, info);
		result += info->len;
	}
	else
	{
		str = va_arg(ap, char *);
		if (!str)
			str = "(null)";
		info->len = ft_strlen(str, info);
		i = 0;
		if (info->prec && info->prec < info->len) // info->prec == 1
		{
			while (i++ < info->width - info->prec)
			{
				ft_putchar(' ');
				result++;
			}
		}
		else
			while (i++ < info->width - info->len)
			{
				ft_putchar(' ');
				result++;
			}
		result += ft_putstr(str, info);
	}
	return(result);
}
			

int		parse_format(va_list ap, char *fmt)
{
	int i;
	int result;
	t_struct *info;

	i = 0;
	result = 0;
	while (fmt[i] != '\0')
	{
		while (fmt[i] != '\0' && fmt[i] != '%')
		{
			ft_putchar(fmt[i++]);
			result++;
		}
		if (fmt[i] == '%')
		{
			info = (t_struct *)malloc(sizeof(t_struct));
			init_struct(info);
			while (fmt[i] != '\0' && !(ft_strchr(SPEC, fmt[i])))
			{
				if (!(info->width) && !(info->prec) && fmt[i - 1] != '.' && ft_strchr(DIGIT, fmt[i]))
					info->width = ft_atoi(fmt + i);
				if (fmt[i] == '.')
					info->dot = 1;
				if (!(info->prec) && ft_strchr(DIGIT, fmt[i]) && fmt[i - 1] == '.')
					info->prec = ft_atoi(fmt + i);
				i++;
			}
			if (fmt[i] != '\0' && ft_strchr(SPEC, fmt[i]))
			{
				info->spec = fmt[i++]; // ++ here
				result += printing(ap, info);
			}
			free(info);
		}
	}
	return (result);
}

int		ft_printf(const char *fmt, ...)
{
	va_list ap;
	int result;

	va_start(ap, fmt);
	result = parse_format(ap, (char *)fmt);
	va_end(ap);
	return (result);
}
