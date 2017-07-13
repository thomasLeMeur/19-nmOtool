/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_archive.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/24 21:20:27 by tle-meur          #+#    #+#             */
/*   Updated: 2017/02/27 11:47:50 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

int				ft_getsize(char *s)
{
	if (ft_strncmp(s, AR_EFMT1, ft_strlen(AR_EFMT1)) == 0)
		return (ft_atoi(s + ft_strlen(AR_EFMT1)));
	else
		return (0);
}

static char		*ft_getname(char *s, int size)
{
	int		i;
	int		len;
	char	*name;

	len = size;
	if ((name = (char *)malloc((FT_MAX(16, size) + 1) * sizeof(char))) == NULL)
		return (NULL);
	if (len == 0)
		while (len < 16 && s[len] != '\0' && s[len] != ' ')
			len++;
	i = -1;
	while (++i < len && s[i] != '\0')
		name[i] = s[i];
	name[i] = '\0';
	return (name);
}

static t_obj	*ft_addobj(t_obj *lst, struct ar_hdr *h, int off)
{
	t_obj	*n;

	if (((n = (t_obj *)malloc(sizeof(t_obj))) == NULL) ||
			((n->next = NULL) && 0) ||
			((n->off = off) && 0) ||
			((n->s = ft_getsize(h->ar_name)) && 0) ||
			((n->addr = (uint64_t)((void *)(h + 1) + n->s)) && 0) ||
			((n->name = ft_getname((char *)(h + (n->s != 0)), n->s)) == NULL))
		return (NULL);
	if (lst == NULL)
		return (n);
	while (lst->next != NULL)
	{
		if (lst->addr == n->addr)
			break ;
		lst = lst->next;
	}
	if (lst->addr != n->addr)
		return (NULL + 1 + 0 * (int)(lst->next = n));
	free(n->name);
	free(n);
	return (NULL + 1);
}

void			ft_readarchive(t_env *e, void *ptr, struct ar_hdr *h)
{
	int				i;
	int				n;
	struct s_obj	*r;
	struct ranlib	*l;
	struct s_obj	*lst;

	n = *(int *)((void *)h + sizeof(*h) + ft_getsize(h->ar_name)) / sizeof(*l);
	l = (void *)h + sizeof(*h) + ft_getsize(h->ar_name) + sizeof(int);
	if ((r = (struct s_obj *)ft_strchr(e->filename, '(')) != NULL)
		e->filename[(char *)r - e->filename - 1] = '\0';
	lst = NULL;
	i = 0;
	while (i++ < n)
		if (fileout(e, l) == EXIT_FAILURE)
			return ;
		else if (fileout(e, ptr + l->ran_off) == EXIT_FAILURE)
			return ;
		else if ((r = ft_addobj(lst, ptr + l->ran_off, l->ran_off)) == NULL)
		{
			ft_perror(e, "Malloc error");
			return ;
		}
		else if (l++ && r != NULL + 1)
			lst = r;
	(e->prog == FT_NM) ? ft_nm_printobjs(e, lst) : ft_otool_printobjs(e, lst);
}

void			ft_find64arch(t_env *e, struct fat_header *head, int *vars)
{
	uint32_t		i;
	struct fat_arch	*arch;

	i = 0;
	e->rev = 1;
	ft_memset(vars, 0, 6 * sizeof(int));
	arch = (struct fat_arch *)((void *)head + sizeof(*head));
	while (i++ < e->lim)
	{
		e->rev = 1;
		if (fileout(e, arch) == EXIT_FAILURE)
			return ;
		e->m = *(uint32_t *)((void *)head + rev32(e, arch->offset));
		if (rev32(e, arch->cputype & CPU_ARCH_MASK) == CPU_TYPE_POWERPC &&
				(e->m == MH_MAGIC_64 || e->m == MH_CIGAM_64))
			vars[0] = 1;
		else if (rev32(e, arch->cputype & CPU_ARCH_MASK) == CPU_TYPE_ARM &&
				(e->m == MH_MAGIC_64 || e->m == MH_CIGAM_64))
			vars[1] = 1;
		else if (rev32(e, arch->cputype & CPU_ARCH_MASK) == CPU_TYPE_X86 &&
				(e->m == MH_MAGIC_64 || e->m == MH_CIGAM_64))
			vars[2] = 1;
		arch++;
	}
}
