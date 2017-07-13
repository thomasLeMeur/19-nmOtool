/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utilities.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/24 21:20:13 by tle-meur          #+#    #+#             */
/*   Updated: 2017/02/27 12:02:23 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

uint32_t	rev32(t_env *e, uint32_t value)
{
	uint32_t	*n;
	uint32_t	val;

	val = 0;
	n = &value;
	val += (uint32_t)*((unsigned char *)n + 3) * (uint32_t)1;
	val += (uint32_t)*((unsigned char *)n + 2) * (uint32_t)256;
	val += (uint32_t)*((unsigned char *)n + 1) * (uint32_t)65536;
	val += (uint32_t)*((unsigned char *)n + 0) * (uint32_t)16777216;
	return ((e->rev == 1) ? val : value);
}

uint64_t	rev64(t_env *e, uint64_t value)
{
	uint64_t	*n;
	uint64_t	val;

	val = 0;
	n = &value;
	val += (uint64_t)*((unsigned char *)n + 7) * (uint64_t)1;
	val += (uint64_t)*((unsigned char *)n + 6) * (uint64_t)256;
	val += (uint64_t)*((unsigned char *)n + 5) * (uint64_t)65536;
	val += (uint64_t)*((unsigned char *)n + 4) * (uint64_t)16777216;
	val += (uint64_t)*((unsigned char *)n + 3) * (uint64_t)4294967296;
	val += (uint64_t)*((unsigned char *)n + 2) * (uint64_t)1099511627776;
	val += (uint64_t)*((unsigned char *)n + 1) * (uint64_t)281474976710656;
	val += (uint64_t)*((unsigned char *)n + 0) * (uint64_t)72057594037927936;
	return ((e->rev == 1) ? val : value);
}

int			fileout(t_env *e, void *addr)
{
	if (addr < e->beg || addr >= e->end)
	{
		ft_perror(e, "An offset under/overflowed the file");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void		ft_findarch(t_env *e, void *ptr)
{
	int			tmp;
	int			vars[6];

	e->ptr = ptr;
	tmp = e->rev;
	e->rev = 1;
	e->lim = rev32(e, ((struct fat_header *)ptr)->nfat_arch);
	e->rev = tmp;
	ft_find64arch(e, (struct fat_header *)ptr, vars);
	ft_protectmultiarchs(e, (struct fat_header *)ptr, vars);
	ft_callgoodarchs(e, (struct fat_header *)ptr, vars);
}

void		ft_core(t_env *e, void *ptr)
{
	if (fileout(e, ptr) == EXIT_FAILURE)
		return ;
	e->magic = *(int *)ptr;
	if ((e->magic == MH_MAGIC && !(e->rev = 0)) ||
			(e->magic == MH_CIGAM && (e->rev = 1)))
		ft_handle_32(e, ptr);
	else if ((e->magic == MH_MAGIC_64 && !(e->rev = 0)) ||
			(e->magic == MH_CIGAM_64 && (e->rev = 1)))
		ft_handle_64(e, ptr);
	else if (e->magic == FAT_MAGIC || e->magic == FAT_CIGAM)
		ft_findarch(e, ptr);
	else if (ft_strncmp((char *)ptr, ARMAG, SARMAG) == 0)
		ft_readarchive(e, ptr, ptr + SARMAG);
	else
		ft_perror(e, "Unknown format file");
}
