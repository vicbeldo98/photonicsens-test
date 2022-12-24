#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/core/mat.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <iostream>


using namespace cv;
using namespace std;


Mat sobelFilter(Mat originalImage){

    int rows = originalImage.rows;
    int cols = originalImage.cols;

    Mat paddedImage, targetImage;

    targetImage = originalImage.clone();

    copyMakeBorder(originalImage, paddedImage, 1, 1, 1, 1, BORDER_REPLICATE);

    float kernelx[3][3] = { 
        {  1, 0, -1 },
        {  2, 0, -2 },
        {  1, 0, -1 } 
    };

    float kernely[3][3] = {
        { -1, -2, -1 },
        { 0,  0,  0 },
        { 1,  2,  1 }
    };

    for(int row = 1; row < rows; row++){
        for(int column = 1; column < cols; column++){
            double intensityChangeRow = 0;
            double intensityChangeCol = 0;

            for(int kernelRow = 0; kernelRow < 3; kernelRow++){
                for(int kernelCol = 0; kernelCol < 3; kernelCol++){
                    int targetX = row + kernelRow - 1;
                    int targetY = column + kernelCol -1;
                    int targetValue = (int)(paddedImage.at<uchar>(targetX, targetY));

                    intensityChangeRow += targetValue * kernelx[kernelRow][kernelCol];
                    intensityChangeCol += targetValue * kernely[kernelRow][kernelCol];
                }
            }

            double resultForPixel = sqrt( pow(intensityChangeRow, 2) + pow(intensityChangeCol, 2))/8;

            targetImage.at<uchar>(row, column) = resultForPixel;
        }
    }

    return targetImage;
}


int main()
{
    // Define image path and read
    std::string image_path = samples::findFile("images/lena.png");
    Mat img = imread(image_path, IMREAD_GRAYSCALE);

    if(img.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }

    // Display image
    imshow("Image", img);
    waitKey(0);

    // Apply sobel filter to the image
    Mat result = sobelFilter(img);

    // Display image with Sobel filter
    imshow("Image", result);
    waitKey(0);

    return 0;
}