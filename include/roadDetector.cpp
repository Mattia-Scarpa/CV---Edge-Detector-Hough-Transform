#include <roadDetector.h>


// constructors
  roadDetector::roadDetector(cv::Mat& src) {
    grayImg = src.clone();
  }

  roadDetector::roadDetector(std::string path, bool display) {
    originalImg = imread(path, cv::IMREAD_COLOR);
    cvtColor(originalImg, grayImg, cv::COLOR_BGR2GRAY);

    if (display) {
      showImg(grayImg, "source_image");
    }
  }


  // functions
  bool roadDetector::isEmpty() {
    if (grayImg.empty()) {
      return true;
    }
    else {
      return false;
    }
  }

  void roadDetector::getImage(cv::Mat& dst) {
    dst = originalImg;
  }

  void roadDetector::findEdges(cv::Mat& dst, double tresh1, double tresh2, int apertureSobel, bool l2gradient, bool display) {

    cv::Canny(grayImg, edgesImg, tresh1, tresh2, apertureSobel, l2gradient);
    dst = edgesImg;

    if (display) {
      showImg(dst, "edge_image");
    }
  }

  void roadDetector::findLines(std::vector<cv::Vec2f>& lines, double rho, double theta, int tresh, double srn, double stn, double min_theta, double max_theta) {

    HoughLines(edgesImg, lines, rho, theta, tresh, srn, stn);
    linesPolar = lines;
  }

  void roadDetector::getLines(std::vector<cv::Point>& linesPts, cv::Mat& temp) {

    for(size_t i(0); i < linesPolar.size(); i++) {

        float rho = linesPolar[i][0], theta = linesPolar[i][1];

        cv::Point pt1, pt2;

        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;

        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));

        linesPts.push_back(pt1);
        linesPts.push_back(pt2);
        line(temp, pt1, pt2, cv::Scalar(0,0,255), 2, cv::LINE_AA);
    }
    linesPoints = linesPts;
  }

  void roadDetector::findCircles(std::vector<cv::Vec3f>& circles, int method, double dp, double minDist, double param1, double param2) {

    HoughCircles(grayImg, circles, method, dp, minDist, param1, param2);
    circlesInfo = circles;
  }


  void roadDetector::drawCircles(cv::Mat& temp, cv::Scalar color, int thick, int lineType, int shift) {

    for( size_t i(0); i < circlesInfo.size(); i++ ) {
      cv::Point center(cvRound(circlesInfo[i][0]), cvRound(circlesInfo[i][1]));
      int radius = cvRound(circlesInfo[i][2]);
      // draw the circle outline
      circle(temp, center, radius, color, thick, lineType, shift);
     }
  }


  void roadDetector::showImg(cv::Mat src, std::string name) {
    cv::imshow(name, src);
    std::cout << "Press any key to continue..." << std::endl;
    cv::waitKey(0);
  }
