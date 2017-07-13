/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_btreefind_bylevel_up.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/23 14:46:06 by tle-meur          #+#    #+#             */
/*   Updated: 2015/11/23 14:54:21 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_btree	*ft_btreefind_bylevel_up(t_btree *root, const void *content_ref,
		size_t size_ref,
		int (*cmp)(const t_btree *, const void *, size_t))
{
	t_btree	*tmp;
	t_list	*lst;

	if (!root || !content_ref || !size_ref || !cmp)
		return (NULL);
	lst = ft_lstfrom_btree_bylevel_up(root);
	while (lst)
	{
		tmp = (t_btree*)lst->content;
		if ((*cmp)(tmp, content_ref, size_ref))
		{
			while (lst)
				ft_lstdequeue(&lst);
			return (tmp);
		}
		ft_lstdequeue(&lst);
	}
	return (NULL);
}
