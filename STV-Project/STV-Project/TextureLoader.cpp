#include "TextureLoader.h"
#include "Dependencies\freeimage\FreeImage.h"
#include <fstream>
#include <iostream>



TextureLoader::TextureLoader()
{
}

TextureLoader::~TextureLoader()
{
	cout << "Deleting all textures." << endl;
	if(_textures.size() != 0) glDeleteTextures(_textures.size(), &_textures[0]);
}

GLuint TextureLoader::LoadPNG(string file_path)
{
	return LoadPNG(file_path, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST);
}

GLuint TextureLoader::LoadLinearPNG(string file_path)
{
	return LoadPNG(file_path, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR);
}

GLuint TextureLoader::LoadPNG(string file_path, GLenum min_mode, GLenum mag_mode)
{
	cout << "Loading texture '" << &file_path[0] << "' ";
	FREE_IMAGE_FORMAT format = FIF_UNKNOWN;
	FIBITMAP* data(0);

	//Check signature to deduce format
	format = FreeImage_GetFileType(file_path.c_str(), 0);

	//Guess based on extention
	if (format == FIF_UNKNOWN)
	{
		format = FreeImage_GetFIFFromFilename(file_path.c_str());
	}
	//Unknown
	if (format == FIF_UNKNOWN)
	{
		format = FreeImage_GetFIFFromFilename(file_path.c_str());
		cout << endl << "\tError: Cannot deduce format." << endl;
		return 0;
	}

	if (FreeImage_FIFSupportsReading(format)) 
	{
		data = FreeImage_Load(format, file_path.c_str());
	}
	if(!data)
	{
		cout << endl << "\tError: Cannot load data." << endl;
	}
	
	BYTE* raw_data = FreeImage_GetBits(data);

	int width = FreeImage_GetWidth(data);
	int height = FreeImage_GetHeight(data);
	int bits_per_pixel = FreeImage_GetBPP(data);

	if (width == 0 || height == 0 || bits_per_pixel == 0) 
	{
		cout << endl << "\tError: Width, Height or BPP is 0." << endl;
	}

	//Load as texture
	GLuint texture;
	glGenTextures(1, &texture);
	_textures.push_back(texture);
	cout << "ID: " << texture << " ";

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_mode);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_mode);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_BGRA, GL_UNSIGNED_BYTE, raw_data);

	FreeImage_Unload(data);
	cout << "done." << endl;
	return texture;
}