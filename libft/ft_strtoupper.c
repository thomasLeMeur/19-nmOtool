/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoupper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/23 14:52:05 by tle-meur          #+#    #+#             */
/*   Updated: 2015/11/23 14:57:40 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtoupper(const char *s)
{
	int		i;
	char	*tmp;

	i = -1;
	tmp = NULL;
	if (s)
	{
		if (!(tmp = ft_strnew(ft_strlen(s))))
			return (NULL);
		while (s[++i])
			tmp[i] = ft_toupper(s[i]);
	}
	return (tmp);
}
