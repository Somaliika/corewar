#include "corewar.h"


int			show_error(const char *error, int err_index)
{
    // system("leaks asm");
    if (err_index < 0)
	    ft_printf("%t%s%t\n", B_RED, error, EOC);
    else
        ft_printf("%t%s%i%t\n", B_RED, error, err_index + 1, EOC);
	exit(0);
}

int valid_magic(char *magic)
{
    if (magic[0] == '0' && (magic[1] == 'x' || magic[1] == 'X'))
        return (1);
    return (0);
}

int valid_champ_name(char *name)
{
    int len;

    len = ft_strlen(name);
    if (len < 2 || name[len - 1] != 's' || name[len - 2] != '.')
        return (0);
    return (1);
}

int valid_head(t_list *champ)
{
    int i;
    int is_name;
    int is_comment;

    is_name = 0;
    is_comment = 0;
    i = 0;
    while (champ)
    {
        if (ft_strstr(champ->content, NAME_CMD_STRING) && !is_name)
            is_name = 1;
        else if (ft_strstr(champ->content, COMMENT_CMD_STRING) && !is_comment)
            is_comment = 1;
        else
        {
            ft_printf("%s %i\n", "Syntax error at line:", i);
            return (0);
        }
        if (is_name && is_comment)
            return (1);
        champ = champ->next;
        i++;
    }
    return (0);
}

void    valid_label(char *line, int pos_label_char, int line_index)
{
    int status;

    status = 0;
    while (--pos_label_char >= 0)
    {
        if ((status == 0 || status == 1) && ft_strchr(LABEL_CHARS, line[pos_label_char]))
            status = 1;
        else if ((status == 1 || status == 2) && ft_strchr(" \t", line[pos_label_char]))
            status = 2;
        else
            show_error("Label error in line: ", line_index);
    }
}

void    valid_command_line(char *line, int line_index)
{
    int i;
    int j;
    int is_print;

    i = -1;
    j = -1;
    is_print = 0;
    while (line[++j] != '\0')
    {
        if (line[j] == COMMENT_CHAR)
            break;
        else if (is_print && (line[j] == ' ' || line[j] == '\t'))
            break;
        else if (line[j] != ' ' && line[j] != '\t')
            is_print = 1;
        if (line[j] == LABEL_CHAR)
        {
            valid_label(line, j, line_index);
            i = j;
            break;
        }
    }
    while (line[++i] != '\0' && (line[i] == ' ' || line[i] == '\t'));
    if (line[i] != COMMENT_CHAR && line[i] != '\0')
        valid_operation(&line[i], line_index);
}

void valid_champ_file(t_list *champ)
{
    t_list *tmp;
    int i;

    if (!valid_head(champ))
        show_error("Head file error", -1);
    tmp = champ->next->next;
    i = 2;
    while (tmp)
    {
        valid_command_line(tmp->content, i);
        tmp = tmp->next;
        i++;
    }
}
void valid_existing_labels(t_list *code_list)
{
    t_list *tmp;

    tmp = code_list;
    while (tmp)
    {
        tmp = tmp->next;
    }
}