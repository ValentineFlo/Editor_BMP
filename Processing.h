#pragma once
#include "Image.h"
class Image;

struct Roi
{
	Roi(const int& width_ = 0, const int& height_ = 0);

	void attachImage(Image& image, const int& centerRow, const int& centerCol);

	RGB meanOf(Image& image, const int& centerRow, const int& centerCol);

	RGB medOf(Image& image, const int& centerRow, const int& centerCol);

	RGB mean();

	RGB med();
	int width;
	int height;
	std::vector<RGB> pixels;
};

namespace processing
{
	void convertToGray(Image& image);
	void convertToBinary(Image& image, const uint8& thearshold = 122);
	void zoom(Image& image, float zoomFactor, const Point& origin, const RGB& fillcolor);
	void zoom(Image& image, float zoomFactor, const RGB& fillcolor);
	void rotation(Image& image, float angleInDegrees, const Point& center, const RGB& fillColor);
	void rotation(Image& image, float angleInDegrees, const RGB& fillColor);
	void StretchHistogramme(Image& image);
	void meanfilter(Image& image, const int& width = 3, const int& height = 3);
	void medfilter(Image& image, const int& width = 3, const int& height = 3);
}