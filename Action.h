#pragma once
#include <math.h>
#include <vector>
using namespace std;

class Action {
public:
	virtual void tick() = 0;
	virtual bool isFinished() = 0;
	virtual void reset() = 0;
};

//===========================================================================
enum InterpolationMethod {
	LINAR,
	SINE
};
template<typename T> class ActionInterpolation : public Action {
public:
	T& ref;
	int now;
	int duration;
	T from;
	T to;
	int method;

	ActionInterpolation(T& ref, int duration, T from, T to, int method = LINAR) : ref(ref), now(0), duration(duration), from(from), to(to), method(method) {}
	ActionInterpolation(T& ref, int duration, T to, int method = LINAR) : ref(ref), now(0), duration(duration), from(ref), to(to), method(method) {}

	void tick() {

		T delta = to - from;

		if (now < duration) {
			now++;


			switch (method) {
			case LINAR:
				ref = from + (to - from) / duration * now;
				break;
			case SINE:
				ref = from + (to - from) * sin(1.0 * now / duration * 3.14159265 / 2);
				break;
			}
		}
	}

	bool isFinished() {
		return now >= duration;
	}

	void reset() {
		now = 0;
		ref = from;
	}
};
//===========================================================================
class ActionRepeatForever : public Action {
public:
	Action* action;
	ActionRepeatForever(Action* action) : action(action) {}

	void tick() {
		if (action == NULL) {
			return;
		}

		action->tick();
		if (action->isFinished()) {
			action->reset();
		}
	}

	bool isFinished() {
		return false;
	}

	void reset() {
		action->reset();
	}
};