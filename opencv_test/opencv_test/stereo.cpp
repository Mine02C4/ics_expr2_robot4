#include "stereo.hpp"

#include <opencv2/calib3d/calib3d.hpp>

int Stereo::CalcDisparity(cv::Mat & left_img, cv::Mat & right_img, cv::Mat & disparity)
{
  using namespace cv;
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

void Stereo::RenderDisparity(cv::Mat & left_img, cv::Mat & right_img, cv::Mat & disparity, int numberOfDisparities)
{
}
