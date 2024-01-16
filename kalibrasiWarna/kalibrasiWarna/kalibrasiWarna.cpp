// kalibrasiWarna.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <iostream>
#include <cstring>
#include <ctime>
#include <limits.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

//===========Define Definition=============
#define ul		72
#define H 		0
#define S		1
#define V		2
#define E		0
#define D		1
#define C		2
#define RECT	0
#define CROSS	1
#define ELIPSE	2

//===========IPC Definition=============
#define PI 			3.1415926536

using namespace std;
using namespace cv;
 //===========Global Variables=============
void trackBar_update(), trackBar_updateobs(), trackBar_updateRoi();
void expData(int, int, int, int, int, int, int, int, int);
void expData3(int, int, int, int, int, int, int, int, int);
void expData2(int, int, int, int, int);
void impData(), impData2(), impData3();

int cekExp = 0;
int wMin[3] = { 0,0,0 }, wMax[3] = { 255,255,255 }, ED[3] = { 0,0,0 }, diameter = 0;
int roii[5] = { 0,0,512,512,0 };
int temproii[2] = { 800,600 };
int height, width, step;
int posX = 0, posY = 0, mode = 0;

Mat GetThresImage(Mat, Mat);
Mat Erosion(Mat);
Mat Dilation(Mat);
Mat erosion_src, dilation_src, erosion_dst, dilation_dst;
Mat imPart;
int rad = 320;

Point initialClickPoint, currentMousePoint;
Rect rectangleROI;
bool mouseIsDragging, mouseMove, rectangleSelected;
vector<int> H_ROI, S_ROI, V_ROI;

char file[] = { "E:/ERNA ALFI/SEMESTER 5/Workshop Komputer Visi/Final projek/HSV_data/hsv0.ul" };
string name;
short colorMode = 1;

//===========Data Initialization=============
void clickAndDrag_Rectangle(int event, int x, int y, int flags, void* param) {
	if (event == CV_EVENT_LBUTTONDOWN && mouseIsDragging == false) {
		initialClickPoint = Point(x, y);
		mouseIsDragging = true;
	}
	if (event == CV_EVENT_MOUSEMOVE && mouseIsDragging == true) {
		currentMousePoint = Point(x, y);
		mouseMove = true;
	}
	if (event == CV_EVENT_LBUTTONUP && mouseIsDragging == true) {
		rectangleROI = Rect(initialClickPoint, currentMousePoint);
		mouseIsDragging = mouseMove = false;
		rectangleSelected = true;
	}
	if (event == CV_EVENT_RBUTTONDOWN) {
		wMin[H] = wMin[S] = wMin[V] = 0;
		wMax[H] = wMax[S] = wMax[V] = 0;
		ED[E] = ED[D] = ED[C] = 0;
	}
}
void expData(int MinH, int MinS, int MinV, int MaxH, int MaxS, int MaxV, int Er, int Dl, int Cl) {
	ofstream output(file);
	if (output.is_open()) {
		cout << "Alhamdulillah" << endl;
		output << MinH << ";" << MinS << ";" << MinV << ";" << endl;
		output << MaxH << ";" << MaxS << ";" << MaxV << ";" << endl;
		output << Er << ";" << Dl << ";" << Cl << ";" << endl;
	}
	else cout << "Gak isok bukak!" << endl;
	output.close();
}
void expData2(int MinH, int MinS, int MinV, int MaxH, int Yeah) {
	ofstream output(file);
	if (output.is_open()) {
		cout << "Alhamdulillah bruhh" << endl;
		output << MinH << ";" << MinS << ";" << MinV << ";" << MaxH << ";" << Yeah << ";" << endl;
	}
	else cout << "Gak isok bukak!" << endl;
	output.close();
}
void expData3(int MinH, int MinS, int MinV, int MaxH, int MaxS, int MaxV, int Er, int Dl, int dm) {
	ofstream output(file);
	if (output.is_open()) {
		cout << "Alhamdulillah simpan data Obstacle" << endl;
		output << MinH << ";" << MinS << ";" << MinV << ";" << endl;
		output << MaxH << ";" << MaxS << ";" << MaxV << ";" << endl;
		output << Er << ";" << Dl << ";" << endl;
		output << dm << ";" << endl;
	}
	else cout << "Gak isok bukak!" << endl;
	output.close();
}
void impData() {
	ifstream buka(file);
	string line;
	int j = 0, buf = 0, data = 0;

	if (buka.is_open()) {
		while (buka.good()) {
			char buffer[5] = {};
			getline(buka, line);
			for (int i = 0; i < line.length(); i++) {
				buffer[j] = line[i];
				if (line[i] == ';') {
					j = 0;
					buf = atoi(buffer);
					if (data < 3) {
						wMin[data] = buf; printf("wMin : %d\n", wMin[data]);
					}
					else if (data < 6) {
						wMax[data % 3] = buf; printf("wMax : %d\n", wMax[data % 3]);
					}
					else if (data < 9) {
						ED[data % 6] = buf; printf("ED : %d\n", ED[data % 6]);
					}
					data++;
					continue;
				}
				j++;
			}
		}
		buka.close();
	}
}
void impData2() {
	ifstream buka(file);
	string line;
	int j = 0, buf = 0, data = 0;

	if (buka.is_open()) {
		while (buka.good()) {
			char buffer[5] = {};
			getline(buka, line);
			for (int i = 0; i < line.length(); i++) {
				buffer[j] = line[i];
				if (line[i] == ';') {
					j = 0;
					buf = atoi(buffer);
					if (data < 5) {
						roii[data] = buf;
						printf("roi %d : %d\n", data, roii[data]);
					}
					data++;
					continue;
				}
				j++;
			}
		}
		buka.close();
	}
}
void impData3() {
	ifstream buka(file);
	string line;
	int j = 0, buf = 0, data = 0;

	if (buka.is_open()) {
		while (buka.good()) {
			char buffer[5] = {};
			getline(buka, line);
			for (int i = 0; i < line.length(); i++) {
				buffer[j] = line[i];
				if (line[i] == ';') {
					j = 0;
					buf = atoi(buffer);
					if (data < 3) {
						wMin[data] = buf; printf("wMin : %d\n", wMin[data]);
					}
					else if (data < 6) {
						wMax[data % 3] = buf; printf("wMax : %d\n", wMax[data % 3]);
					}
					else if (data < 8) {
						ED[data % 6] = buf; printf("ED : %d\n", ED[data % 6]);
					}
					else if (data == 8) {
						diameter = buf; printf("diameter : %d\n", diameter);
					}data++;
					continue;
				}
				j++;
			}
		}
		buka.close();
	}
}
void recordHSV_Values(Mat img, Mat hsv_frame) {
	if (mouseMove == false && rectangleSelected == true) {
		if (H_ROI.size() > 0)	H_ROI.clear();
		if (S_ROI.size() > 0)	S_ROI.clear();
		if (V_ROI.size() > 0)	V_ROI.clear();
		if (rectangleROI.width > 1 || rectangleROI.height > 1) {
			for (int i = rectangleROI.x; i < rectangleROI.x + rectangleROI.width; i++) {
				for (int j = rectangleROI.y; j < rectangleROI.y + rectangleROI.height; j++) {
					H_ROI.push_back((int)hsv_frame.at<Vec3b>(j, i)[0]);
					S_ROI.push_back((int)hsv_frame.at<Vec3b>(j, i)[1]);
					V_ROI.push_back((int)hsv_frame.at<Vec3b>(j, i)[2]);
				}
			}
		}
		if (H_ROI.size() > 0) {
			wMin[H] = *min_element(H_ROI.begin(), H_ROI.end());
			wMax[H] = *max_element(H_ROI.begin(), H_ROI.end());
		}
		if (S_ROI.size() > 0) {
			wMin[S] = *min_element(S_ROI.begin(), S_ROI.end());
			wMax[S] = *max_element(S_ROI.begin(), S_ROI.end());
		}
		if (V_ROI.size() > 0) {
			wMin[V] = *min_element(V_ROI.begin(), V_ROI.end());
			wMax[V] = *max_element(V_ROI.begin(), V_ROI.end());
		}
		if (file[ul] == '6')
			trackBar_updateobs();
		else if (file[ul] == '7')
			trackBar_updateRoi();
		else
			trackBar_update();
		rectangleSelected = false;
	}

	if (mouseMove == true)
		rectangle(img, initialClickPoint, Point(currentMousePoint.x, currentMousePoint.y), Scalar(0, 0, 255), 1, 8, 0);
}

//===========Trackbar=============
void trackBar() {
	cvCreateTrackbar("H/Y MIN", "Result", &wMin[H], 255, 0);
	cvCreateTrackbar("S/U MIN", "Result", &wMin[S], 255, 0);
	cvCreateTrackbar("V MIN", "Result", &wMin[V], 255, 0);
	cvCreateTrackbar("H/Y MAX", "Result", &wMax[H], 255, 0);
	cvCreateTrackbar("S/U MAX", "Result", &wMax[S], 255, 0);
	cvCreateTrackbar("V MAX", "Result", &wMax[V], 255, 0);
	cvCreateTrackbar("E", "Result", &ED[E], 100, 0);
	cvCreateTrackbar("D", "Result", &ED[D], 100, 0);
	cvCreateTrackbar("MC", "Result", &ED[C], 100, 0);
}
void trackBarobs() {
	cvCreateTrackbar("H/Y MIN", "ResultOBS", &wMin[H], 255, 0);
	cvCreateTrackbar("S/U MIN", "ResultOBS", &wMin[S], 255, 0);
	cvCreateTrackbar("V MIN", "ResultOBS", &wMin[V], 255, 0);
	cvCreateTrackbar("H/Y MAX", "ResultOBS", &wMax[H], 255, 0);
	cvCreateTrackbar("S/U MAX", "ResultOBS", &wMax[S], 255, 0);
	cvCreateTrackbar("V MAX", "ResultOBS", &wMax[V], 255, 0);
	cvCreateTrackbar("E", "ResultOBS", &ED[E], 100, 0);
	cvCreateTrackbar("D", "ResultOBS", &ED[D], 100, 0);
	cvCreateTrackbar("Diameter", "ResultOBS", &diameter, 200, 0);
}
void trackBarRoi() {
	cvCreateTrackbar("set roi x", "ResultROI", &roii[0], 800, 0);
	cvCreateTrackbar("set roi y", "ResultROI", &roii[1], 600, 0);
	cvCreateTrackbar("set lebar x", "ResultROI", &roii[2], 800, 0);
	cvCreateTrackbar("set tinggi y", "ResultROI", &roii[3], 600, 0);
	cvCreateTrackbar("warpAffine", "ResultROI", &roii[4], 360, 0);
}
void trackBar_update() {
	cvSetTrackbarPos("H/Y MIN", "Result", wMin[H]);
	cvSetTrackbarPos("S/U MIN", "Result", wMin[S]);
	cvSetTrackbarPos("V MIN", "Result", wMin[V]);
	cvSetTrackbarPos("H/Y MAX", "Result", wMax[H]);
	cvSetTrackbarPos("S/U MAX", "Result", wMax[S]);
	cvSetTrackbarPos("V MAX", "Result", wMax[V]);
	cvSetTrackbarPos("E", "Result", ED[E]);
	cvSetTrackbarPos("D", "Result", ED[D]);
	cvSetTrackbarPos("MC", "Result", ED[C]);
}
void trackBar_updateobs() {
	cvSetTrackbarPos("H/Y MIN", "ResultOBS", wMin[H]);
	cvSetTrackbarPos("S/U MIN", "ResultOBS", wMin[S]);
	cvSetTrackbarPos("V MIN", "ResultOBS", wMin[V]);
	cvSetTrackbarPos("H/Y MAX", "ResultOBS", wMax[H]);
	cvSetTrackbarPos("S/U MAX", "ResultOBS", wMax[S]);
	cvSetTrackbarPos("V MAX", "ResultOBS", wMax[V]);
	cvSetTrackbarPos("E", "ResultOBS", ED[E]);
	cvSetTrackbarPos("D", "ResultOBS", ED[D]);
	cvSetTrackbarPos("Diameter", "ResultOBS", diameter);
}
void trackBar_updateRoi() {
	cvSetTrackbarPos("set roi x", "ResultROI", roii[0]);
	cvSetTrackbarPos("set roi y", "ResultROI", roii[1]);
	cvSetTrackbarPos("set lebar x", "ResultROI", roii[2]);
	cvSetTrackbarPos("set tinggi y", "ResultROI", roii[3]);
	cvSetTrackbarPos("warpAffine", "ResultROI", roii[4]);
}

//===========Vision Function=============
Mat GetThresImage(Mat imgFrame, Mat img) {
	Mat imHSV, thres;
	if (colorMode == 1)
	{
		cvtColor(img, imHSV, COLOR_BGR2HSV);
	}
	else if (colorMode == 2)
	{
		cvtColor(img, imHSV, COLOR_BGR2YUV);
	}
	recordHSV_Values(imgFrame, imHSV);
	inRange(imHSV, Scalar(wMin[H], wMin[S], wMin[V]), Scalar(wMax[H], wMax[S], wMax[V]), thres);
	//imshow("HSV", imHSV);
	return thres;
}
Mat Erosion(Mat img) {
	erosion_src = img;
	int erosion_type;
	if (mode == RECT) erosion_type = MORPH_RECT;
	else if (mode == CROSS) erosion_type = MORPH_CROSS;
	else if (mode == ELIPSE) erosion_type = MORPH_ELLIPSE;

	Mat element = getStructuringElement(erosion_type, Size(2 * ED[E] + 1, 2 * ED[E] + 1), Point(ED[E], ED[E]));
	erode(erosion_src, erosion_dst, element);
	return erosion_dst;
}
Mat Dilation(Mat img) {
	dilation_src = img;

	int dilation_type;
	if (mode == RECT) dilation_type = MORPH_RECT;
	else if (mode == CROSS) dilation_type = MORPH_CROSS;
	else if (mode == ELIPSE) dilation_type = MORPH_ELLIPSE;

	Mat element = getStructuringElement(dilation_type,Size(2 * ED[D] + 1, 2 * ED[D] + 1), Point(ED[D], ED[D]));

	dilate(dilation_src, dilation_dst, element);
	morphologyEx(dilation_dst, dilation_dst, MORPH_CLOSE, element, Point(-1, -1), ED[C]);

	return dilation_dst;
}

//===========Welcoming Message=============
static void help() {
	cout <<"\t Intruksi Kalibrasi:\n"
		"\t 1    - Tutup Botol Merah\n"
		"\t 2    - Tutup Botol Biru\n"
		"\t 3    - Tutup Botol Hijau\n"
		"\t 4    - ROI FRAME\n"
		"\t r    - Reset\n"
		"\t q    - Exit\n"
		"\t s    - Saved 						" << endl;
}

//===========Main Program=============
int main(int argc, char** argv)
{
	VideoCapture capture(1);
	mouseIsDragging = mouseMove = rectangleSelected = false;
	cvNamedWindow("Result");
	cvNamedWindow("ResultROI");
	cvNamedWindow("ResultOBS");
	help();

	capture.set(CV_CAP_PROP_FRAME_WIDTH, 512);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 512);
	if (!capture.isOpened()) {
		fprintf(stderr, "Frame null!\n");
		fprintf(stderr, "Cek kabel kamera!\n");
		exit(1);
	}
	Mat frame;
	Mat frame_roi, frame2;
	Mat pre_frame;
	namedWindow("Frame", 1);
	setMouseCallback("Frame", clickAndDrag_Rectangle, &frame);
	int counter = 0;
	int counterr = 0;
	Rect roi;
	while (1)
	{
		//capture >> pre_frame;
		pre_frame = imread("Sapiens.jpg", CV_LOAD_IMAGE_COLOR);
		flip(pre_frame, pre_frame, 1);
		if (counterr == 1)
		{
			imwrite("gambar.jpg", pre_frame);
			counterr = 0;
		}

		roi.x = roii[0];
		roi.y = roii[1];
		roi.width = roii[2];
		roi.height = roii[3];
		int test1 = roii[2] / 2;
		int test2 = roii[3] / 2;
		pre_frame(roi).copyTo(frame);
		pre_frame(roi).copyTo(frame_roi);
		frame_roi.copyTo(frame2);
		circle(frame, cvPoint(365, 408), diameter, cv::Scalar(0, 0, 0), -1, 4, 0);
		Mat procRotation = getRotationMatrix2D(Point(roi.width / 2, roi.height / 2), roii[4], 1);
		warpAffine(frame2, frame2, procRotation, frame2.size());
		line(frame2, cvPoint(0, roi.height / 2), cvPoint(roi.width, roi.height / 2), Scalar(255, 0, 255), 2, 8, 0);
		line(frame2, cvPoint(roi.width / 2, 0), cvPoint(roi.width / 2, roi.height), Scalar(255, 0, 255), 2, 8, 0);
		circle(frame2, cvPoint(test1, test2), diameter, cv::Scalar(0, 255, 0));
		Mat thresh = GetThresImage(frame, frame);
		Mat erosion = Erosion(thresh);
		Mat dilation = Dilation(erosion);
		imshow("Threshold", dilation);
		imshow("Frame", frame);
		if (counter == 0)
		{
			file[ul] = '4';
			cvNamedWindow("Result");
			trackBar();
			name = "Inisial Frame"; cout << name << endl;
			impData();
			trackBar_update();
			mode = ELIPSE;
		}
		else if (counter > 0)
			counter = 1;

		char key = cvWaitKey(33);
		switch (key)
		{
		case 'q': 	fprintf(stderr, "Alhamdulillah!\n");
			exit(1);
			break;
		case '1':	file[ul] = '0';
			cvDestroyWindow("ResultROI");
			cvDestroyWindow("ResultOBS");
			cvNamedWindow("Result");
			trackBar();
			name = "Kalibrasi tutup merah"; cout << name << endl;
			impData();
			trackBar_update();
			mode = ELIPSE;
			break;
		case '2':	file[ul] = '4';
			cvDestroyWindow("ResultROI");
			cvDestroyWindow("ResultOBS");
			cvNamedWindow("Result");
			trackBar();
			name = "Kalibrasi tutup biru"; cout << name << endl;
			impData();
			trackBar_update();
			mode = ELIPSE;
			break;
		case '3':	file[ul] = '6';
			cvDestroyWindow("ResultROI");
			cvDestroyWindow("Result");
			cvNamedWindow("ResultOBS");
			trackBarobs();
			name = "kalibrasi tutup hijau"; cout << name << endl;
			impData3();
			trackBar_updateobs();
			mode = ELIPSE;
			break;

		case '4':   file[ul] = '7';
			cvDestroyWindow("Result");
			cvDestroyWindow("ResultOBS");
			cvNamedWindow("ResultROI");
			trackBarRoi();
			name = "Setting ROI"; cout << name << endl;
			impData2();
			trackBar_updateRoi();
			break;
		case 's':	cout << "Simpan data kalibrasi " << name << endl;
			if (file[ul] == '7')
				expData2(roii[0], roii[1], roii[2], roii[3], roii[4]);
			else if (file[ul] == '6')
				expData3(wMin[H], wMin[S], wMin[V], wMax[H], wMax[S], wMax[V], ED[E], ED[D], diameter);
			else
				expData(wMin[H], wMin[S], wMin[V], wMax[H], wMax[S], wMax[V], ED[E], ED[D], ED[C]);
			break;
		case 'y':	cout << "Mode YUV" << name << endl;
			colorMode = 2;
			impData();
			break;
		case 'h':	cout << "Mode HSV" << name << endl;
			colorMode = 1;
			impData();
			break;
		case 'r':	cout << "Reset data kalibrasi " << name << endl;
			wMin[H] = wMin[S] = wMin[V] = ED[E] = ED[D] = ED[C] = 0;
			wMax[H] = wMax[S] = wMax[V] = 255;
			trackBar_update();
			break;
		case 'c': 	cout << "simpan gambar tengah" << endl;
			counterr = 1;
			break;

		}

		counter = 1;
		thresh.release();
		erosion.release();
		dilation.release();
	}
	capture.release();
	frame.release();
	cvDestroyAllWindows();
}
