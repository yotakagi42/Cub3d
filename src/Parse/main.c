#include "parse.h"

// エラーメッセージ(Error : *mg)出してexitする
void	error_exit(char *mg)
{
	printf("Error : %s\n", mg);
	exit(1);
}
// 長さと最後の拡張子を比較する
void	check_extension(char *file)
{
	size_t	len;

	len = ft_strlen(file);
	if (len < 4 || ft_strncmp(file + len - 4, ".cub", 4) != 0)
		error_exit("invalid extension");
}

char	skip_space(char *line)
{
	if (*line == ' ' || *line == '\t' || *line == '\n')
		line++;
	return (line);
}

void	init_date(t_data *data)
{
	data->so_path = NULL;
	data->no_path = NULL;
	data->we_path = NULL;
	data->ea_path = NULL;
	data->floor_color = -1;
	data->ceiling_color = -1;
	data->map = NULL;
	data->map_height = 0;
	data->map_height = 0;
	data->pos_x = 0;
	data->pos_y = 0;
	data->dir_x = 0;
	data->dir_y = 0;
}

void parse_path(char *path)
{
	return;
}

void	identify_line(char *line, t_data *data)
{
	char *ptr;

	ptr = skip_space(line);
	if(!ptr)
		return;
	if(ft_strncmp(ptr,"NO",2) == 0)
		parse_path(ptr);
	else if(ft_strncmp(ptr,"SO",2) == 0)
		parse_path(ptr);
	else if(ft_strncmp(ptr,"WE",2) == 0)
		parse_path(ptr);
	else if(ft_strncmp(ptr,"EA",2) == 0)
		parse_path(ptr);
	else if(ft_strncmp(ptr,"F",1) == 0)
		parse_color(ptr);
	else if(ft_strncmp(ptr,"C",1) == 0)
		parse_color(ptr);
	else
		parse_map(ptr);
}

int	main(int ac, char **av)
{
	int		fd;
	char	*line;
	t_data	data;

	if (ac != 2)
		error_exit("invalid arguments");
	check_extension(av[1]);
	init_date(&data);
	fd = open(av[1], O_RDONLY);
	if (fd < 0)
		error_exit("can not open");
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		identify_line(line, &data);
		free(line);
	}
	close(fd);
	return (0);
}
