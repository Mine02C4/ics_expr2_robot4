#include <iostream>
#include <vector>
#include <iterator>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "videotest.h"
#include "calibration.hpp"
#include "stereo.hpp"

#if defined(__linux__) || defined(__APPLE__)

#define kOPENCV_KEY_ENTER 10

#elif _MSC_VER

#define kOPENCV_KEY_ENTER 13

#endif

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
  int mode;
  cout << "Select mode" << endl;
  cout << "1: Feature detection test" << endl;
  cout << "2: Camera calibration(single)" << endl;
  cout << "3: Stereo camera calibration" << endl;
  cout << "4: Object detection by color" << endl;
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
      int key = waitKey(1) & 0xff;
      if (key != 27) {
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
          bool found = Calibration::DetectChessboardCorners(src_image[i], BOARD_SIZE, imageCorners);
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
          int key = waitKey(10) & 0xff;
          switch (key)
          {
          case kOPENCV_KEY_ENTER: // Enter
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
          bool found = Calibration::DetectChessboardCorners(src_image[i], BOARD_SIZE, imageCorners);
          cboard_preview = src_image[i].clone();
          drawChessboardCorners(cboard_preview, BOARD_SIZE, imageCorners, found);
          imshow("chessboard", cboard_preview);
          int key = waitKey(0) & 0xff;
          string filename = "cboard_" + to_string(i) + ".png";
          switch (key)
          {
          case kOPENCV_KEY_ENTER: // Enter
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
      int key = waitKey(0) & 0xff;
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
    const int BOARD_W = 10;
    const int BOARD_H = 7;
    const Size BOARD_SIZE = Size(BOARD_W, BOARD_H);
    const int N_BOARDS = 10;
    const float SCALE = 23;

    vector<string> calibfiles = {
      "calibration_r.xml",
      "calibration_l.xml"
    };

    vector<Mat> src_image(N_BOARDS * 2);
    bool files_exist = true;
    for (int i = 0; i < N_BOARDS; i++) {
      src_image[i * 2] = imread("stereo_cboard_" + to_string(i) + "_r.png");
      if (src_image[i * 2].data == NULL) {
        files_exist = false;
        for (i = i * 2 - 1; i >= 0; --i) {
          src_image[i].release();
        }
        break;
      }
      src_image[i * 2 + 1] = imread("stereo_cboard_" + to_string(i) + "_l.png");
      if (src_image[i * 2 + 1].data == NULL) {
        files_exist = false;
        for (i = i * 2; i >= 0; --i) {
          src_image[i].release();
        }
        break;
      }
    }

    vector<vector<Point2f> > imagePoints[2];
    vector<vector<Point3f> > objectPoints;
    vector<Point3f> objectCorners;
    for (int j = 0; j < BOARD_H; j++) {
      for (int i = 0; i < BOARD_W; i++) {
        objectCorners.push_back(Point3f(i * SCALE, j * SCALE, 0.0f));
      }
    }
    Mat R, T, E, F;

    if (files_exist) {
      int input;
      cout << "Stereo chessboard images are already exist." << endl;
      cout << "1: Use the images" << endl;
      cout << "2: Take new images" << endl;
      cin >> input;
      switch (input)
      {
      case 1:
      {
        for (int i = 0; i < N_BOARDS * 2; i++) {
          vector<Point2f> imageCorners;
          Mat dst_image, gray_image;
          bool found = Calibration::DetectChessboardCorners(src_image[i], BOARD_SIZE, imageCorners);
          assert(found);
          imagePoints[i % 2].push_back(imageCorners);
        }
        break;
      }
      case 2:
        for (int i = 0; i < N_BOARDS * 2; ++i) {
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
      Mat frame0, frame1;
      VideoCapture cap[2];
      int right_id, left_id;
      cout << "Input right camera ID: ";
      cin >> right_id;
      cout << "Input left camera ID: ";
      cin >> left_id;
      cap[0].open(right_id);
      cap[1].open(left_id);
      if (!cap[0].isOpened() || !cap[1].isOpened()) {
        cerr << "Cannot open camera." << endl;
        exit(1);
      }
      for (int i = 0; i < N_BOARDS; i++) {
        bool capture_phase = true;
        while (capture_phase) {
          cap[0] >> frame0;
          cap[1] >> frame1;
          imshow("chessboard camera Right", frame0);
          imshow("chessboard camera Left", frame1);
          int key = waitKey(10) & 0xff;
          switch (key)
          {
          case kOPENCV_KEY_ENTER: // Enter
            src_image[i * 2] = frame0.clone();
            src_image[i * 2 + 1] = frame1.clone();
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
          vector<Point2f> imageCorners[2];
          Mat cboard_preview[2];
          bool found[2];
          try
          {
            found[0] = Calibration::DetectChessboardCorners(src_image[i * 2], BOARD_SIZE, imageCorners[0]);
            found[1] = Calibration::DetectChessboardCorners(src_image[i * 2 + 1], BOARD_SIZE, imageCorners[1]);
          }
          catch (const std::exception& ex)
          {
            cerr << ex.what() << endl;
            --i;
            continue;
          }
          cboard_preview[0] = src_image[i * 2].clone();
          cboard_preview[1] = src_image[i * 2 + 1].clone();
          drawChessboardCorners(cboard_preview[0], BOARD_SIZE, imageCorners[0], found);
          drawChessboardCorners(cboard_preview[1], BOARD_SIZE, imageCorners[1], found);
          imshow("chessboard camera Right", cboard_preview[0]);
          imshow("chessboard camera Left", cboard_preview[1]);
          int key = waitKey(0) & 0xff;
          switch (key)
          {
          case kOPENCV_KEY_ENTER: // Enter
            if (found[0] && found[1]) {
              imwrite("stereo_cboard_" + to_string(i) + "_r.png", src_image[i * 2]);
              imwrite("stereo_cboard_" + to_string(i) + "_l.png", src_image[i * 2 + 1]);
              imagePoints[0].push_back(imageCorners[0]);
              imagePoints[1].push_back(imageCorners[1]);
            }
            else {
              --i;
            }
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

    Mat cameraMatrix[2], distCoeffs[2];
    Mat R1, R2, P1, P2, Q;
    Rect validRoi[2];
    bool calibrated = false;
    if (files_exist) {
      FileStorage fsi("intrinsics.xml", FileStorage::READ);
      FileStorage fse("extrinsics.xml", FileStorage::READ);
      if (fsi.isOpened() && fse.isOpened()) {
        int input;
        cout << "Calibration data are already exist." << endl;
        cout << "1: Use the calibration data" << endl;
        cout << "2: Calibration again" << endl;
        cin >> input;
        switch (input)
        {
        case 1:
        {
          fsi["M1"] >> cameraMatrix[0];
          fsi["M2"] >> cameraMatrix[1];
          fsi["D1"] >> distCoeffs[0];
          fsi["D2"] >> distCoeffs[1];
          fse["R"] >> R;
          fse["T"] >> T;
          fse["R1"] >> R1;
          fse["R2"] >> R2;
          fse["P1"] >> P1;
          fse["P2"] >> P2;
          fse["Q"] >> Q;
          fse["vroi0"] >> validRoi[0];
          fse["vroi1"] >> validRoi[1];
          calibrated = true;
          break;
        }
        case 2:
          break;
        default:
          cerr << "Invalid input" << endl;
          exit(1);
        }
        fsi.release();
        fse.release();
      }
    }
    Size imageSize = src_image[0].size();
    if (!calibrated) {
      FileStorage fs0(calibfiles[0], FileStorage::READ);
      FileStorage fs1(calibfiles[1], FileStorage::READ);
      if (!fs0.isOpened() || !fs1.isOpened()) {
        cerr << "Cannot open calibration XML" << endl;
        exit(1);
      }
      fs0["cameraMatrix"] >> cameraMatrix[0];
      fs1["cameraMatrix"] >> cameraMatrix[1];
      fs0["distCoeffs"] >> distCoeffs[0];
      fs1["distCoeffs"] >> distCoeffs[1];

      for (int i = 0; i < N_BOARDS; i++) {
        objectPoints.push_back(objectCorners);
      }
      double rms = stereoCalibrate(objectPoints, imagePoints[0], imagePoints[1],
        cameraMatrix[0], distCoeffs[0],
        cameraMatrix[1], distCoeffs[1],
        imageSize, R, T, E, F,
        CALIB_FIX_ASPECT_RATIO +
        CALIB_ZERO_TANGENT_DIST +
        CALIB_FIX_INTRINSIC +
        CALIB_SAME_FOCAL_LENGTH +
        CALIB_RATIONAL_MODEL +
        CALIB_FIX_K3 + CALIB_FIX_K4 + CALIB_FIX_K5,
        TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 100, 1e-5));
      cout << "done with RMS error=" << rms << endl;

      // Error check
      double err = 0;
      int npoints = 0;
      vector<Vec3f> lines[2];
      for (int i = 0; i < N_BOARDS; i++)
      {
        int npt = (int)imagePoints[0][i].size();
        Mat imgpt[2];
        for (int k = 0; k < 2; k++)
        {
          imgpt[k] = Mat(imagePoints[k][i]);
          undistortPoints(imgpt[k], imgpt[k], cameraMatrix[k], distCoeffs[k], Mat(), cameraMatrix[k]);
          computeCorrespondEpilines(imgpt[k], k + 1, F, lines[k]);
        }
        for (int j = 0; j < npt; j++)
        {
          double errij = fabs(imagePoints[0][i][j].x*lines[1][j][0] +
            imagePoints[0][i][j].y*lines[1][j][1] + lines[1][j][2]) +
            fabs(imagePoints[1][i][j].x*lines[0][j][0] +
              imagePoints[1][i][j].y*lines[0][j][1] + lines[0][j][2]);
          err += errij;
        }
        npoints += npt;
      }
      cout << "average epipolar err = " << err / npoints << endl;

      // save intrinsic parameters
      FileStorage fs("intrinsics.xml", FileStorage::WRITE);
      if (fs.isOpened())
      {
        fs << "M1" << cameraMatrix[0] << "D1" << distCoeffs[0] <<
          "M2" << cameraMatrix[1] << "D2" << distCoeffs[1];
        fs.release();
      }
      else
      {
        cerr << "Error: can not save the intrinsic parameters" << endl;
      }


      stereoRectify(cameraMatrix[0], distCoeffs[0],
        cameraMatrix[1], distCoeffs[1],
        imageSize, R, T, R1, R2, P1, P2, Q,
        CALIB_ZERO_DISPARITY, 1, imageSize, &validRoi[0], &validRoi[1]);

      fs.open("extrinsics.xml", FileStorage::WRITE);
      if (fs.isOpened())
      {
        fs << "R" << R << "T" << T << "R1" << R1 << "R2" << R2 << "P1" << P1 << "P2" << P2 << "Q" << Q;
        fs << "vroi0" << validRoi[0];
        fs << "vroi1" << validRoi[1];
        fs.release();
      }
      else {
        cerr << "Error: can not save the extrinsic parameters" << endl;
      }
    }

    // OpenCV can handle left-right
    // or up-down camera arrangements
    bool isVerticalStereo = fabs(P2.at<double>(1, 3)) > fabs(P2.at<double>(0, 3));

    Mat rmap[2][2];
    // IF BY CALIBRATED (BOUGUET'S METHOD)
    bool useCalibrated = false;
    if (useCalibrated)
    {
      // we already computed everything
    }
    // OR ELSE HARTLEY'S METHOD
    else
      // use intrinsic parameters of each camera, but
      // compute the rectification transformation directly
      // from the fundamental matrix
    {
      vector<Point2f> allimgpt[2];
      for (int k = 0; k < 2; k++)
      {
        for (int i = 0; i < N_BOARDS; i++)
          std::copy(imagePoints[k][i].begin(), imagePoints[k][i].end(), back_inserter(allimgpt[k]));
      }
      F = findFundamentalMat(Mat(allimgpt[0]), Mat(allimgpt[1]), FM_8POINT, 0, 0);
      Mat H1, H2;
      stereoRectifyUncalibrated(Mat(allimgpt[0]), Mat(allimgpt[1]), F, imageSize, H1, H2, 3);

      R1 = cameraMatrix[0].inv()*H1*cameraMatrix[0];
      R2 = cameraMatrix[1].inv()*H2*cameraMatrix[1];
      P1 = cameraMatrix[0];
      P2 = cameraMatrix[1];
    }

    //Precompute maps for cv::remap()
    initUndistortRectifyMap(cameraMatrix[0], distCoeffs[0], R1, P1, imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
    initUndistortRectifyMap(cameraMatrix[1], distCoeffs[1], R2, P2, imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);

    Mat canvas;
    double sf;
    int w, h;
    if (!isVerticalStereo)
    {
      sf = 600. / MAX(imageSize.width, imageSize.height);
      w = cvRound(imageSize.width*sf);
      h = cvRound(imageSize.height*sf);
      canvas.create(h, w * 2, CV_8UC3);
    }
    else
    {
      sf = 300. / MAX(imageSize.width, imageSize.height);
      w = cvRound(imageSize.width*sf);
      h = cvRound(imageSize.height*sf);
      canvas.create(h * 2, w, CV_8UC3);
    }
    bool preview_phase = true;
    int preview_index = 0;
    while (preview_phase) {
      for (int k = 0; k < 2; k++)
      {
        Mat img = src_image[preview_index * 2 + k], rimg;
        remap(img, rimg, rmap[k][0], rmap[k][1], INTER_LINEAR);
        Mat canvasPart = !isVerticalStereo ? canvas(Rect(w*k, 0, w, h)) : canvas(Rect(0, h*k, w, h));
        resize(rimg, canvasPart, canvasPart.size(), 0, 0, INTER_AREA);
        if (useCalibrated)
        {
          Rect vroi(cvRound(validRoi[k].x*sf), cvRound(validRoi[k].y*sf),
            cvRound(validRoi[k].width*sf), cvRound(validRoi[k].height*sf));
          rectangle(canvasPart, vroi, Scalar(0, 0, 255), 3, 8);
        }
      }
      if (!isVerticalStereo)
        for (int j = 0; j < canvas.rows; j += 16)
          line(canvas, Point(0, j), Point(canvas.cols, j), Scalar(0, 255, 0), 1, 8);
      else
        for (int j = 0; j < canvas.cols; j += 16)
          line(canvas, Point(j, 0), Point(j, canvas.rows), Scalar(0, 255, 0), 1, 8);
      imshow("rectified", canvas);

      Mat disparity;
      Mat left_gray, right_gray;
      Mat rleft, rright;
      remap(src_image[preview_index * 2], rright, rmap[0][0], rmap[0][1], INTER_LINEAR);
      remap(src_image[preview_index * 2 + 1], rleft, rmap[1][0], rmap[1][1], INTER_LINEAR);
      cvtColor(rright, right_gray, COLOR_BGR2GRAY);
      cvtColor(rleft, left_gray, COLOR_BGR2GRAY);
      int numberOfDisparities = Stereo::CalcDisparity(left_gray, right_gray, disparity);
      Stereo::RenderDisparity(disparity, numberOfDisparities);
      Mat xyz;
      reprojectImageTo3D(disparity, xyz, Q, true);

      int key = waitKey(0) & 0xff;
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
  case 4:
  {
    VideoTest vt;
    vt.Init();
    while (1) {
      vt.ReadFrame();
      vt.DetectionByColor();
      int key = waitKey(1) & 0xff;
      if (key != 27) {
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
