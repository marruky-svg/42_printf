/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elsouza <elsouza@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 14:55:00 by elsouza           #+#    #+#             */
/*   Updated: 2026/11/07 13:02:06 by elsouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_char(char c)
{
	return (write(1, &c, 1));
}

int	print_str(const char *str)
{
	int	i;

	if (!str)
		return (print_str("(null)"));
	i = 0;
	while (str[i])
		if (print_char(str[i++]) == -1)
			return (-1);
	return (i);
}

int	print_num(unsigned long n, const char *set, unsigned long len)
{
	int				count;
	int				i;

	count = 0;
	if (n >= len)
	{
		count = print_num(n / len, set, len);
		if (count == -1)
			return (-1);
	}
	i = print_char(set[n % len]);
	if (i == -1)
		return (-1);
	return (count + i);
}

int	print_signed(int n)
{
	long int	nb;
	int			count;
	int			i;

	nb = n;
	count = 0;
	if (n < 0)
	{
		count = print_char('-');
		if (count == -1)
			return (-1);
		nb = -nb;
	}
	i = print_num(nb, "0123456789", 10);
	if (i == -1)
		return (-1);
	return (count + i);
}

int	print_ptr(void *ptr)
{
	int	count;
	int	i;

	if (!ptr)
		return (print_str(PTR_NULL));
	count = print_str("0x");
	if (count == -1)
		return (-1);
	i = print_num((unsigned long) ptr, "0123456789abcdef", 16);
	if (i == -1)
		return (-1);
	return (count + i);
}
