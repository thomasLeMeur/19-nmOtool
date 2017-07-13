/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool_routine32.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/24 21:17:17 by tle-meur          #+#    #+#             */
/*   Updated: 2017/02/27 11:49:09 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static void	ft_printmem_32(t_env *e, struct section *sec, void *ptr)
{
	uint64_t	i;
	uint64_t	j;
	uint64_t	lim;

	i = 0;
	lim = rev32(e, sec->size);
	while (i < lim)
	{
		j = 0;
		ft_printf("%.8llx\t", rev32(e, sec->addr) + i);
		while (j < 16 && i + j < lim)
		{
			if (e->ppc == 1)
				ft_printf("%.2hhx",
						*(char *)(ptr + rev32(e, sec->offset) + i + j++));
			else
				ft_printf("%.2hhx ",
						*(char *)(ptr + rev32(e, sec->offset) + i + j++));
			if (e->ppc == 1 && j % 4 == 0)
				ft_putchar(' ');
		}
		ft_putchar('\n');
		i += 16;
	}
}

static int	loopsections_32(t_env *e, struct segment_command *seg, void *ptr)
{
	uint32_t		i;
	int				cnt;
	struct section	*sec;

	i = 0;
	cnt = 0;
	sec = (struct section *)((void *)seg + sizeof(*seg));
	while (i++ < rev32(e, seg->nsects))
	{
		if (fileout(e, sec) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (ft_strcmp(sec->segname, SEG_TEXT) == 0 &&
				ft_strcmp(sec->sectname, SECT_TEXT) == 0)
		{
			ft_printf("%s:\n", e->filename);
			ft_printf("Contents of (%s,%s) section\n", SEG_TEXT, SECT_TEXT);
			ft_printmem_32(e, sec, ptr);
			return (EXIT_SUCCESS);
		}
		sec++;
	}
	return (EXIT_SUCCESS);
}

void		ft_handle_32(t_env *e, void *ptr)
{
	uint32_t				i;
	int						cnt;
	struct load_command		*lc;
	struct segment_command	*seg;
	struct mach_header		*head;

	i = 0;
	cnt = 0;
	e->width = 8;
	head = (struct mach_header *)ptr;
	lc = ptr + sizeof(*head);
	while (i++ < rev32(e, head->ncmds))
	{
		if (fileout(e, lc) == EXIT_FAILURE)
			return ;
		if (rev32(e, lc->cmd) == LC_SEGMENT && ++cnt)
		{
			seg = (struct segment_command *)lc;
			if (loopsections_32(e, seg, ptr) == EXIT_FAILURE)
				return ;
		}
		lc = (void *)lc + rev32(e, lc->cmdsize);
	}
	if (cnt == 0)
		ft_perror(e, "No symbol found");
}
