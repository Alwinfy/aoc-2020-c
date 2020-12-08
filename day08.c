#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arraylist.h"

typedef enum __statecode {
	CONTINUE = 0,
	LOOP,
	SEGFAULT
} statecode;

struct __boot_state;

typedef void (*ins_ptr)(struct __boot_state *state, int arg);

typedef struct __instruction {
	ins_ptr	action;
	int	arg;
	int	visits;
} instruction;

typedef struct __boot_state {
	instruction	*pc;
	int		acc;
	statecode	code;
} boot_state;
	

void ins_nop(boot_state *state, int arg) {
	(void) state, (void) arg;
}
void ins_acc(boot_state *state, int arg) {
	state->acc += arg;
}
void ins_jmp(boot_state *state, int arg) {
	state->pc += arg - 1;
}

ins_ptr get_ins(char *name) {
	if (!strcmp(name, "nop")) {
		return ins_nop;
	}
	if (!strcmp(name, "acc")) {
		return ins_acc;
	}
	if (!strcmp(name, "jmp")) {
		return ins_jmp;
	}
	return NULL;
}

void run(alist *program, boot_state *state) {
	instruction	*front, *back;
	size_t		i;

	for (i = 0; i < program->size; i++) {
		alist_get(instruction, program, i).visits = 0;
	}
	front = program->list;
	back = alist_back(program);

	state->pc = front;
	state->acc = 0;
	state->code = CONTINUE;
	while (!state->code) {
		instruction *fetch = state->pc++;
		if (fetch < front || fetch > back) {
			state->code = SEGFAULT;
			continue;
		}
		if (fetch->visits++) {
			state->code = LOOP;
			continue;
		}
		fetch->action(state, fetch->arg);
	}
}

int main() {
	alist		program;
	boot_state	state;
	char		op[4];
	int		arg;
	size_t		i;

	alist_init(instruction, &program);
	while (scanf("%3s%d", op, &arg) == 2) {
		instruction	next;

		next.arg = arg;
		next.action = get_ins(op);
		if (!next.action) {
			printf("Bad opcode: %s\n", op);
			continue;
		}
		alist_add(&program, &next);
	}

	run(&program, &state);
	printf("%d\n", state.acc);

	for (i = 0; i < program.size; i++) {
		instruction *ins = alist_get_typeless(&program, i);
		if (ins->action == ins_acc) continue;
		ins->action = ins->action == ins_nop ? ins_jmp : ins_nop;
		run(&program, &state);
		if (state.code == SEGFAULT) {
			break;
		}
		ins->action = ins->action == ins_nop ? ins_jmp : ins_nop;
	}
	printf("%d\n", state.acc);

	alist_free(&program);
	return 0;
}
