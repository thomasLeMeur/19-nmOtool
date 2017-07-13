/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/23 14:52:14 by tle-meur          #+#    #+#             */
/*   Updated: 2015/11/23 14:57:43 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t len)
{
	size_t	size;
	char	*tmp;

	if (!s || !len)
		return (NULL);
	size = ft_strlen(s);
	size = FT_MIN(size, len);
	if (!(tmp = ft_strnew(size)))
		return (NULL);
	ft_memcpy(tmp, s, size * sizeof(char));
	return (tmp);
}
