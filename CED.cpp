#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <math.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {

  // load image
  String imgName = argv[1];
  String imgPath = "../testImages/" + imgName;
  Mat src = imread(imgPath, IMREAD_GRAYSCALE);
  imshow("src", src);

  // apply Gaussian filter
  Mat blurred;
  blur(src, blurred, Size(3,3));
  imshow("blurred", blurred);

  // compute image gradient
  Mat xComponent, yComponent;
  Sobel(blurred, xComponent, CV_32F, 1, 0, 3);
  Sobel(blurred, yComponent, CV_32F, 0, 1, 3);

  // convert to polar coordinates
  Mat magnitude, angle;
  cartToPolar(xComponent, yComponent, magnitude, angle, true);

  // normalize values
  normalize(magnitude, magnitude, 0, 1, NORM_MINMAX);

  // apply non-maximum suppression
  int neighbor1X, neighbor1Y, neighbor2X, neighbor2Y;
  float gradientAngle;
  for (int x = 0; x < blurred.rows; x++) {
    for (int y = 0; y < blurred.cols; y++) {
      gradientAngle = angle.at<float>(y, x);
      if (abs(gradientAngle) > 180)
        gradientAngle = abs(gradientAngle-180);
      else
        gradientAngle = abs(gradientAngle);

      if (gradientAngle <= 22.5) {
        neighbor1X = x-1;
        neighbor1Y = y;
        neighbor2X = x+1;
        neighbor2Y = y;
      } else if (22.5 < gradientAngle <= 67.5) {
        neighbor1X = x-1;
        neighbor1Y = y-1;
        neighbor2X = x+1;
        neighbor2Y = y+1;
      } else if (67.5 < gradientAngle <= 112.5) {
        neighbor1X = x;
        neighbor1Y = y-1;
        neighbor2X = x;
        neighbor2Y = y+1;
      } else if (112.5 < gradientAngle <= 157.5) {
        neighbor1X = x-1;
        neighbor1Y = y+1;
        neighbor2X = x+1;
        neighbor2Y = y-1;
      } else if (157.5 < gradientAngle <= 202.5) {
        neighbor1X = x-1;
        neighbor1Y = y;
        neighbor2X = x+1;
        neighbor2Y = y;
      }

      if ((0 <= neighbor1X < blurred.rows) && (0 <= neighbor1Y < blurred.cols)) {
        if (magnitude.at<float>(y, x) < magnitude.at<float>(neighbor1Y, neighbor1X)) {
          magnitude.at<float>(y, x) = 0;
          continue;
        }
      }
      if ((0 <= neighbor2X < blurred.rows) && (0 <= neighbor2Y < blurred.cols)) {
        if (magnitude.at<float>(y, x) < magnitude.at<float>(neighbor2Y, neighbor2X)) {
          magnitude.at<float>(y, x) = 0;
          continue;
        }
      }
    }
  }
  imshow("non-max suppression", magnitude);

  // apply double thresholding
  float magMax = 0.2, magMin = 0.1;
  Mat strong = Mat::zeros(magnitude.rows, magnitude.cols, CV_32F);
  Mat weak = Mat::zeros(magnitude.rows, magnitude.cols, CV_32F);
  Mat suppress = Mat::zeros(magnitude.rows, magnitude.cols, CV_32F);
  float gradientMagnitude;
  for (int x = 0; x < magnitude.cols; x++) {
    for (int y = 0; y < magnitude.rows; y++) {
      gradientMagnitude = magnitude.at<float>(x, y);

      if (gradientMagnitude > magMax)
        strong.at<float>(x, y) = gradientMagnitude;
      else if (gradientMagnitude <= magMax && gradientMagnitude > magMin)
        weak.at<float>(x, y) = gradientMagnitude;
      else
        suppress.at<float>(x, y) = gradientMagnitude;
    }
  }
  imshow("strong", strong);
  imshow("weak", weak);
  imshow("suppress", suppress);

  // apply hysteresis
  for (int x = 0; x < weak.cols; x++) {
    for (int y = 0; y < weak.rows; y++) {
      if (weak.at<float>(x, y) != 0) {
        if ((strong.at<float>(x+1, y))
        || (strong.at<float>(x-1, y))
        || (strong.at<float>(x, y+1))
        || (strong.at<float>(x, y-1))
        || (strong.at<float>(x-1, y-1))
        || (strong.at<float>(x+1, x-1))
        || (strong.at<float>(x-1, y+1))
        || (strong.at<float>(x+1, y-1)))
          strong.at<float>(x, y) = weak.at<float>(x, y);
      }
    }
  }

  imshow("final edge detection", strong);

  waitKey(0);
  return 0;
}
