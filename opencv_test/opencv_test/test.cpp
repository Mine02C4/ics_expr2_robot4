#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "videotest.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	int mode;
	cout << "Select mode" << endl;
	cout << "1: Feature detection test" << endl;
	cout << "2: Calibration" << endl;
	cout << "3: Object detection by color" << endl;
	cin >> mode;
	switch (mode)
	{
	case 1:
	{
		VideoTest* vt = new VideoTest();
		vt->Init();

		while (1) {
			vt->ReadFrame();
			vt->TrackFeatures();
			int key = waitKey(1);
			if (key >= 0) {
				break;
			}
		}
		delete vt;
		break;
	}
	case 2:
	{
		const int BOARD_W = 10; //‰¡
		const int BOARD_H = 7;
		const Size BOARD_SIZE = Size(BOARD_W, BOARD_H);
		const int N_CORNERS = BOARD_H * BOARD_W;
		const int N_BOARDS = 10;
		const float SCALE = 23;
		const Size IM_SIZE = Size(1280, 720);

		vector<Mat> src_image(N_BOARDS);
		for (int i = 0; i < N_BOARDS; i++) {
			string filename = "test_" + to_string(i) + ".jpg";
			cout << filename << endl;
			resize(imread(filename), src_image[i], Size(), 0.25, 0.25);
		}

		//cout << src_image[1];
		vector<vector<Point2f>>imagePoints;

		Mat gray_image;
		bool found;
		for (int i = 0; i < N_BOARDS; i++) {
			vector<Point2f> imageCorners;
			Mat dst_image;
			found = findChessboardCorners(src_image[i], BOARD_SIZE, imageCorners);

			cvtColor(src_image[i], gray_image, CV_BGR2GRAY);
			cornerSubPix(gray_image, imageCorners, Size(9, 9), Size(-1, -1), TermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 30, 0.1));

			dst_image = src_image[i].clone();
			drawChessboardCorners(dst_image, BOARD_SIZE, imageCorners, found);
			namedWindow("test" + to_string(i));
			imshow("test" + to_string(i), src_image[i]);
			for (int i = 0; i < 10; i++) {
				cout << i << " " << (int)(imageCorners[i].x + 0.5) << " " << (int)(imageCorners[i].y + 0.5) << endl;
			}
			imagePoints.push_back(imageCorners);
		}
		vector<vector<Point3f>> objectPoints;
		vector<Point3f> objectCorners;
		for (int j = 0; j < BOARD_H; j++) {
			for (int i = 0; i < BOARD_W; i++) {
				objectCorners.push_back(Point3f(i * SCALE, j * SCALE, 0.0f));
			}
		}
		for (int i = 0; i < N_BOARDS; i++) {
			objectPoints.push_back(objectCorners);
		}

		Mat cameraMatrix;
		Mat distCoeffs;
		vector<Mat> rvecs;
		vector<Mat> tvecs;
		double rms = calibrateCamera(objectPoints, imagePoints, src_image[0].size(), cameraMatrix, distCoeffs, rvecs, tvecs);

		cout << fixed << right;
		cout << "Re-projection Error(unit: pixel)" << endl;
		cout << " " << rms << endl;
		cout << endl;
		cout << "cameraMatrix(unit: pixel)" << endl;
		cout << " fx = " << cameraMatrix.at<double>(0, 0);
		cout << " fy= " << cameraMatrix.at<double>(1, 1);
		cout << " cx= " << cameraMatrix.at<double>(0, 2);
		cout << " cy= " << cameraMatrix.at<double>(1, 2);
		cout << endl << endl;
		cout << "distCorffs " << endl;
		cout << " k1=" << distCoeffs.at<double>(0, 0);
		cout << " k2 = " << distCoeffs.at<double>(0, 1);
		cout << " p1 = " << distCoeffs.at<double>(0, 2);
		cout << " p1 = " << distCoeffs.at<double>(0, 3);
		cout << " k3 = " << distCoeffs.at<double>(0, 4);
		cout << endl << endl;

		FileStorage fs("calibration.xml", FileStorage::WRITE);
		fs << "cameraMatrix" << cameraMatrix;
		fs << "distCoeffs" << distCoeffs;
		fs.release();

		break;
	}
	case 3:
	{
		VideoTest vt;
		vt.Init();
		while (1) {
			vt.ReadFrame();
			vt.DetectionByColor();
			int key = waitKey(1);
			if (key >= 0) {
				break;
			}
		}
		break;
	}
	default:
		break;
	}

	return 0;
}
