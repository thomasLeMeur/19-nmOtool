/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_architecture.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/24 23:13:12 by tle-meur          #+#    #+#             */
/*   Updated: 2017/02/24 23:14:59 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static int	ft_checkforprotect(t_env *e, struct fat_arch *arch, int *vars)
{
	if (rev32(e, arch->cputype & CPU_ARCH_MASK) == CPU_TYPE_POWERPC
			&& (vars[0] == 0 || e->m == MH_MAGIC_64 || e->m == MH_CIGAM_64))
	{
		if ((vars[3] = vars[3] + 2) <= 2)
			return (1);
	}
	else if (rev32(e, arch->cputype & CPU_ARCH_MASK) == CPU_TYPE_X86
			&& (vars[2] == 0 || e->m == MH_MAGIC_64 || e->m == MH_CIGAM_64))
	{
		if ((vars[5] = vars[5] + 2) <= 2)
			return (1);
	}
	else if (rev32(e, arch->cputype & CPU_ARCH_MASK) == CPU_TYPE_ARM
			&& (vars[1] == 0 || e->m == MH_MAGIC_64 || e->m == MH_CIGAM_64))
	{
		if ((vars[4] = vars[4] + 2) <= 2)
			return (1);
	}
	else if ((int)rev32(e, arch->cputype) == e->cputype)
		return (1);
	return (0);
}

void		ft_protectmultiarchs(t_env *e, struct fat_header *head, int *vars)
{
	uint32_t		i;
	struct fat_arch	*arch;

	i = 0;
	e->cnt = 0;
	arch = (struct fat_arch *)((void *)head + sizeof(*head));
	while (i++ < e->lim)
	{
		e->rev = 1;
		if (fileout(e, arch) == EXIT_FAILURE)
			return ;
		e->m = *(uint32_t *)((void *)head + rev32(e, arch->offset));
		e->cnt += ft_checkforprotect(e, arch++, vars);
	}
}

static int	ft_calltherealarch_2(t_env *e, struct fat_arch *arch, int *vars)
{
	if (rev32(e, arch->cputype & CPU_ARCH_MASK) == CPU_TYPE_X86
			&& (vars[2] == 0 || e->m == MH_MAGIC_64 || e->m == MH_CIGAM_64))
	{
		if ((vars[2] = vars[2] + 2) <= 3)
		{
			if (e->cputype != (int)rev32(e, arch->cputype))
				ft_strcat(e->filename, (e->prog == FT_NM) ?
						" (for architecture i386)" : " (architecture i386)");
			ft_core(e, e->ptr + rev32(e, arch->offset));
		}
	}
	else if (rev32(e, arch->cputype & CPU_ARCH_MASK) == CPU_TYPE_ARM
			&& (vars[1] == 0 || e->m == MH_MAGIC_64 || e->m == MH_CIGAM_64))
	{
		if ((vars[1] = vars[1] + 2) <= 3)
		{
			ft_strcat(e->filename, (e->prog == FT_NM) ?
					" (for architecture arm)" : " (architecture arm)");
			ft_core(e, e->ptr + rev32(e, arch->offset));
		}
	}
	return (EXIT_FAILURE);
}

static int	ft_calltherealarch(t_env *e, struct fat_arch *arch, int *vars)
{
	if (ft_strncmp(e->ptr + rev32(e, arch->offset), ARMAG, SARMAG) == 0)
	{
		if (e->cputype == (int)rev32(e, arch->cputype))
		{
			ft_core(e, e->ptr + rev32(e, arch->offset));
			return (EXIT_SUCCESS);
		}
	}
	else if (rev32(e, arch->cputype & CPU_ARCH_MASK) == CPU_TYPE_POWERPC
			&& (vars[0] == 0 || e->m == MH_MAGIC_64 || e->m == MH_CIGAM_64))
	{
		if ((vars[0] = vars[0] + 2) <= 3)
		{
			e->ppc = 1;
			ft_strcat(e->filename, (e->prog == FT_NM) ?
					" (for architecture ppc)" : " (architecture ppc)");
			ft_core(e, e->ptr + rev32(e, arch->offset));
		}
	}
	else if (ft_calltherealarch_2(e, arch, vars))
		;
	return (EXIT_FAILURE);
}

void		ft_callgoodarchs(t_env *e, struct fat_header *head, int *vars)
{
	uint32_t		i;
	int				type;
	struct fat_arch	*arch;
	char			file[ft_strlen(e->filename) + 1];

	i = 0;
	arch = (struct fat_arch *)((void *)head + sizeof(*head));
	while (i++ < e->lim)
	{
		e->rev = 1;
		e->ppc = 0;
		type = e->prog;
		e->print = (e->cnt > 1) ? 1 : 0;
		ft_strcpy(file, e->filename);
		if (fileout(e, arch) == EXIT_FAILURE)
			return ;
		e->m = *(uint32_t *)(e->ptr + rev32(e, arch->offset));
		if (ft_calltherealarch(e, arch, vars) == EXIT_SUCCESS)
			break ;
		clearenv(e);
		initenv(e, file, type);
		arch++;
	}
}
