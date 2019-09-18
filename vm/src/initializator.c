/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilian <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/13 13:45:23 by ilian             #+#    #+#             */
/*   Updated: 2019/09/13 13:45:31 by ilian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

t_vm	*create_game()
{
	t_vm	*vm;
	
	vm = (t_vm*)ft_memalloc(sizeof(t_vm));
	vm->cycles_to_die = CYCLE_TO_DIE;
	vm->cycles_till_next_check = CYCLE_TO_DIE;
	vm->catalog = get_commad_catalog();
	return (vm);
}

void	create_arena(t_vm *vm)
{
	t_champ	*ch;
	int		i;

    vm->arena = (char*)ft_memalloc(MEM_SIZE);
    // ft_memset(vm->arena, '0', MEM_SIZE * 2);
	ch = vm->champs;
	i = 0;
	while (ch)
	{
		ft_memcpy(&(vm->arena[i]), ch->byte_code->code, ch->byte_code->length);
		i += MEM_SIZE / get_quantity_players(vm->champs);
		ch = ch->next_champ;
	}
}

void	create_carriages(t_vm *vm)
{
	t_carriage	*car;
	t_carriage	*tmp;
	t_champ		*ch;
	int			i;

	i = 0;
	tmp = NULL;
	car = NULL;
	ch = vm->champs;
	while (ch)
	{
		car = (t_carriage*)ft_memalloc(sizeof(t_carriage));
		car->location = i * MEM_SIZE / get_quantity_players(vm->champs);
		car->id = i++;
		car->registers[1] = ch->id * -1;
		car->next = tmp;
		car->op = get_com_by_code(vm->catalog, get_i(vm->arena, car->location));
		car->cycles_to_run = car->op->cycles;
		tmp = car;
		ch = ch->next_champ;
	}
	vm->cars = car;
}

t_carriage	*copy_carriage(t_vm *vm, t_carriage *car)
{
	t_carriage	*new_car;
	t_carriage	*tmp;
	int			i;

	if (!vm || !car)
		return (NULL);
	new_car = (t_carriage*)ft_memalloc(sizeof(t_carriage));
	*new_car = (t_carriage){car->id, car->location, car->carry, car->registers, car->live, car->op, car->cycles_to_run, NULL};
	i = -1;
	while (++i < REG_NUMBER)
		new_car->registers[i] = car->registers[i];
	if (!vm->cars)
	{
		vm->cars = new_car;
		return (new_car);
	}
	tmp = vm->cars;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_car;
	return (new_car);
}

t_champ *create_champ(t_byte_code *str_champ, int id)
{
    t_champ 		*champ;
	char			*code;
	unsigned int	magic;
	int				res;
	t_header		*header;

	magic = byte_to_int(str_champ->code);
	if (magic != COREWAR_EXEC_MAGIC)
		return NULL;
    header = (t_header*)ft_memalloc(sizeof(t_header));
	header->magic = magic;
	str_champ->code += 4;
	ft_memcpy(header->prog_name, str_champ->code, PROG_NAME_LENGTH);
	str_champ->code += PROG_NAME_LENGTH + 4;
	header->prog_size = byte_to_int(str_champ->code);
	str_champ->code += 4;
	ft_memcpy(header->comment, str_champ->code, COMMENT_LENGTH);
	str_champ->code += COMMENT_LENGTH + 4;
	str_champ->length -= PROG_NAME_LENGTH + COMMENT_LENGTH + 4 * 4;
    champ = (t_champ*)malloc(sizeof(t_champ));
    *champ = (t_champ){id, header, str_champ, NULL, NULL};
    return (champ);
}