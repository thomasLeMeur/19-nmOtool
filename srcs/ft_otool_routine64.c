/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool_routine64.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/24 21:18:20 by tle-meur          #+#    #+#             */
/*   Updated: 2017/02/24 21:19:13 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static void	ft_printmem_64(t_env *e, struct section_64 *sec, void *ptr)
{
	uint64_t	i;
	uint64_t	j;
	uint64_t	lim;

	i = 0;
	lim = rev64(e, sec->size);
	while (i < lim)
	{
		j = 0;
		ft_printf("%.16llx\t", sec->addr + i);
		while (j < 16 && i + j < lim)
			ft_printf("%.2hhx ",
					*(char *)(ptr + rev32(e, sec->offset) + i + j++));
		ft_putchar('\n');
		i += 16;
	}
}

static int	loopsections_64(t_env *e, struct segment_command_64 *seg,
			void *ptr)
{
	uint32_t			i;
	int					cnt;
	struct section_64	*sec;

	i = 0;
	cnt = 0;
	sec = (struct section_64 *)((void *)seg + sizeof(*seg));
	while (i++ < rev32(e, seg->nsects))
	{
		if (fileout(e, sec) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (ft_strcmp(sec->segname, SEG_TEXT) == 0 &&
				ft_strcmp(sec->sectname, SECT_TEXT) == 0)
		{
			ft_printf("%s:\n", e->filename);
			ft_printf("Contents of (%s,%s) section\n", SEG_TEXT, SECT_TEXT);
			ft_printmem_64(e, sec, ptr);
			return (EXIT_SUCCESS);
		}
		sec++;
	}
	return (EXIT_SUCCESS);
}

void		ft_handle_64(t_env *e, void *ptr)
{
	uint32_t					i;
	int							cnt;
	struct load_command			*lc;
	struct segment_command_64	*seg;
	struct mach_header_64		*head;

	i = 0;
	cnt = 0;
	e->width = 16;
	head = (struct mach_header_64 *)ptr;
	lc = ptr + sizeof(*head);
	while (i++ < rev32(e, head->ncmds))
	{
		if (fileout(e, lc) == EXIT_FAILURE)
			return ;
		if (rev32(e, lc->cmd) == LC_SEGMENT_64 && ++cnt)
		{
			seg = (struct segment_command_64 *)lc;
			if (loopsections_64(e, seg, ptr) == EXIT_FAILURE)
				return ;
		}
		lc = (void *)lc + rev32(e, lc->cmdsize);
	}
	if (cnt == 0)
		ft_perror(e, "No symbol found");
}
