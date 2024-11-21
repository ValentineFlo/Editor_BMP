#include "Image.h"
#include <iostream>
RGB::RGB(const uint8& r, const uint8& g, const uint8& b)
{
    channels[0] = r;
    channels[1] = g;
    channels[2] = b;
}

Image::Image(const int& Width, const int& Height)
    :m_Width(Width)
    , m_Height(Height)
{}

void Image::clear()
{
    m_data.clear();
}

void Image::setSize(const int& Width, const int& Height)
{
    m_Width = Width;
    m_Height = Height;
    m_data.resize(m_Width * m_Height);
}

RGB& Image::getPixel(const int& row, const int& col)
{

    if (row >= m_Height || col >= m_Width)
        std::cerr << "Out of range" << std::endl;

    return m_data[row * m_Width + col];
}


int Image::getWidth() const
{
    return m_Width;
}
int Image::getHeight() const
{
    return m_Height;
}
int Image::getSize()const
{
    return (m_Height * m_Width);
}

bool Image::IsInside(const int& row, const int& col) const
{
    if (row >= m_Height || col >= m_Width || row < 0 || col < 0)
        return false;
    return true;

}

RGB& Image::operator[](const size_t& index)
{
    return  m_data[index];
}

Point operator-(const Point& op1, const Point& op2)
{
    return Point(op1.x - op2.x, op1.y - op2.y);
}
Point operator+(const Point& op1, const Point& op2)
{
    return Point(op1.x + op2.x, op1.y + op2.y);
}
Point operator*(const Point& op1, float op2)
{
    return Point(op1.x * op2, op1.y * op2);
}
Point operator/(const Point& op1, float op2)
{
    return Point(op1.x / op2, op1.y / op2);
}