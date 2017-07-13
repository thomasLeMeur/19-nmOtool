/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/24 21:10:41 by tle-meur          #+#    #+#             */
/*   Updated: 2017/02/27 11:54:35 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

int		initenv(t_env *e, char *name, int type)
{
	int			ret;
	struct stat	buf;

	ft_strncpy(e->filename, name, 255);
	e->filename[255] = '\0';
	if ((e->fd = open(name, O_RDONLY)) < 0)
		ft_perror(e, "Can't open the file");
	else if ((ret = fstat(e->fd, &buf)) < 0)
		ft_perror(e, "Can't stat the file");
	else if ((e->beg = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE,
			e->fd, 0)) == MAP_FAILED)
		ft_perror(e, "Can't load the file");
	else if ((e->syms = (char **)malloc(1 * sizeof(char *))) == NULL)
		ft_perror(e, "Malloc error");
	if ((e->rev = 0) || e->fd < 0 || ret < 0 || e->beg == MAP_FAILED ||
		(e->nbsyms = 0) || e->syms == NULL)
	{
		if (e->fd >= 0)
			close(e->fd);
		return (EXIT_FAILURE);
	}
	e->prog = type;
	e->filesize = buf.st_size;
	e->end = e->beg + e->filesize;
	return (EXIT_SUCCESS);
}

void	clearenv(t_env *e)
{
	int	i;

	if (munmap(e->beg, e->filesize) < 0)
		ft_perror(e, "Can't free the file");
	if (e->syms != NULL)
	{
		i = -1;
		while (++i < e->nbsyms)
			if (e->syms[i] != NULL)
				free(e->syms[i]);
		free(e->syms);
	}
	close(e->fd);
}

void	ft_sort(t_env *e)
{
	int		i;
	int		j;
	int		ret;
	char	*tmp;

	i = 0;
	while (i < e->nbsyms)
	{
		j = i + 1;
		while (j < e->nbsyms)
		{
			if ((ret = ft_strcmp(e->syms[i] + e->width + 3,
							e->syms[j] + e->width + 3)) == 0)
				ret = ft_strcmp(e->syms[i], e->syms[j]);
			if (ret > 0)
			{
				tmp = e->syms[i];
				e->syms[i] = e->syms[j];
				e->syms[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	ft_sethex(t_env *e, char *s)
{
	size_t		i;
	size_t		len;
	char		buf[34];
	static char	grammar[17] = "0123456789abcdef";

	len = 0;
	ft_bzero(buf, 34 * sizeof(char));
	if (!(i = 0) && e->value != 0)
	{
		while (e->value)
		{
			buf[i++] = grammar[e->value % 16];
			e->value /= 16;
		}
		i = 0 * (len -= len - i);
		while (i < len / 2 && ((buf[32] = buf[i]) || 1))
		{
			buf[i] = buf[len - 1 - i];
			buf[len - 1 - i++] = buf[32];
		}
	}
	i = 0;
	while (len++ < (size_t)e->width)
		s[i++] = '0';
	ft_strcpy(s + i, buf);
}

void	*ft_addline(t_env *e)
{
	int		i;
	char	**tab;

	if ((tab = (char **)malloc((e->nbsyms + 2) * sizeof(char *))) == NULL)
		return (NULL);
	else if ((tab[e->nbsyms] = (char *)malloc((e->width + 3 +
						ft_strlen(e->sym) + 1) * sizeof(char))) == NULL)
	{
		free(tab);
		return (NULL);
	}
	if ((i = 0) || e->letter == 'U' || e->letter == 'u')
		while (i < e->width)
			tab[e->nbsyms][i++] = ' ';
	else
		ft_sethex(e, tab[e->nbsyms]);
	tab[e->nbsyms][e->width + 0] = ' ';
	tab[e->nbsyms][e->width + 1] = e->letter;
	tab[e->nbsyms][e->width + 2] = ' ';
	ft_strcpy(tab[e->nbsyms] + e->width + 3 + 0 * (i = -1), e->sym);
	while (++i < e->nbsyms)
		tab[i] = e->syms[i];
	tab[++e->nbsyms] = NULL;
	free(e->syms);
	return ((e->syms = tab));
}
