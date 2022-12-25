#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/core/mat.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <iostream>
#include <omp.h>
#include <time.h>


using namespace cv;
using namespace std;


Mat sobelFilter(Mat originalImage){
    
    // Define rows and columns shape
    int rows = originalImage.rows;
    int cols = originalImage.cols;

    // Define matrix to convolve and result matrix
    Mat paddedImage, targetImage;

    // Initialize result matrix
    targetImage = Mat(rows, cols, originalImage.type());

    // Create padding
    copyMakeBorder(originalImage, paddedImage, 1, 1, 1, 1, BORDER_REPLICATE);

    // Define kernel direction
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
    
    // set number of threads
    omp_set_num_threads(16);

    // iterate over all pixels parallelising
    #pragma omp parallel for shared(targetImage) collapse(2)
    for(int row = 1; row < rows; row++){
        for(int column = 1; column < cols; column++){
            // for each pixel, compute the change of intensity in both directions
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
            // update with new value each pixel
            targetImage.at<uchar>(row, column) = sqrt( pow(intensityChangeRow, 2) + pow(intensityChangeCol, 2))/8;
        }
    }

    return targetImage;
}


int main()
{
    // Initialize time
    double beginTime = clock();

    // Define paths
    string imagePath = samples::findFile("images/lena.png");
    string targetImagePath = "images/lena_sobel.png";

    // Read image
    Mat img = imread(imagePath, IMREAD_GRAYSCALE);

    if(img.empty())
    {
        cout << "Could not read the image: " << imagePath << endl;
        return 1;
    }

    // Apply sobel filter to the image
    Mat result = sobelFilter(img);

    // Save image with Sobel filter
    imwrite(targetImagePath, result);

    // Seconds spent
    cout << "SECONDS SPENT: " << float( clock () - beginTime )/CLOCKS_PER_SEC << endl;

    return 0;
}