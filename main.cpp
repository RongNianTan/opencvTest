#include <stdio.h>
#include <iostream>
#include <fstream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>

int g_slider_position = 0;
int g_run = 1, g_dontset = 0;
cv::VideoCapture g_cap;
void onTrackbarSlide(int pos, void*);
int GaussianChange(const cv::String imagePath);
int pyrDownChange(const cv::String imagePath);
int readCamera();
int readImage(const cv::String imgPath);
const cv::String imagePath = "E:/GitRepository/opencv-3.4/samples/data/lena.jpg";
const cv::String videoPath = "E:/GitRepository/opencv-3.4/samples/data/vtest.avi";
const cv::String Path = "E:/workplace/opencv/opencv-3.4/samples/data/test.MPEG";
int main() {
   // pyrDownChange(imagePath);
    readImage(imagePath);
}

int pyrDownChange(const cv::String imagePath) {
    cv::Mat in, out;
    cv::namedWindow("Example1", 1);
    cv::namedWindow("Example2", 1);

    cv::VideoCapture cap;
    cap.open(videoPath);

    double fps = cap.get(cv::CAP_PROP_FPS);

    cv::Size size((int)cap.get(cv::CAP_PROP_FRAME_WIDTH), (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    cv::VideoWriter writer;
    writer.open(Path, cv::VideoWriter::fourcc('M', 'J','P','G'),fps,size);
    //in = cv::imread(imagePath);


    cv::Mat logpolar_frame, bgr_frame;

    for (;;) {
        cap >> bgr_frame;
        if (bgr_frame.empty())break;

        cv::imshow("Example1", bgr_frame);

        cv::logPolar(
            bgr_frame,
            logpolar_frame,
            cv::Point2f(
                bgr_frame.cols / 2,
                bgr_frame.rows / 2
            ),
            40,
            cv::WARP_FILL_OUTLIERS
        );
        cv::imshow("Example2", logpolar_frame);
        writer << logpolar_frame;

        char c = cv::waitKey(10);
        if (c == 27)break;
    }
    cap.release();
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;

    int c;
    //cv::pyrDown(in, out);
    // cv::pyrDown(out, out);//金字塔
    // cv::cvtColor(in, in, cv::COLOR_RGB2GRAY);//颜色变换
    // cv::Canny(in, out, 10, 100, 3, true); //边缘检测

    //cv::Mat grad_x, grad_y;
    //cv::Mat abs_grad_x, abs_grad_y;
    //cv::Mat grad;

  //  Sobel(in, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT);
  //  convertScaleAbs(grad_x, abs_grad_x);

    /// 求Y方向梯度
    //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  //  Sobel(in, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT);
  //  convertScaleAbs(grad_y, abs_grad_y);

    /// 合并梯度(近似)
   // addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

   

   // cv::imshow("Example1", in);
  //  cv::imshow("Example2", grad);
    
    cv::waitKey(0);
    return 0;
}



//高斯模糊处理
int GaussianChange(const cv::String imagePath) {
    cv::Mat in, out;
    cv::namedWindow("Example1", 1);
    cv::namedWindow("Example2", 1);

    in = cv::imread(imagePath);

    cv::GaussianBlur(in, out, cv::Size(5, 5), 3, 3);//两次高斯模糊处理
    cv::GaussianBlur(out, out, cv::Size(5, 5), 3, 3); //GaussianBlur(), blur(), medianBlur() or bilateralFilter()等

    cv::imshow("Example1", in);
    cv::imshow("Example2", out);
    cv::waitKey(0);
    return 0;
}


int runVideo(const cv::String videoPath) {
    cv::namedWindow("Example", 1);
    g_cap.open(videoPath);

    int frames = (int)g_cap.get(cv::CAP_PROP_FRAME_COUNT);
    int tmpW = (int)g_cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int tmpH = (int)g_cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    cv::createTrackbar("Position", "Example", &g_slider_position, frames, onTrackbarSlide);

    cv::Mat frame;
    while (1) {
        if (g_run != 0) {
            g_cap >> frame;
            if (frame.empty())
                break;
            int current_pos = (int)g_cap.get(cv::CAP_PROP_POS_FRAMES);
            g_dontset = 1;

            cv::setTrackbarPos("Position", "Example", current_pos);
            cv::imshow("Example", frame);

            g_run -= 1;
        }

        char c = (char)cv::waitKey(10);
        if (c == 's') {
            g_run = 1;
            printf("single step,run= %d", g_run);
        }
        if (c == 'r') {
            g_run = -1;
            printf("run mode,run=%d", g_run);
        }
        if (c == 27)
            break;
    }
    cv::destroyWindow("Example");
    return 0;
}

void onTrackbarSlide(int pos, void*) {
  //  g_cap.set(cv::CAP_PROP_POS_FRAMES, pos);

    if (!g_dontset)
        g_run = 1;
    g_dontset = 0;
}

int readImage(const cv::String imgPath){
   
    cv::Mat img = cv::imread(imgPath, 1);
   
    if (img.empty()) {
        printf("read img error");
        return -1;
    }

    cv::namedWindow("Example", cv::WINDOW_AUTOSIZE);
    cv::imshow("Example", img);
    cv::waitKey(0);
    cv::destroyWindow("Example");
    return 0;
}

int readCamera() {


    
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;

    int c;


    cv::VideoCapture cap;
    cv::namedWindow("Example", cv::WINDOW_AUTOSIZE);
    if (!cap.open(0))
        printf("open camera failed");

    cv::Mat frame ,out;
    while (true)
    {
        cap >> frame;

        if (frame.empty())
            break;

       // cv::GaussianBlur(frame, frame, cv::Size(3, 3), 0, 0);
        /// 转换为灰度图
        cv::cvtColor(frame, out, CV_RGB2GRAY);

        cv::Mat grad_x, grad_y;
        cv::Mat abs_grad_x, abs_grad_y;
        cv::Mat grad;

        Sobel(frame, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT);
        convertScaleAbs(grad_x, abs_grad_x);

        /// 求Y方向梯度
        //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
        Sobel(frame, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT);
        convertScaleAbs(grad_y, abs_grad_y);

        /// 合并梯度(近似)
        addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

        cv::imshow("Example", grad);

        if (cv::waitKey(33) >= 0)
            break;
    }

    return 0;
}
