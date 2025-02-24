/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:36:35 by czhu              #+#    #+#             */
/*   Updated: 2024/12/09 15:00:37 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_format(char spec, va_list *ap)
{
	int	count;

	count = 0;
	if (spec == 'c')
		count += print_char(va_arg(*ap, int));
	else if (spec == 's')
		count += print_string(va_arg(*ap, char *));
	else if (spec == 'd' || spec == 'i')
		count += print_digit(va_arg(*ap, int));
	else if (spec == 'u')
		count += print_unsigned(va_arg(*ap, unsigned int));
	else if (spec == 'p')
		count += print_ptr(va_arg(*ap, void *));
	else if (spec == 'x')
		count += print_hex(va_arg(*ap, unsigned int), 'x');
	else if (spec == 'X')
		count += print_hex(va_arg(*ap, unsigned int), 'X');
	else if (spec == '%')
		count += print_char('%');
	else
		count += write(1, &spec, 1);
	return (count);
}

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		count;

	count = 0;
	va_start(ap, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (*format)
				count += print_format(*format, &ap);
		}
		else
			count += write(1, format, 1);
		format++;
	}
	va_end(ap);
	return (count);
}
