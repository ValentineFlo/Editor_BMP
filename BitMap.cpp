#pragma once

#include "BitMap.h"
#include <iostream>
#include <fstream>

Bitmap::Bitmap(const int& width, const int& height)//constructeur class Bitmap qui paramètre la taille de l'image en hauteur et largeur de pixel
{
    setWidth(width);
    setHeight(height);
}

int Bitmap::getWidth() const
{
    static const auto startWidth = 4;
    static const auto endWidht = startWidth + 4;
    int Widht = 0;
    for (auto i = endWidht - 1; i >= startWidth; --i)
        Widht += (m_infoHeader[i] << ((i - startWidth) * 8));

    return Widht;
}

int Bitmap::getHeight() const
{
    static const auto startHeight = 8;
    static const auto endHeight = startHeight + 4;
    int Height = 0;
    for (auto i = endHeight - 1; i >= startHeight; --i)
        Height += (m_infoHeader[i] << ((i - startHeight) * 8));

    return Height;
}


void Bitmap::setWidth(const int& Widht)
{
    static const auto startWidth = 4;
    for (int i = startWidth; i < startWidth + 4; ++i)
        m_infoHeader[i] = static_cast<unsigned char>(Widht >> (i - startWidth) * 8);


}

void Bitmap::setHeight(const int& Height)
{
    static const auto startHeight = 8;
    for (int i = startHeight; i < startHeight + 4; ++i)
        m_infoHeader[i] = static_cast<unsigned char>(Height >> (i - startHeight) * 8);

}

//Calcule la taille totale des headers
int Bitmap::getHeaderSize() const
{
    return sizeof(m_fileHeader) + sizeof(m_infoHeader);
}

//Calcule la taille totale du fichier
int Bitmap::getFileSize() const
{
    return getHeaderSize() * getImageSize();
}

//Calcule l'aire de l'image (donc la taille de l'image)
int Bitmap::getImageSize() const
{
    const auto lineSizeWithPadding = 3 * getWidth() + getPadding(getWidth() * 3, 4);
    const auto ImageSize = lineSizeWithPadding * getHeight();
    return 3 * ImageSize;// taille de l'image * les 3 octets des pixels 
}

//void Bitmap::setData(RGB* data, const int& size)//sert à chargers les données de pixels dans l'image BMP
//{
//    m_data.clear();//Efface les données existantes dans m_data
//    m_data.setSize(getWidht()*getHeight());//Redimensionne m_data pour qu'il puisse contenir le nombre exact de pixels spécifié par size
//    for (auto i = 0; i < size; ++i)// la boucle for parcourt chaque pixel de l'image et la copie dans data
//        m_data[i] = data[i];
//}

int Bitmap::getPadding(int val, int multiple) const
{
    return(multiple - val % multiple) % multiple;
}



void Bitmap::savePixel(std::ofstream& file)
{
    const int Height = getHeight();
    const int Width = getWidth();
    for (int row = Height - 1; row >= 0; --row)
    {
        for (int col = 0; col < Width; ++col)
        {
            file.write(reinterpret_cast<const char*>(&m_Image.getPixel(row, col).channels[0]), 3);
        }
        for (int i = 0; i < getPadding(Width * 3, 4); ++i)
        {
            uint8 pad = 0;
            file.write(reinterpret_cast<char*>(&pad), 1);
        }

    }
}

// écrit le fichier  d'en tête pour créer l'image
bool Bitmap::save(const std::string& filename)
{
    std::ofstream file(filename, std::ios::binary);//crée le fichier et l'écrit en binaire

    if (!file.is_open())
        return false;

    updateFileHeader();
    updateInfoHeader();

    file.write(reinterpret_cast<char*>(&m_fileHeader[0]), 14); // file header to write
    file.write(reinterpret_cast<char*>(&m_infoHeader[0]), 40); // info header to write


    savePixel(file);



    file.close();
    return true;
}
bool Bitmap::load(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open())
        return false;

    file.read(reinterpret_cast<char*>(&m_fileHeader[0]), 14); // file header to read
    file.read(reinterpret_cast<char*>(&m_infoHeader[0]), 40); // info header to read


    const int Height = getHeight();
    const int Width = getWidth();
    const int padding = getPadding(Width * 3, 4);
    m_Image.clear();//Efface les données existantes dans m_data
    m_Image.setSize(Width, Height);//Redimensionne m_data pour qu'il puisse contenir le nombre exact de pixels spécifié par size
    for (int row = Height - 1; row >= 0; --row)
    {
        for (int col = 0; col < Width; ++col)
        {
            file.read(reinterpret_cast<char*>(&m_Image.getPixel(row, col).channels[0]), 3);
        }

        char* pad = new char[padding];
        file.read(&pad[0], padding);
        delete[] pad;
        pad = nullptr;


    }

    file.close();
    return true;

}

void Bitmap::setImage(const Image& image)
{
    m_Image = image;
    setWidth(m_Image.getWidth());
    setHeight(m_Image.getHeight());
}
Image& Bitmap::getImage()
{
    return m_Image;
}




// résolution d'image par défaut du format du fichier
int Bitmap::getPpm()
{
    return static_cast<int>(72 * 39.3701);
}

void Bitmap::updateFileHeader()// permet d'update la taille de l'image quand on l'a modifiera
{
    // transforme taille du fichier totale en entier en octet
    static const int start = 2;
    const auto FileSize = getFileSize();
    //m_fileHeader[2] = FileSize;
    //m_fileHeader[3] = FileSize >> 8;
    //m_fileHeader[4] = FileSize >> 16;
    //m_fileHeader[5] = FileSize >> 24;
    for (auto i = start; i < start + 4; ++i)
        m_fileHeader[i] = static_cast<uint8>(FileSize >> (i - start) * 8);//transforme FileSize en char et décale les bits en 8
}

void Bitmap::updateInfoHeader()// permet d'update les info de l'image quand on l'a modifiera
{
    //faire une fonction pour tout les for


    // transforme le nb de bits par pixel en entier en octet
    static const auto startImageSize = 20;
    const auto dataSize = getImageSize();
    for (auto i = startImageSize; i < startImageSize + 4; ++i)
        m_infoHeader[i] = static_cast<uint8>(dataSize >> (i - startImageSize) * 8);//transforme dataSize en char et décale les bits en 8

    // transforme la résolution de l'image en entier en octet
    static const auto startPpm = startImageSize + 4;
    const auto ppm = getPpm();//voir int getPpm()
    for (auto j = 0; j < 2; ++j)
    {
        const auto start = startPpm + 4 * j;
        for (auto i = start; i < startPpm + 4 * (j + 1); ++i)
            m_infoHeader[i] = static_cast<uint8>(ppm >> (i - start) * 8);//transforme la résolution d'image par défaut en char et décale les bits en 8
    }
}



