#include "Vector2.h"

VecD i2d(VecI x) {
	return VecD( x.x,x.y );
}

VecI d2i(VecD x) {
	return VecI(x.x,x.y );
}