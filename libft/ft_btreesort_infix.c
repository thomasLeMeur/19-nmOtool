/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_btreesort_infix.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/23 14:47:32 by tle-meur          #+#    #+#             */
/*   Updated: 2015/11/23 14:54:55 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_btreesort_infix(t_btree *root,
		int (*cmp)(const t_btree *, const t_btree *))
{
	t_btree	*t;

	if (!root || !cmp)
		return ;
	if ((t = root->left))
	{
		ft_btreesort_infix(t, cmp);
		if ((*cmp)(t, root))
			FT_SWAP(t->content, root->content, void *);
	}
	if ((t = root->right))
	{
		if ((*cmp)(root, t))
			FT_SWAP(root->content, t->content, void *);
		ft_btreesort_infix(t, cmp);
	}
}
