/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elsouza <elsouza@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:40:40 by elsouza           #+#    #+#             */
/*   Updated: 2026/11/07 13:03:43 by elsouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# ifdef __linux__
#  define PTR_NULL "(nil)"
# else
#  define PTR_NULL "0x0"
# endif
# include <unistd.h>
# include <stdarg.h>

int	print_char(char c);
int	print_str(const char *str);
int	print_num(unsigned long n, const char *set, unsigned long len);
int	print_ptr(void *ptr);
int	print_signed(int n);
int	ft_printf(const char *str, ...);

#endif