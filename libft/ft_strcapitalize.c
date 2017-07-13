/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcapitalize.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/23 14:52:05 by tle-meur          #+#    #+#             */
/*   Updated: 2015/11/23 14:57:40 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcapitalize(const char *s)
{
	int		i;
	char	*tmp;
	int		begin;

	tmp = NULL;
	if (s && (i = -1))
	{
		begin = 1;
		if (!(tmp = ft_strnew(ft_strlen(s))))
			return (NULL);
		while (s[++i])
			if (begin)
			{
				tmp[i] = ft_toupper(s[i]);
				if (ft_isalnum(s[i]))
					begin = 0;
			}
			else
			{
				tmp[i] = ft_tolower(s[i]);
				if (s[i] == '.' || s[i] == '\n')
					begin = 1;
			}
	}
	return (tmp);
}
