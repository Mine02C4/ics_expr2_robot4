#include "stereo.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <GL/freeglut.h>

using namespace cv;

int Stereo::CalcDisparity(cv::Mat & left_img, cv::Mat & right_img, cv::Mat & disparity)
{
  auto image_size = left_img.size();
#define CALIB_MODE_SGBM
  int SADWindowSize = 3;
  int minDisparity = 0;
  int numberOfDisparities = 0;
  numberOfDisparities = numberOfDisparities > 0 ? numberOfDisparities : ((image_size.width / 8) + 15) & -16;
  int blockSize = SADWindowSize > 0 ? SADWindowSize : 9;
#ifdef CALIB_MODE_SGBM
  auto sgbm = StereoSGBM::create(
    minDisparity,
    numberOfDisparities,
    SADWindowSize
  );
  sgbm->setPreFilterCap(63);
  sgbm->setBlockSize(blockSize);
  int cn = 1;
  sgbm->setP1(8 * cn * blockSize * blockSize);
  sgbm->setP2(32 * cn * blockSize * blockSize);
  sgbm->setMinDisparity(0);
  sgbm->setNumDisparities(numberOfDisparities);
  sgbm->setUniquenessRatio(10);
  sgbm->setSpeckleWindowSize(100);
  sgbm->setSpeckleRange(32);
  sgbm->setDisp12MaxDiff(1);
  sgbm->setMode(StereoSGBM::MODE_SGBM);
  sgbm->compute(left_img, right_img, disparity);
#else
  auto bm = StereoBM::create();
  bm->setROI1(validRoi[0]);
  bm->setROI2(validRoi[1]);
  bm->setPreFilterCap(31);
  bm->setBlockSize(SADWindowSize > 0 ? SADWindowSize : 9);
  bm->setMinDisparity(0);
  bm->setNumDisparities(numberOfDisparities);
  bm->setTextureThreshold(10);
  bm->setUniquenessRatio(15);
  bm->setSpeckleWindowSize(100);
  bm->setSpeckleRange(32);
  bm->setDisp12MaxDiff(1);
  bm->compute(left_img, right_img, disparity);
#endif
  return numberOfDisparities;
}

void Stereo::RenderDisparity(Mat & disparity, int numberOfDisparities)
{
  Mat disparity8;
  disparity.convertTo(disparity8, CV_8U, 255 / (numberOfDisparities * 16.0));
  imshow("disparity", disparity8);
}

Stereo *ste = NULL;

static int drag_mouse_r = 0;
static int drag_mouse_l = 0;
static int last_mouse_x;
static int last_mouse_y;

static float viewpoint_x = 3.0;
static float viewpoint_y = 40.0;
static float viewpoint_z = 50.0;
static float sight_x = 0.0;
static float sight_y = 0.0;
static float sight_z = 0.0;
static float upper_x = 0.0;
static float upper_y = 0.0;
static float upper_z = 1.0;

static cv::Mat currentPose = (cv::Mat_<float>(3, 4) <<
  1.0f, 0.0f, -3.0f, 3.0f,
  0.0f, 0.0f, -40.0f, 40.0f,
  0.0f, -1.0f, -50.0f, 50.0f);

void render()
{
  if (ste != NULL) {
    ste->RenderGLWindow();
  }
}

void setCurrentCameraPose()
{
  viewpoint_x = currentPose.at<float>(0, 3);
  viewpoint_y = currentPose.at<float>(1, 3);
  viewpoint_z = currentPose.at<float>(2, 3);
  sight_x = viewpoint_x + currentPose.at<float>(0, 2);
  sight_y = viewpoint_y + currentPose.at<float>(1, 2);
  sight_z = viewpoint_z + currentPose.at<float>(2, 2);
  upper_x = -1.0f * currentPose.at<float>(0, 1);
  upper_y = -1.0f * currentPose.at<float>(1, 1);
  upper_z = -1.0f * currentPose.at<float>(2, 1);
}


void Stereo::RenderGLWindow()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPolygonMode(GL_FRONT, GL_FILL);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();


  gluLookAt(viewpoint_x, viewpoint_y, viewpoint_z,
    sight_x, sight_y, sight_z,
    upper_x, upper_y, upper_z);

  glEnable(GL_DEPTH_TEST);

  // axis
  glLineWidth(5.0f);
  glBegin(GL_LINES);
  glColor3f(1.0f, 0.0f, 0.0f);		//x axis(Red)
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(200.0f, 0.0f, 0.0f);
  glColor3f(0.0f, 1.0f, 0.0f);		//y axis(Green)
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 200.0f, 0.0f);
  glColor3f(0.0f, 0.0f, 1.0f);		//z axis(Blue)
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, 200.0f);
  glEnd();

  glColor3f(0.8f, 0.0f, 0.8f);
  glPointSize(2.0f);
  glBegin(GL_POINTS);
  for (int i = 0; i < img3d_.rows; ++i) {
    for (int j = 0; j < img3d_.cols; ++j) {
      Vec3f p = img3d_.at<Vec3f>(i, j);
      if (!isinf(p[0])) {
        glVertex3f(p[0], p[1], p[2] / 10.0);
      }
    }
  }
  glEnd();
  glutSwapBuffers();
}

void Stereo::SetData(cv::Mat & img3d)
{
  img3d_ = img3d.clone();
  render();
}

void mouseWheel(int wheelNumber, int direction, int x, int y)
{
  float dZ = (float)direction * 0.2f;
  currentPose.at<float>(0, 3) += dZ * currentPose.at<float>(0, 2);
  currentPose.at<float>(1, 3) += dZ * currentPose.at<float>(1, 2);
  currentPose.at<float>(2, 3) += dZ * currentPose.at<float>(2, 2);
  setCurrentCameraPose();
  render();
}



void motion(int mx, int my)
{
  if (drag_mouse_l == 0 && drag_mouse_r == 1) {
    float dX = (last_mouse_x - mx) * 0.2f;
    float dY = (last_mouse_y - my) * 0.2f;
    currentPose.at<float>(0, 3) += dX * currentPose.at<float>(0, 0);
    currentPose.at<float>(1, 3) += dX * currentPose.at<float>(1, 0);
    currentPose.at<float>(2, 3) += dX * currentPose.at<float>(2, 0);
    currentPose.at<float>(0, 3) += dY * currentPose.at<float>(0, 1);
    currentPose.at<float>(1, 3) += dY * currentPose.at<float>(1, 1);
    currentPose.at<float>(2, 3) += dY * currentPose.at<float>(2, 1);
    setCurrentCameraPose();
  }
  else if (drag_mouse_l == 1 && drag_mouse_r == 0) {
    float dX = (float)(mx - last_mouse_x) * 0.01f;
    float dY = (float)(last_mouse_y - my) * 0.01f;
    cv::Mat rVecX, rVecY, rMatX, rMatY;
    rVecY = dX * currentPose(cv::Rect(1, 0, 1, 3));		// horizontal move -> y-axis rot
    cv::Rodrigues(rVecY, rMatY);
    rVecX = dY * currentPose(cv::Rect(0, 0, 1, 3));		// vertical move -> x-axis rot
    cv::Rodrigues(rVecX, rMatX);
    currentPose = rMatX * rMatY * currentPose;
    setCurrentCameraPose();
  }

  last_mouse_x = mx;
  last_mouse_y = my;

  render();
}


void mouse(int button, int state, int mx, int my)
{
  if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
    drag_mouse_l = 1;
  else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
    drag_mouse_l = 0;

  if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
    drag_mouse_r = 1;
  else if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_UP))
    drag_mouse_r = 0;

  last_mouse_x = mx;
  last_mouse_y = my;
}

static void resize(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(100.0, (double)w / (double)h, 1.0, 10000.0);
  glMatrixMode(GL_MODELVIEW);
}

Stereo::Stereo()
{
  ste = this;
  int argc = 0;
  glutInit(&argc, NULL);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(640, 480);
  gl_window_ = glutCreateWindow("Reconstruction");
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutMouseWheelFunc(mouseWheel);
  resize(640, 480);
  setCurrentCameraPose();
}

