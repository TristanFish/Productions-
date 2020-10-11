#include "Texture.h"
#include <iostream>

Texture::Texture()
{
	textureID = 0;
}



bool Texture::LoadImage(const char* filename)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	SDL_Surface* textureSurface = IMG_Load(filename);
	if (textureSurface == nullptr) {
		std::cout << "texture: " << filename << " didn't load" << std::endl;
		return false;
	}
	int numOfColours = textureSurface->format->BytesPerPixel;
	int format;
	if (numOfColours == 4)
	{
		if (textureSurface->format->Rmask == 0x000000ff)
		{
			format = GL_RGBA;
		}
		else
		{
			format = GL_BGRA;
		}
	}
	else if (numOfColours == 3)
	{
		if (textureSurface->format->Rmask == 0x000000ff)
		{
			format = GL_RGB;
		}
		else
		{
			format = GL_BGR;
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, numOfColours, textureSurface->w, textureSurface->h, 0, format, GL_UNSIGNED_BYTE, textureSurface->pixels);
	
	SDL_FreeSurface(textureSurface);
	textureSurface = nullptr;
	/// Wrapping and filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0); /// Unbind the texture
	return false;
}

void Texture::DestroyTexture()
{
	glDeleteTextures(1, &textureID);
}


Texture::~Texture()
{

}
