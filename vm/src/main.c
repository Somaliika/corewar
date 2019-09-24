/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilian <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/13 13:46:39 by ilian             #+#    #+#             */
/*   Updated: 2019/09/13 13:46:41 by ilian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include "visual.h"

void	introducing(t_champ *ch)
{
	ft_printf("Introducing contestants...\n");
	while (ch)
	{
		ft_printf("* Player %i, weighing %i bytes, \"%s\" (\"%s\") !\n",
			ch->id, ch->byte_code->length, ch->header->prog_name, ch->header->comment);
		ch = ch->next_champ;
	}
	
}

void	game(t_vm *vm, t_bool b)
{
	int		res;
	
	res = 1;
	if(b)
		vs_broach(vm);
	else
	{		
		while (cycle(vm) && res)
		{
			res = check(vm);
		}
	}
}

void	winner(t_vm *vm)
{
	if (vm->last_live)
	{
		ft_printf("Contestant %i, \"%s\", has won !\n", vm->last_live->id, &(vm->last_live->header->prog_name[0]));
	}
	else
		ft_printf("There is no winner.\n");
}

int     main(int argc, char **argv)
{
	t_vm	*vm;
	t_champ	*last;
	int		i;
	t_bool b;

	b = false;
	vm = create_game();
	vm->champs = get_champs(argc, argv);
	last = vm->champs;
	printf("first: \\%s/\n", argv[2] );
	while (last->next_champ)
		last = last->next_champ;
	vm->last_live = last;
	create_arena(vm);
	create_carriages(vm);
	if(!ft_strcmp(argv[3], "-v"))
		b = true;
	// system("leaks -q corewar");
	introducing(vm->champs);
	game(vm, b);
	winner(vm);
	return (0);
}
