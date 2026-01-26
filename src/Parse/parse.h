#ifndef PARSE_H
# define PARSE_H

# include "get_next_line.h"
# include "libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_data
{
	char	*no_path;
	char	*so_path;
	char	*we_path;
	char	*ea_path;
	int		floor_color;
	int		ceiling_color;
	char	**map;
	int		map_width;
	int		map_height;
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
}			t_data;

void		error_exit(char *mg);
void		free_data(t_data *data);

#endif
