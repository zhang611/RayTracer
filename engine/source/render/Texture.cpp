#include "Texture.h"
#include <iostream>
#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace RayTracer
{
    
    Texture::Texture(int width, int height)
    : Texture(width, height, std::make_unique<Color[]>(width * height), 3)
    {
    }
    Texture::Texture(int width, int height, std::unique_ptr<Color[]> pBuffer, int channels)
    {
        this->pBuffer = std::move(pBuffer);
        this->width = width;
        this->height = height;
    }

    void Texture::PutPixel( unsigned int x,unsigned int y,Color c )
    {
        
        assert( x >= 0 );
        assert( y >= 0 );
        assert( x < width );
        assert( y < height );
        pBuffer[y * width + x] = c;
    }
    Color Texture::GetPixel( unsigned int x,unsigned int y ) const
    {
        assert( x >= 0 );
        assert( y >= 0 );
        assert( x < width );
        assert( y < height );
        return pBuffer[y * width + x];
    }

    std::shared_ptr<Texture> Texture::LoadFrom(const std::string name)
    {
        int width;
        int height;
        int channels;
        unsigned char *image = stbi_load(name.c_str(), &width, &height, &channels, STBI_default);
        if (!image) {
            // throw std::runtime_error("Failed to load image: " + filename);
            char error[50] = "Failed to load image: ";
            strcat_s(error, sizeof(error), name.c_str());
            std::cerr << error << std::endl;
            return nullptr;
        }

        assert(width > 0 && height > 0 && channels > 0);
	
        std::unique_ptr<Color[]> pBuffer = std::make_unique<Color[]>(width * height);
        
        Color c;

        for (int i = 0; i < width * height; i++)
        {
            c.r = image[i * channels];
            c.g = image[i * channels + 1];
            c.b = image[i * channels + 2];
            c.a = channels == 4 ? image[i * channels + 3] : 255;
            pBuffer[i] = c;
        }
        
        stbi_image_free(image);
        return std::make_shared<Texture>(width, height, std::move(pBuffer), channels);
    } 
}