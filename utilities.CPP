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

    Mat paddedImage, targetImage =  originalImage.clone();

    // Kernels are 3 x 3 so padding should be set to the original image
    int rows = originalImage.rows;
    int cols = originalImage.cols;
    copyMakeBorder(originalImage, paddedImage, 1, 1, 1, 1, BORDER_REPLICATE);

    float kernelx[3][3] = { 
        { -1, 0, 1 },
        { -2, 0, 2 },
        { -1, 0, 1 } 
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
                    int targetValue = (int)(paddedImage.at<uchar>(targetX,targetY));

                    intensityChangeRow += targetValue * kernelx[kernelRow][kernelCol];
                    intensityChangeCol += targetValue * kernely[kernelRow][kernelCol];
                }
            }

            double resultForPixel = sqrt( pow(intensityChangeRow, 2) + pow(intensityChangeCol, 2));
            targetImage.at<uchar>(row, column) = resultForPixel;
        }
    }

    return targetImage;
}


int main()
{
    std::string image_path = samples::findFile("images/tesla.jpg");
    Mat img = imread(image_path, IMREAD_GRAYSCALE);
    if(img.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }
    
    cout << "Original image size is: " << img.size() << endl;

    /*int down_width = 800;
    int down_height = 1000;
    Mat resized_down;
    resize(img, resized_down, Size(down_width, down_height));*/

    imshow("Image", img);
    waitKey(0);

    Mat result = sobelFilter(img);

    cout << "New image size is: " << result.size() << endl;

    imshow("Image", result);
    waitKey(0);

    ///////////////////////////////////////////////////////////////////////////////////////
    /// Generate grad_x and grad_y
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y, grad;
    int ddepth = CV_16S;

    /// Gradient X
    Sobel(img, grad_x, ddepth, 1, 0, 1, 1, 0, BORDER_DEFAULT);

    /// Gradient Y
    Sobel(img, grad_y, ddepth, 0, 1, 1, 1, 0, BORDER_DEFAULT);
    //![sobel]

    //![convert]
    // converting back to CV_8U
    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);
    //![convert]

    //![blend]
    /// Total Gradient (approximate)
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    //![blend]

    //![display]
    imshow("Ellos", grad);
    waitKey(0);
    return 0;
}