#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

/*
** define
*/

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

void	ft_putchar(int c)
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

int		ft_atoi(char *str)
{
	int i;
	int result;

	i = 0;
	result = 0;
	while(str[i] != '\0' && ft_strchr(DIGIT, str[i]))
	{
		result = result * 10 + str[i] - 48;
		i++;
	}
	return (result);
}

int		ft_num_len(long long num, t_struct *info)
{
	int share;
	int len;

	share = (info->spec == 'd' ? 10: 16);
	len = (num <= 0 ? 1 : 0);
	if (num == 0 && info->dot &&!(info->prec))
		len = 0;
	num = (num < 0 ? -num : num);
	while (num >= 1)
	{
		num = num / share;
		len++;
	}
	return(len);
}

int		ft_strlen(char *str, t_struct *info)
{
	int i;

	i = 0;
	if (info->dot &&!(info->prec))
		return (0);
	while (str[i])
		i++;
	return (i);
}

void	ft_putnbr_base(long long num, t_struct *info)
{
	long long share;
	char *arr;
	long long num2;

	if (num == 0)
	{
		if (info->dot &&!(info->prec))
			return;
		ft_putchar('0');
		return;
	}
	share = (info->spec == 'd' ? 10 : 16);
	arr = (info->spec == 'd' ?  DIGIT : HEXA);
	num2 = num < 0 ? -num : num;
	if (num2 > 0)
	{
		if (num2 >= share)
			ft_putnbr_base(num2 / share, info);
		ft_putchar(arr[num2 % share]);
	}
}

int		ft_putstr(char *str, t_struct *info)
{
	int i;

	i = 0;
	while (str[i] != '\0' && ((i < info->prec && info->dot) || !(info->dot)))
		ft_putchar(str[i++]);
	return (i);
}

/*
** ft_printf
*/

void	init_struct(t_struct *info)
{
	info->width = 0;
	info->dot = 0;
	info->prec = 0;
	info->spec = 0;
	info->len = 0;
}

int		printing(va_list ap, t_struct *info)
{
	long long num;
	int i;
	int result;
	char *str;

	i = 0;
	result = 0;
	if(info->spec == 'd' || info->spec == 'x')
	{
		if (info->spec == 'd')
			num = va_arg(ap, int);
		else
			num = va_arg(ap, unsigned int);
		info->len = ft_num_len(num, info);
		i = (num < 0 ? 1 : 0);
		if(info->len < info->prec)
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
		if (info->len < info->prec)
		{
			i = 0;
			while (i++ <info->prec - info->len)
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
		if (info->prec && info->prec < info->len)
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
		result += ft_putstr(str, info);
	}
	return (result);
}

int		parse_format(va_list ap, char *fmt)
{
	int i;
	static t_struct *info;
	int result;
	
	i = 0;
	result = 0;
	info = (t_struct *)malloc(sizeof(t_struct));
	init_struct(info);
	while(fmt[i] != '\0')
	{
		while (fmt[i] != '\0' && fmt[i] != '%')
		{
			ft_putchar(fmt[i++]);
			result++;
		}
		while (fmt[i] != '\0' && !(ft_strchr(SPEC, fmt[i])))
		{
			if (ft_strchr(DIGIT, fmt[i]) && !(info->width) && !(info->prec) && fmt[i - 1] != '.')
				info->width = ft_atoi(fmt + i);
			if ((ft_strchr(DIGIT, fmt[i]) && !(info->prec) && fmt[i - 1] == '.'))
				info->prec = ft_atoi(fmt + i);
			if (fmt[i] == '.')
				info->dot = 1;
			i++;
		}
		if (ft_strchr(SPEC, fmt[i]))
		{
			info->spec = fmt[i++];
			result += printing(ap, info);
		}
	}
	free(info);
	return(result);
}

int		ft_printf(const char *fmt, ...)
{
	va_list ap;
	int 	result;

	va_start(ap, fmt);
	result = parse_format(ap, (char *)fmt);
	va_end(ap);
	return (result);
}
