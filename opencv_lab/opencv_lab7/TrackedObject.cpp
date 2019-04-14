#include "TrackedObject.h"

TrackedObject::TrackedObject(Point new_point, Moments new_m) {
	point = new_point;
	m = new_m;
}

void TrackedObject::setPoint(int x, int y) {
	point = Point(x, y);
};
Point TrackedObject::getPoint() {
	return point;
};

void TrackedObject::setMoments(Moments new_m) {
	m = new_m;
};
Moments TrackedObject::getMoments() {
	return m;
};
