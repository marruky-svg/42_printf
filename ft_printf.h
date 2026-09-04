/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elsouza <elsouza@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:40:40 by elsouza           #+#    #+#             */
/*   Updated: 2026/09/03 22:53:41 by elsouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <unistd.h>
# include <stdarg.h>

typedef struct s_spec
{
	int	minus;
	int	zero;
	int	hash;
	int	space;
	int	plus;
	int	width;
	int	precision;
}	t_spec;

int	print_char(char c);
int	print_str(const char *str);
int	print_num(unsigned long n, const char *set, unsigned long len);
int	print_ptr(void *ptr);
int	print_signed(int n);
int	ft_printf(const char *str, ...);

#endif