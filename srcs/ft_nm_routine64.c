/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm_routine64.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/24 21:15:11 by tle-meur          #+#    #+#             */
/*   Updated: 2017/02/27 11:48:19 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static int	loopsections_64(t_env *e, struct segment_command_64 *seg, int lim)
{
	uint32_t			i;
	struct section_64	*s;

	i = 0;
	s = (struct section_64 *)((void *)seg + sizeof(*seg));
	while (i++ < rev32(e, seg->nsects))
	{
		if (fileout(e, s) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (++e->sects_cnt == lim)
		{
			if (ft_strcmp(s->segname, SEG_TEXT) == 0)
				e->letter += !ft_strcmp(s->sectname, SECT_TEXT) ? 'T' : 'S';
			else if (ft_strcmp(s->segname, SEG_DATA) == 0)
				if (ft_strcmp(s->sectname, SECT_DATA) == 0)
					e->letter += 'D';
				else
					e->letter += !ft_strcmp(s->sectname, SECT_BSS) ? 'B' : 'S';
			else
				e->letter += 'S';
			return (EXIT_SUCCESS);
		}
		s++;
	}
	return (EXIT_FAILURE);
}

static int	loopsegments_64(t_env *e, void *ptr, struct nlist_64 *symb)
{
	uint32_t					i;
	struct load_command			*lc;
	struct segment_command_64	*seg;
	struct mach_header_64		*head;

	head = (struct mach_header_64 *)ptr;
	lc = ptr + sizeof(*head);
	e->sects_cnt = 0;
	i = 0;
	while (i++ < rev32(e, head->ncmds))
	{
		if (fileout(e, lc) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (rev32(e, lc->cmd) == LC_SEGMENT_64)
		{
			seg = (struct segment_command_64 *)lc;
			if (loopsections_64(e, seg, symb->n_sect) == EXIT_SUCCESS)
				return (EXIT_SUCCESS);
		}
		lc = (void *)lc + rev32(e, lc->cmdsize);
	}
	e->letter += (e->value != 0) ? 'C' : 'U';
	return (EXIT_SUCCESS);
}

static int	findletter_64(t_env *e, void *ptr, struct nlist_64 *s)
{
	int	t;

	t = s->n_type;
	e->letter = ((t & N_EXT) == 0) ? 32 : 0;
	if ((t & N_TYPE) == N_UNDF && ((t & N_EXT) == 0) && e->value != 0)
		e->letter += 'C';
	else if ((t & N_TYPE) == N_ABS)
		e->letter += 'A';
	else if ((t & N_TYPE) == N_INDR)
		e->letter += 'I';
	else if (loopsegments_64(e, ptr, s) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	else if (e->letter == 'S' && (t & N_TYPE) == N_UNDF)
		e->letter = e->letter - 'S' + 'U';
	if (ft_addline(e) == NULL)
	{
		ft_perror(e, "Malloc error");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static void	print_64(t_env *e, void *ptr, struct symtab_command *sym)
{
	uint32_t		i;
	struct nlist_64	*symbs;

	i = UINT32_MAX;
	symbs = ptr + rev32(e, sym->symoff);
	if (fileout(e, symbs) == EXIT_FAILURE)
		return ;
	if (e->print == 1)
		ft_printf("\n%s:\n", e->filename);
	while (++i < rev32(e, sym->nsyms))
	{
		e->value = rev64(e, symbs[i].n_value);
		e->sym = ptr + rev32(e, sym->stroff) + rev32(e, symbs[i].n_un.n_strx);
		if (fileout(e, e->sym) == EXIT_FAILURE)
			return ;
		if (*e->sym != '\0' && (symbs[i].n_type & N_STAB) == 0)
			if (findletter_64(e, ptr, symbs + i) == EXIT_FAILURE)
				return ;
	}
	ft_sort(e);
	ft_printsyms(e);
}

void		ft_handle_64(t_env *e, void *ptr)
{
	uint32_t				i;
	struct load_command		*lc;
	struct mach_header_64	*head;

	i = 0;
	e->width = 16;
	head = (struct mach_header_64 *)ptr;
	lc = ptr + sizeof(*head);
	while (i++ < rev32(e, head->ncmds))
	{
		if (fileout(e, lc) == EXIT_FAILURE)
			return ;
		if (rev32(e, lc->cmd) == LC_SYMTAB)
		{
			print_64(e, ptr, (struct symtab_command *)lc);
			return ;
		}
		lc = (void *)lc + rev32(e, lc->cmdsize);
	}
	ft_perror(e, "No symbol found");
}
