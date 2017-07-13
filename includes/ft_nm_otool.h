/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm_otool.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/24 21:09:52 by tle-meur          #+#    #+#             */
/*   Updated: 2017/02/27 11:46:24 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_OTOOL_H
# define FT_NM_OTOOL_H

# include <ar.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <mach-o/fat.h>
# include <mach-o/nlist.h>
# include <mach-o/loader.h>
# include <mach-o/ranlib.h>
# include "libft.h"

# define FT_NM		1
# define FT_OTOOL	2

typedef struct	s_obj
{
	uint64_t		addr;

	struct s_obj	*next;

	char			*name;
	int				s;
	int				off;
}				t_obj;

typedef struct	s_env
{
	int				rev;
	unsigned int	magic;
	cpu_type_t		cputype;
	cpu_subtype_t	cpusubtype;

	int				nbsyms;
	char			**syms;

	char			letter;
	uint64_t		value;
	char			*sym;

	int				sects_cnt;
	int				width;
	int				print;
	void			*ptr;
	uint32_t		cnt;
	uint32_t		lim;
	int				ppc;
	uint32_t		m;

	void			*beg;
	void			*end;

	int				fd;
	size_t			filesize;
	char			filename[256];

	int				prog;
	char			progname[256];
}				t_env;

/*
**	ft_env.c
*/
void			ft_sort(t_env *e);
void			clearenv(t_env *e);
void			*ft_addline(t_env *e);
void			ft_sethex(t_env *e, char *s);
int				initenv(t_env *e, char *name, int type);

/*
**	ft_arch.c
*/
void			ft_callgoodarchs(t_env *e, struct fat_header *head, int *vars);
void			ft_protectmultiarchs(t_env *e, struct fat_header *head,
				int *vars);

/*
**	ft_archive.c
*/
int				ft_getsize(char *s);
void			ft_readarchive(t_env *e, void *ptr, struct ar_hdr *h);
void			ft_find64arch(t_env *e, struct fat_header *head, int *vars);

/*
**	ft_print.c
*/
void			ft_printsyms(t_env *e);
void			ft_perror(t_env *e, char *error);
void			ft_nm_printobjs(t_env *e, t_obj *lst);
void			ft_otool_printobjs(t_env *e, t_obj *lst);

/*
**	ft_utilities.c
*/
void			ft_core(t_env *e, void *ptr);
int				fileout(t_env *e, void *addr);
uint32_t		rev32(t_env *e, uint32_t value);
uint64_t		rev64(t_env *e, uint64_t value);
void			ft_findarch(t_env *e, void *ptr);

/*
**	ft_nm_routine32.c and ft_otool_routine32.c
*/
void			ft_handle_32(t_env *e, void *ptr);

/*
**	ft_nm_routine64.c and ft_otool_routine64.c
*/
void			ft_handle_64(t_env *e, void *ptr);

#endif
