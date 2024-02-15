#include "Random.h"

mt19937_64 rng{ random_device{}() };

int randInt(int l, int r) {
	uniform_int_distribution<int> d(l, r);
	return d(rng);
}

double randDouble() {
	uniform_real_distribution<double> d(0, 1);
	return d(rng);
}