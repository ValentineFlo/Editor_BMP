#include "processing.h"
#include <cmath>
#include <algorithm>

Roi::Roi(const int& width_, const int& height_)
	: width(width_)
	, height(height_)
{
	pixels.resize(width * height);
}

void Roi::attachImage(Image& image, const int& rowCenter, const int& colCenter)
{
	int r = 0;
	int HalfHeight = height / 2;
	int HalfWidth = height / 2;



	for (auto row = rowCenter - HalfHeight; row <= rowCenter + HalfWidth; ++row)
	{
		int c = 0;
		for (auto col = colCenter - width / 2; col <= colCenter + width / 2; ++col)
		{
			pixels[r * width + c] = image.getPixel(row, col);
			++c;
		}
		++r;
	}
}

RGB Roi::meanOf(Image& image, const int& centerRow, const int& centerCol)
{
	attachImage(image, centerRow, centerCol);
	return mean();
}

RGB Roi::mean()//moyenne
{
	RGB result;
	int averageR = 0;
	int averageG = 0;
	int averageB = 0;


	for (auto i = 0; i < (height * width); ++i)
	{
		averageR += pixels[i].channels[0];
		averageG += pixels[i].channels[1];
		averageB += pixels[i].channels[2];
	}


	result.channels[0] = static_cast<uint8>(averageR / (height * width));
	result.channels[1] = static_cast<uint8>(averageG / (height * width));
	result.channels[2] = static_cast<uint8>(averageB / (height * width));

	return result;
}

RGB Roi::medOf(Image& image, const int& centerRow, const int& centerCol)
{
	attachImage(image, centerRow, centerCol);
	return med();
}
RGB Roi::med()
{
	std::vector<uint8> redValues, greenValues, blueValues;

	for (auto i = 0; i < (height * width); ++i)
	{
		redValues.push_back(pixels[i].channels[0]);
		greenValues.push_back(pixels[i].channels[1]);
		blueValues.push_back(pixels[i].channels[2]);
	}

	// Trier les vecteurs
	std::sort(redValues.begin(), redValues.end());
	std::sort(greenValues.begin(), greenValues.end());
	std::sort(blueValues.begin(), blueValues.end());

	// Récupérer les valeurs médianes
	size_t middleIndex = (redValues.size() + 1) / 2;
	uint8 medianR = redValues[middleIndex];
	middleIndex = (greenValues.size() + 1) / 2;
	uint8 medianG = greenValues[middleIndex];
	middleIndex = (blueValues.size() + 1) / 2;
	uint8 medianB = blueValues[middleIndex];

	// Nouveau : Multiplier par 2 comme dans mean()
	return RGB{
		medianR,
		medianG,
		medianB
	};
}


namespace processing
{
	void convertToGray(Image& image)
	{

		for (auto i = 0; i < image.getSize(); ++i)
		{
			int average = 0;
			for (auto channelsIdx = 0; channelsIdx < 3; ++channelsIdx)
				average += image[i].channels[channelsIdx];

			average /= 3;

			for (auto channelsIdx = 0; channelsIdx < 3; ++channelsIdx)
				image[i].channels[channelsIdx] = static_cast<uint8>(average);
		}
	}
	void convertToBinary(Image& image, const uint8& thearshold)
	{

		for (auto i = 0; i < image.getSize(); ++i)
		{
			for (auto channelsIdx = 0; channelsIdx < 3; ++channelsIdx)
				image[i].channels[channelsIdx] = image[i].channels[channelsIdx] > thearshold ?
				std::numeric_limits<uint8>::max() : 0;

		}
	}
	void fill(Image& image, const RGB& fillColor)
	{
		for (int row = 0; row < image.getHeight(); ++row)
		{
			for (int col = 0; col < image.getWidth(); ++col)
			{
				image.getPixel(row, col) = fillColor;
			}
		}
	}
	void zoom(Image& image, float zoomFactor, const RGB& fillcolor)
	{
		Point midPoint(image.getWidth() / 2, image.getHeight() / 2);
		zoom(image, zoomFactor, midPoint, fillcolor);
	}

	void zoom(Image& image, float zoomFactor, const Point& origin, const RGB& fillColor)
	{
		Image src = image;
		Image& dest = image;


		for (int row = 0; row < image.getHeight(); ++row)
		{
			for (int col = 0; col < image.getWidth(); ++col)
			{
				Point destinationCoord(col, row);
				Point SourceCoord = origin + (Point(col, row) - origin) / zoomFactor;
				if (dest.IsInside(SourceCoord.y, SourceCoord.x))
					dest.getPixel(destinationCoord.y, destinationCoord.x) = src.getPixel(SourceCoord.y, SourceCoord.x);
				else
					dest.getPixel(destinationCoord.y, destinationCoord.x) = fillColor;
			}
		}
	}

	float degToRad(float angleInDegres)
	{
		return angleInDegres / 180.0f * 3.14;
	}


	void rotation(Image& image, float angleInDegrees, const Point& center, const RGB& fillColor)

	{
		Image src = image;
		Image& dest = image;


		for (int row = 0; row < image.getHeight(); ++row)
		{
			for (int col = 0; col < image.getWidth(); ++col)
			{
				float cosa = std::cos(-degToRad(angleInDegrees));
				float sina = std::sin(-degToRad(angleInDegrees));
				Point destinationCoord(col, row);

				Point Msrc(col, row);
				Msrc = Msrc - center;

				Point Mdest(Msrc.x * cosa - Msrc.y * sina,
					Msrc.x * sina + Msrc.y * cosa);

				Mdest = Mdest + center;
				if (dest.IsInside(Mdest.y, Mdest.x))
					dest.getPixel(destinationCoord.y, destinationCoord.x) = src.getPixel(Mdest.y, Mdest.x);
				else
					dest.getPixel(destinationCoord.y, destinationCoord.x) = fillColor;
			}
		}
	}
	void rotation(Image& image, float angleInDegrees, const RGB& fillColor)
	{
		Point midPoint(image.getWidth() / 2, image.getHeight() / 2);
		rotation(image, angleInDegrees, midPoint, fillColor);
	}


	void StretchHistogramme(Image& image)
	{
		RGB max;
		RGB min;

		const auto absoluteMax = std::numeric_limits<uint8> ::max();
		const auto absoluteMin = std::numeric_limits<uint8> ::min();
		for (auto channelIdx = 0; channelIdx < 3; ++channelIdx)
		{
			max.channels[channelIdx] = absoluteMin;
			min.channels[channelIdx] = absoluteMax;
		}
		for (int i = 0; i < image.getSize(); ++i)
		{
			for (auto channelIdx = 0; channelIdx < 3; ++channelIdx)
			{
				if (max.channels[channelIdx] < image[i].channels[channelIdx])
					max.channels[channelIdx] = image[i].channels[channelIdx];
				if (min.channels[channelIdx] > image[i].channels[channelIdx])
					min.channels[channelIdx] = image[i].channels[channelIdx];
			}


		}
		for (auto i = 0; i < image.getSize(); i++)
		{
			for (auto channelIdx = 0; channelIdx < 3; ++channelIdx)
			{
				const int numerator = absoluteMax * image[i].channels[channelIdx] - min.channels[channelIdx];
				image[i].channels[channelIdx] = static_cast<uint8>(numerator / (max.channels[channelIdx] - min.channels[channelIdx]));
			}
		}

	}
	void meanfilter(Image& image, const int& width, const int& height)
	{
		Roi roi(width, height);
		Image src = image;
		Image& dest = image;

		for (auto row = height / 2; row < image.getHeight() - height / 2; ++row)
		{
			for (auto col = width / 2; col < image.getWidth() - width / 2; ++col)
			{
				dest.getPixel(row, col) = roi.meanOf(src, row, col);
			}
		}

	}
	void medfilter(Image& image, const int& width, const int& height)
	{
		Roi roi(width, height);
		Image src = image;
		Image& dest = image;

		for (auto row = height / 2; row < image.getHeight() - height / 2; ++row)
		{
			for (auto col = width / 2; col < image.getWidth() - width / 2; ++col)
			{
				dest.getPixel(row, col) = roi.medOf(src, row, col);
			}
		}

	}
}



