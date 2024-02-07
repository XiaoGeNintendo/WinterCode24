#include "Action.h"

ActionInterpolation<Uint8>* aalpha(Actor* actor, int duration, int from, int to)
{
	return new ActionInterpolation<Uint8>(actor->color.a, duration, from, to, LINAR);
}

ActionInterpolation<Uint8>* aalpha(Actor* actor, int duration, int to) {
	return new ActionInterpolation<Uint8>(actor->color.a, duration, to, LINAR);
}
