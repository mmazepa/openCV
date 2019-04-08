#include "TrackingObject.h"

int x, y;

void setPoint(int new_x, int new_y) {
	x = new_x;
	y = new_y;
};

void setPositionX(int new_x) {
	x = new_x;
};
int getPositionX() {
	return x;
};

void setPositionY(int new_y) {
	y = new_y;
};
int getPositionY() {
	return y;
};
