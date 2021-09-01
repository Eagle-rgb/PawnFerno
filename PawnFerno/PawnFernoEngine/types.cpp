#include "types.h"

int directionIndex(Direction d) {
	switch (d)
	{
	case NORTH: return 6;
	case EAST: return 0;
	case SOUTH: return 2;
	case WEST: return 4;
	case SOUTHEAST: return 1;
	case SOUTHWEST: return 3;
	case NORTHWEST: return 5;
	case NORTHEAST: return 7;
	default: return -1;
	}
}