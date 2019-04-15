#include<opencv2/highgui/highgui.hpp>
using namespace cv;

class TrackedObject {
private:
	Point point;
	Moments m;
public:
	TrackedObject(Point, Moments);

	void setPoint(int, int);
	Point getPoint();

	void setMoments(Moments);
	Moments getMoments();
};