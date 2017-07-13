/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/24 21:16:55 by tle-meur          #+#    #+#             */
/*   Updated: 2017/02/27 11:48:35 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

int		main(int ac, char **av)
{
	t_env		e;
	int			i;

	ft_strcpy(e.progname, av[0]);
	if (initenv(&e, av[0], FT_OTOOL) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	e.cputype = ((struct mach_header *)e.beg)->cputype;
	e.cpusubtype = ((struct mach_header *)e.beg)->cpusubtype;
	clearenv(&e);
	i = 0;
	while (i++ < ac)
	{
		if (i == ac && ac != 1)
			break ;
		e.print = (ac > 2) ? 1 : 0;
		if (initenv(&e, (ac == 1) ? "a.out" : av[i], FT_OTOOL) == EXIT_SUCCESS)
		{
			ft_core(&e, e.beg);
			clearenv(&e);
		}
	}
	return (EXIT_SUCCESS);
}
