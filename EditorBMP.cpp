#include "BitMap.h"
#include "Image.h"
#include "processing.h"
#include <iostream>

int main(int argc, char* argv[])
{

    Bitmap bmp;
    bmp.load("C:\\Users\\vflorens\\source\\repos\\EditorBmp\\EditorBMP\\FFVII.bmp");
    auto image = bmp.getImage();

    if (std::strcmp(argv[1], "Greyscale") == 0)
    {
        processing::convertToGray(image);
        std::cout << "Grey : " << (std::atoi(argv[2]));
    }

    else if (std::strcmp(argv[1], "Binary") == 0)
    {
        processing::convertToGray(image);
        processing::convertToBinary(image);
        std::cout << "Binary" << (std::atoi(argv[2]));
    }

    else if (std::strcmp(argv[1], "Zoom") == 0)
    {
        processing::zoom(image, 0.5f, RGB{ 255, 255, 255 });
        std::cout << "Zoom" << (std::atoi(argv[2]));
    }

    else if (std::strcmp(argv[1], "Rotation") == 0)
    {
        processing::rotation(image, 20.0f, RGB(255, 255, 255));
        std::cout << "Rotation" << (std::atoi(argv[2]));
    }

    else if (std::strcmp(argv[1], "Histogram") == 0)
    {
        processing::StretchHistogramme(image);
        std::cout << "Histogram" << (std::atoi(argv[2]));
    }

    else if (std::strcmp(argv[1], "MeanFilter") == 0)
    {
        processing::meanfilter(image);
        std::cout << "Mean filter" << (std::atoi(argv[2]));
    }

    else if (std::strcmp(argv[1], "MedFilter") == 0)
    {
        processing::medfilter(image, 7, 7);
        std::cout << "Median filter" << (std::atoi(argv[2]));

    }

    else
    {
        std::cerr << "Error " << argv[1] << "\n";
        return 1;
    }

    bmp.setImage(image);
    bmp.save("MyBitmap2.bmp");// crée le fichier bmp

    return 0;
}