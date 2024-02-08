#pragma once
#include <math.h>
#include <vector>
#include <functional>
#include "Actor.h"
#include <string>
#include "Label.h"
using namespace std;

class Action {
public:
	virtual void tick() = 0;
	virtual bool isFinished() = 0;
	virtual void reset() = 0; 
	virtual ~Action() = default;
	virtual string describe();
};

//===========================================================================
enum InterpolationMethod {
	LINAR,
	SINE
};

template<typename T> class ActionInterpolation : public Action {
private:

	void update() {
		switch (method) {
		case LINAR:
			ref = from * 1.0 + (to - from) * 1.0 / duration * now;
			break;
		case SINE:
			ref = from * 1.0 + (to - from) * sin(1.0 * now / duration * 3.14159265 / 2);
			break;
		}
	}

public:
	T& ref;
	int now;
	int duration;
	T from;
	T to;
	int method;

	ActionInterpolation(T& ref, int duration, T from, T to, int method = LINAR) : ref(ref), now(0), duration(duration), from(from), to(to), method(method) {}
	ActionInterpolation(T& ref, int duration, T to, int method = LINAR) : ref(ref), now(0), duration(duration), from(ref), to(to), method(method) {}

	string describe() override {
		return "Interpolation some property for " + to_string(duration) + "f " + to_string(from) + "-->" + to_string(to);
	}

	void tick() override {
		if (now < duration) {
			now++;

			update();
		}
	}

	bool isFinished() override {
		return now >= duration;
	}

	void reset() override {
		now = 0;
		ref = from;
	}
};
//===========================================================================
class ActionRepeatForever : public Action {
public:
	Action* action;
	ActionRepeatForever(Action* action) : action(action) {}

	~ActionRepeatForever() override {
		delete action;
	}

	string describe() override {
		return "Repeating forever for: " + action->describe();
	}

	void tick() override {
		if (action == NULL) {
			return;
		}

		action->tick();
		if (action->isFinished()) {
			action->reset();
		}
	}

	bool isFinished() override {
		return false;
	}

	void reset() override {
		action->reset();
	}
};
//===========================================================================
class ActionDelay : public Action {
public:
	int now = 0;
	int total;

	ActionDelay(int duration) :total(duration) {}

	string describe() override {
		return "Delay for " + to_string(total);
	}

	void tick() override {
		if (now < total) {
			now++;
		}
	}

	bool isFinished() override {
		return now >= total;
	}

	void reset() override {
		now = 0;
	}
};

//===========================================================================
class ActionSequence : public Action {
public:
	vector<Action*> actions;
	int now = 0;

	ActionSequence(vector<Action*> actions) :actions(actions) {}

	string describe() override {
		string s = "Sequence: ";
		for (auto x : actions) {
			s += x->describe();
			s += "then ";
		}
		s += "finally finish";
		return s;
	}

	~ActionSequence() override {
		for (auto x : actions) {
			delete x;
		}
	}

	void tick() override {
		if (now == actions.size()) {
			return;
		}

		if (actions[now]->isFinished()) {
			now++;
		}

		if (now == actions.size()) {
			return;
		}

		actions[now]->tick();
	}

	bool isFinished() override {
		return now == actions.size();
	}

	void reset() override {
		for (auto x : actions) {
			x->reset();
		}
		now = 0;
	}
};
//===========================================================================
class ActionParallel : public Action {
public:
	vector<Action*> actions;
	ActionParallel(vector<Action*> actions) : actions(actions) {}

	~ActionParallel() override {
		for (auto x : actions) {
			delete x;
		}
	}

	string describe() override {
		string s = "Doing: ";
		for (auto x : actions) {
			s += x->describe();
			s += " ";
		}
		s += "together";
		return s;
	}

	void tick() override {
		for (auto x : actions) {
			if (!x->isFinished()) {
				x->tick();
			}
		}
	}

	void reset() override {
		for (auto x : actions) {
			x->reset();
		}
	}

	bool isFinished() override{
		for (auto x : actions) {
			if (!x->isFinished()) {
				return false;
			}
		}
		return true;
	}
};


//===========================================================================

class ActionRunnable : public Action {
public:
	function<void()> func;
	bool done = false;

	ActionRunnable(function<void()> func) : func(func) {}

	string describe() override {
		return "Run native code";
	}

	void tick() override{
		func();
		done = true;
	}

	bool isFinished() override{
		return done;
	}

	void reset() {
		done = false;
	}
};

//===========================================================================

//Helpful functions

ActionInterpolation<Uint8>* aalpha(Actor* actor, int duration, int from, int to);

ActionInterpolation<Uint8>* aalpha(Actor* actor, int duration, int to);


ActionInterpolation<int>* afont(Label* actor, int duration, int to);

ActionInterpolation<int>* afont(Label* actor, int duration, int from, int to);


ActionParallel* amove(Actor* actor, int duration, VecI from, VecI to, bool acenter=false);

ActionParallel* amove(Actor* actor, int duration, VecI to, bool acenter = false);

ActionInterpolation<double>* arotate(Actor* actor, int duration, double from, double to);

ActionInterpolation<double>* arotate(Actor* actor, int duration, double to);

ActionParallel* apara(vector<Action*> actions);

ActionSequence* aseq(vector<Action*> actions);

ActionDelay* adelay(int duration);
