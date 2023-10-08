/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgutak <dgutak@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:59:42 by dgutak            #+#    #+#             */
/*   Updated: 2023/10/08 18:59:35 by dgutak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

size_t	atoi_new(char *str)
{
	size_t	x;
	int		sign;

	x = 0;
	sign = 1;
	while (*str == ' ' || *str == '\v' || *str == '\t' || *str == '\n'
		|| *str == '\f' || *str == '\r')
		str++;
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
			x = x * 10 + *str - '0';
		else
			break ;
		if ((x > 2147483647 && sign == 1) || (sign == -1))
			return (9876543210);
		str++;
	}
	return (sign * x);
}
