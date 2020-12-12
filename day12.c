#include <stdio.h>
#include <stdlib.h>

typedef struct __state {
	int		x, y;
	int		sx, sy;
	/* east = 0, north = 1, cyc */
	int		dir;
} state;

typedef void (*transition)(state *ship, int arg);

void  north(state *ship, int arg) { ship->y += arg; }
void  south(state *ship, int arg) { ship->y -= arg; }
void   east(state *ship, int arg) { ship->x += arg; }
void   west(state *ship, int arg) { ship->x -= arg; }
void  left1(state *ship, int arg) { ship->dir += arg / 90; }
void right1(state *ship, int arg) { ship->dir -= arg / 90; }
void   fwd1(state *ship, int arg) {
	switch (ship->dir & 3) {
	case 0:  east(ship, arg); break;
	case 1: north(ship, arg); break;
	case 2:  west(ship, arg); break;
	case 3: south(ship, arg); break;
	}
}
void left2(state *ship, int arg) {
	int tmp;
	switch ((arg / 90) & 3) {
	case 1: tmp = ship->x; ship->x = -ship->y; ship->y = tmp; break;
	case 2: ship->x = -ship->x; ship->y = -ship->y; break;
	case 3: tmp = ship->y; ship->y = -ship->x; ship->x = tmp; break;
	}
}
void right2(state *ship, int arg) { left2(ship, -arg); }
void fwd2(state *ship, int arg) {
	ship->sx += arg * ship->x;
	ship->sy += arg * ship->y;
}
transition action1(char c) {
	switch (c) {
	case 'N': return north;
	case 'S': return south;
	case 'E': return east;
	case 'W': return west;
	case 'L': return left1;
	case 'R': return right1;
	case 'F': return fwd1;
	}
	return NULL;
}
transition action2(char c) {
	switch (c) {
	case 'N': return north;
	case 'S': return south;
	case 'E': return east;
	case 'W': return west;
	case 'L': return left2;
	case 'R': return right2;
	case 'F': return fwd2;
	}
	return NULL;
}

int main() {
	char	dir;
	int	arg;
	state	state1, state2;

	state1.x = state1.y = state1.dir = 0;
	state2.sx = state2.sy = 0;
	state2.x = 10;
	state2.y = 1;
	while (scanf("%c%d ", &dir, &arg) == 2) {
		action1(dir)(&state1, arg);
		action2(dir)(&state2, arg);
	}
	printf("%d %d\n", abs(state1.x) + abs(state1.y), abs(state2.sx) + abs(state2.sy));
	return 0;
}
