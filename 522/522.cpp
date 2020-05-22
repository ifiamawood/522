// 522.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<opencv.hpp>

using namespace cv;
using namespace std;

cv::Point vP;

int drawHist(cv::Mat & histMat, float * srcHist, int bin_width, int bin_heght)
{
	histMat.create(bin_heght, 256 * bin_width, CV_8UC3);

	histMat = Scalar(255, 255, 255);

	float maxVal = *std::max_element(srcHist, srcHist + 256);

	for (int i = 0; i < 256; i++) {
		Rect binRect;
		binRect.x = i*bin_width;
		float height_i = (float)bin_heght*srcHist[i] / maxVal;
		binRect.height = (int)height_i;
		binRect.y = bin_heght - binRect.height;
		binRect.width = bin_width;
		rectangle(histMat, binRect, CV_RGB(255, 0, 0), -1);
	}

	return 0;
}

void on_mouse(int EVENT, int x, int y, int flags, void* userdata)
{

	Mat hh;
	hh = *(Mat*)userdata;
	switch (EVENT)
	{
	case EVENT_LBUTTONDOWN:
	{
		vP.x = x;
		vP.y = y;
		drawMarker(hh, vP, Scalar(255, 255, 255));
		//circle(hh, vP, 4, cvScalar(255, 255, 255), -1);
		imshow("mouseCallback", hh);
		return;
	}
	break;
	}

}

int main()
{
	//----------------------读取视频文件--------------------------
	VideoCapture capVideo;
	capVideo.open(0);

	int cnt = 0;
	int bin_width = 3;
	int bin_heght = 50;
	float histgram[256] = { 0 };
    Mat frame;
	Mat grayMat;
    Mat selectMat;
	Mat histMat;

	while (1) {
		capVideo >> frame;

		if (frame.empty()) {
			std::cout << "Unable to read frame!" << std::endl;
			return -1;
		}

		//第一帧选取像素
		if (cnt == 0) {
			
			frame.copyTo(selectMat);
			namedWindow("mouseCallback");
			imshow("mouseCallback", selectMat);
			setMouseCallback("mouseCallback", on_mouse, &selectMat);
			waitKey(0);
			destroyAllWindows();
		}

		cvtColor(frame, grayMat, COLOR_BGR2GRAY);

		int index = grayMat.at<uchar>(vP.y, vP.x);
		histgram[index]++;

		drawHist(histMat, histgram, bin_width, bin_heght);

		drawMarker(frame, vP, Scalar(255, 255, 255));
		imshow("frame", frame);
		imshow("histMat", histMat);

		waitKey(30);
		cnt++;
	}

	return 0;
}



