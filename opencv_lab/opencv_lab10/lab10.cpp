#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/objdetect/objdetect.hpp>
#include<iostream>
#include<conio.h>

using namespace cv;
using namespace std;

Mat frame, gray;
VideoCapture cap;

String path = "M:/Programy/OpenCV/opencv/sources/data/haarcascades/";
CascadeClassifier face_cascade = CascadeClassifier(path + "haarcascade_frontalface_default.xml");
CascadeClassifier eyes_cascade = CascadeClassifier(path + "haarcascade_eye_tree_eyeglasses.xml");

// double...
int scaleFactor = 5;
int scaleFactor_max = 100;

// int...
int minNeighbors = 5;
int minNeighbors_max = 100;

// size...
int minSize = 5;
int minSize_max = 100;

void ValueChanger() {
	vector<Rect> faces;
	face_cascade.detectMultiScale(gray, faces);
	//face_cascade.detectMultiScale(gray, faces, (double)scaleFactor, minNeighbors, 0, Size(minSize, minSize));

	for (int i = 0; i < faces.size(); i++) {
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		int face_radius = cvRound((faces[i].width + faces[i].height)*0.25);
		circle(frame, center, face_radius, Scalar(0, 100, 0), 3);
		//GaussianBlur(frame, frame, Size(faces[i].width / 2, faces[i].height / 2), 3);

		Mat faceROI = gray(faces[i]);
		vector<Rect> eyes;
		eyes_cascade.detectMultiScale(faceROI, eyes);
		//eyes_cascade.detectMultiScale(faceROI, eyes, (double)scaleFactor, minNeighbors, 0, Size(minSize, minSize));

		for (int j = 0; j < eyes.size(); j++) {
			Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
			int eye_radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			circle(frame, eye_center, eye_radius, Scalar(0, 0, 200), 3);
		}

		if (eyes.size() == 2) {
			int width = faces[i].x + faces[i].width/2;
			int height = faces[i].y + eyes[0].height;
			Rect censored_black = Rect(faces[i].x + eyes[0].x - 75, faces[0].y + eyes[0].y, width*0.75, height*0.5);
			rectangle(frame, censored_black, Scalar(0, 0, 0), CV_FILLED);
		}
	}
}

void ValueChange(int, void*) {
	ValueChanger();
}

int main() {
	cap.open(0);

	namedWindow("window", CV_WINDOW_AUTOSIZE);

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	//createTrackbar("scaleFactor", "window", &scaleFactor, scaleFactor_max, ValueChange);
	//createTrackbar("minNeighbors", "window", &minNeighbors, minNeighbors_max, ValueChange);
	//createTrackbar("minSize", "window", &minSize, minSize_max, ValueChange);

	while (1) {
		try {
			cap >> frame;
			cvtColor(frame, gray, CV_RGB2GRAY);
			equalizeHist(gray, gray);

			ValueChanger();

			imshow("window", frame);
		} catch (Exception e) {
			cap.open(1);
		}

		if (waitKey(15) == 27) {
			cap.release();
			destroyAllWindows();
			return 0;
		}
	}
	return 0;
}