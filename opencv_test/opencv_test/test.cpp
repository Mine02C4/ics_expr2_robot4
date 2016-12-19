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
    bool files_exist = true;
    for (int i = 0; i < N_BOARDS; i++) {
      string filename = "cboard_" + to_string(i) + ".png";
      cout << filename << endl;
      src_image[i] = imread(filename);
      if (src_image[i].data == NULL) {
        files_exist = false;
        for (--i; i >= 0; --i) {
          src_image[i].release();
        }
        break;
      }
    }

    vector<vector<Point2f>>imagePoints;

    if (files_exist) {
      int input;
      cout << "Chessboard images are already exist." << endl;
      cout << "1: Use the images" << endl;
      cout << "2: Take new images" << endl;
      cin >> input;
      switch (input)
      {
      case 1:
        for (int i = 0; i < N_BOARDS; i++) {
          vector<Point2f> imageCorners;
          Mat dst_image, gray_image;
          bool found = findChessboardCorners(src_image[i], BOARD_SIZE, imageCorners);
          cvtColor(src_image[i], gray_image, CV_BGR2GRAY);
          cornerSubPix(gray_image, imageCorners, Size(9, 9), Size(-1, -1), TermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 30, 0.1));

          dst_image = src_image[i].clone();
          drawChessboardCorners(dst_image, BOARD_SIZE, imageCorners, found);
          for (int i = 0; i < 10; i++) {
            cout << i << " " << (int)(imageCorners[i].x + 0.5) << " " << (int)(imageCorners[i].y + 0.5) << endl;
          }
          imagePoints.push_back(imageCorners);
        }
        break;
      case 2:
        for (int i = 0; i < N_BOARDS; ++i) {
          src_image[i].release();
        }
        files_exist = false;
        break;
      default:
        cerr << "Invalid input" << endl;
        exit(1);
      }
    }

    if (!files_exist) {
      int camera_id;
      cout << "Input camera id (start from 0): ";
      cin >> camera_id;
      Mat frame;
      VideoCapture cap(camera_id);
      if (!cap.isOpened()) {
        cerr << "Cannot open camera: " << camera_id << endl;
        exit(1);
      }
      for (int i = 0; i < N_BOARDS; i++) {
        bool capture_phase = true;
        while (capture_phase) {
          cap >> frame;
          imshow("chessboard", frame);
          int key = waitKey(10);
          switch (key)
          {
          case 13: // Enter
            src_image[i] = frame.clone();
            capture_phase = false;
            break;
          case 27: // Esc
            cout << "Canceled" << endl;
            return 0;
          default:
            break;
          }
        }
        {
          vector<Point2f> imageCorners;
          Mat cboard_preview, gray_image;
          bool found = findChessboardCorners(src_image[i], BOARD_SIZE, imageCorners);
          cvtColor(src_image[i], gray_image, CV_BGR2GRAY);
          cornerSubPix(gray_image, imageCorners, Size(9, 9), Size(-1, -1), TermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 30, 0.1));

          cboard_preview = src_image[i].clone();
          drawChessboardCorners(cboard_preview, BOARD_SIZE, imageCorners, found);
          imshow("chessboard", cboard_preview);
          int key = waitKey(0);
          string filename = "cboard_" + to_string(i) + ".png";
          switch (key)
          {
          case 13: // Enter
            imwrite(filename, src_image[i]);
            imagePoints.push_back(imageCorners);
            break;
          case 32: // Space
            --i;
            break;
          case 27: // Esc
            cout << "Canceled" << endl;
            return 0;
          default:
            break;
          }
        }
      }
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
    cout << " k1 = " << distCoeffs.at<double>(0, 0);
    cout << " k2 = " << distCoeffs.at<double>(0, 1);
    cout << " p1 = " << distCoeffs.at<double>(0, 2);
    cout << " p1 = " << distCoeffs.at<double>(0, 3);
    cout << " k3 = " << distCoeffs.at<double>(0, 4);
    cout << endl << endl;

    FileStorage fs("calibration.xml", FileStorage::WRITE);
    fs << "cameraMatrix" << cameraMatrix;
    fs << "distCoeffs" << distCoeffs;
    fs.release();

    bool preview_phase = true;
    int preview_index = 0;
    while (preview_phase) {
      Mat preview_image;
      undistort(src_image[preview_index], preview_image, cameraMatrix, distCoeffs);
      imshow("chessboard", preview_image);
      int key = waitKey(0);
      switch (key)
      {
      case 27: // Esc
        preview_phase = false;
        break;
      case 'z':
        if (preview_index > 0) {
          --preview_index;
        }
        break;
      case 'x':
        if (preview_index < N_BOARDS - 1) {
          ++preview_index;
        }
        break;
      default:
        break;
      }
    }
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
