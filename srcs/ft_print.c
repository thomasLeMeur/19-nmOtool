/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/24 21:19:29 by tle-meur          #+#    #+#             */
/*   Updated: 2017/02/27 11:48:51 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

void	ft_perror(t_env *e, char *error)
{
	ft_printf_fd(2, "%s: %s: %s\n", e->progname, e->filename, error);
}

void	ft_printsyms(t_env *e)
{
	int	i;

	i = 0;
	while (i < e->nbsyms)
	{
		ft_putendl(e->syms[i++]);
	}
}

void	ft_nm_printobjs(t_env *e, t_obj *lst)
{
	t_obj	*it;
	t_obj	*del;
	int		p;
	char	file[ft_strlen(e->filename) + 1];

	it = lst;
	p = e->print;
	while (it != NULL)
	{
		e->print = 0;
		ft_printf("\n%s(%s):\n", e->filename, it->name);
		ft_core(e, (void *)it->addr);
		del = it;
		it = it->next;
		free(del->name);
		free(del);
		ft_strcpy(file, e->filename);
		clearenv(e);
		if (initenv(e, file, FT_NM) == EXIT_FAILURE)
			return ;
		e->print = p;
	}
}

void	ft_otool_printobjs(t_env *e, t_obj *lst)
{
	t_obj	*it;
	t_obj	*del;
	char	file[ft_strlen(e->filename) + 1];
	int		p;
	char	*newfile;

	it = lst;
	p = e->print;
	ft_strcpy(file, e->filename);
	ft_printf("Archive : %s\n", file);
	while (it != NULL)
	{
		e->print = 0;
		newfile = NULL;
		ft_sprintf(&newfile, "%s(%s)", file, it->name);
		ft_strncpy(e->filename, newfile, 255);
		e->filename[255] = '\0';
		ft_core(e, (void *)it->addr);
		del = it;
		it = it->next;
		free(del->name);
		free(newfile);
		free(del);
	}
}
