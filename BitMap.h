#pragma once

#include <vector>
#include <string>
#include "Image.h"



class Bitmap
{
public:

    Bitmap(const int& width = 0, const int& height = 0);//constructeur class Bitmap qui param�tre la taille de l'image en hauteur et largeur de pixel


    int getWidth() const;


    int getHeight() const;



    void setWidth(const int& Widht);


    void setHeight(const int& Height);


    //Calcule la taille totale des headers
    int getHeaderSize() const;


    //Calcule la taille totale du fichier
    int getFileSize() const;


    //Calcule l'aire de l'image (donc la taille de l'image)
    int getImageSize() const;


    //void setData(RGB* data, const int& size);//sert � chargers les donn�es de pixels dans l'image BMP


    int getPadding(int val, int multiple) const;


    void savePixel(std::ofstream& file);


    // �crit le fichier  d'en t�te pour cr�er l'image
    bool save(const std::string& filename);

    bool load(const std::string& filename);

    void setImage(const Image& image);
    Image& getImage();




private:

    // r�solution d'image par d�faut du format du fichier
    int getPpm();


    void updateFileHeader();// permet d'update la taille de l'image quand on l'a modifiera


    void updateInfoHeader();// permet d'update les info de l'image quand on l'a modifiera


    /*Fichier d'en t�te qui permet de cr�er le fichier et de le lire*/
    uint8 m_fileHeader[14] = {
        'B', 'M', // Id
        0, 0, 0, 0, // Size
        0, 0, // Reserved
        0, 0, // Reserved
        54, 0, 0, 0, // Offset
    };
    /*Le DIB qui permet de donner des info d�taill� sur l'images, qui seront utilis� pour afficher l'image � l'�cran*/
    uint8 m_infoHeader[40] = {
        40, 0, 0, 0, // Header size
        0, 0, 0, 0, // Width
        0, 0, 0, 0, // Height
        1, 0, // Number of color plane
        24, 0, // Bits per pixels
        0, 0, 0, 0, // Compression default
        0, 0, 0, 0, // Image size
        0, 0, 0, 0, // Horizontal resolution
        0, 0, 0, 0, // Vertical resolution
        0, 0, 0, 0, // Number of colors
        0, 0, 0, 0, // Important colors without preferences

    };


    Image m_Image;

};