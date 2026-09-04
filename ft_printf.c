/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elsouza <elsouza@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 12:28:21 by elsouza           #+#    #+#             */
/*   Updated: 2026/09/02 09:10:38 by elsouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	convertion_handler(char c, va_list *ap)
{
	if (c == '%')
		return (print_char('%'));
	else if (c == 'c')
		return (print_char(va_arg(*ap, int)));
	else if (c == 's')
		return (print_str(va_arg(*ap, char *)));
	else if (c == 'u')
		return (print_num(va_arg(*ap, unsigned int), "0123456789", 10));
	else if (c == 'x')
		return (print_num(va_arg(*ap, unsigned int), "0123456789abcdef", 16));
	else if (c == 'X')
		return (print_num(va_arg(*ap, unsigned int), "0123456789ABCDEF", 16));
	else if (c == 'p')
		return (print_ptr(va_arg(*ap, void *)));
	else if (c == 'i' || c == 'd')
		return (print_signed(va_arg(*ap, int)));
	return (0);
}

static int	process_logic(const char *str, int *i, va_list *ap)
{
	int	val;

	if (str[*i] == '%' && str[*i + 1])
	{
		val = convertion_handler(str[*i + 1], ap);
		*i += 2;
	}
	else
	{
		val = print_char(str[*i]);
		(*i)++;
	}
	return (val);
}

int	ft_printf(const char *str, ...)
{
	int		i;
	int		count;
	int		val;
	va_list	ap;

	i = 0;
	count = 0;
	va_start(ap, str);
	while (str[i])
	{
		val = process_logic(str, &i, &ap);
		if (val == -1)
		{
			va_end(ap);
			return (-1);
		}
		count += val;
	}
	va_end(ap);
	return (count);
}
