#include "Action.h"

ActionInterpolation<Uint8>* aalpha(Actor* actor, int duration, int from, int to)
{
	return new ActionInterpolation<Uint8>(actor->color.a, duration, from, to, LINAR);
}

ActionInterpolation<Uint8>* aalpha(Actor* actor, int duration, int to) {
	return new ActionInterpolation<Uint8>(actor->color.a, duration, to, LINAR);
}

ActionInterpolation<double>* arotate(Actor* actor, int duration, double to) {
	return new ActionInterpolation<double>(actor->rotation, duration, to, SINE);
}

ActionInterpolation<double>* arotate(Actor* actor, int duration, double from, double to) {
	return new ActionInterpolation<double>(actor->rotation, duration, from, to, SINE);
}

ActionParallel* amove(Actor* actor, int duration, VecI from, VecI to, bool acenter)
{
	if (acenter) {
		return new ActionParallel({ new ActionInterpolation<int>(actor->position.x, duration, from.x, to.x-actor->size.x/2, SINE),
			new ActionInterpolation<int>(actor->position.y, duration, from.y, to.y-actor->size.y/2, SINE) });
	} else {
		return new ActionParallel({ new ActionInterpolation<int>(actor->position.x, duration, from.x, to.x, SINE),
			new ActionInterpolation<int>(actor->position.y, duration, from.y, to.y, SINE) });
	}
}

ActionParallel* amove(Actor* actor, int duration, VecI to, bool acenter)
{
	return amove(actor, duration, actor->position, to, acenter);
}
