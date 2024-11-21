#pragma once
#include <vector>

using uint8 = unsigned char;

struct Point
{
    Point(float x_, float y_) :x(x_), y(y_)
    {}
    float x;
    float y;
};

Point operator-(const Point& op1, const Point& op2);

Point operator+(const Point& op1, const Point& op2);

Point operator*(const Point& op1, float op2);

Point operator/(const Point& op1, float op2);



struct RGB
{
    RGB(const uint8& r = 0, const uint8& g = 0, const uint8& b = 0);

    uint8 channels[3];//pixel codé sur 3 octets (RGB pour la couleur)
};

class Image
{
public:
    Image(const int& Width = 0, const int& Height = 0);

    void clear();
    void setSize(const int& Width, const int& Height);
    RGB& getPixel(const int& row, const int& col);
    int getWidth()const;
    int getHeight()const;
    int getSize()const;

    bool IsInside(const int& row, const int& col) const;

    RGB& operator[](const size_t& index);
private:
    int m_Width;
    int m_Height;
    std::vector <RGB> m_data;
};