#pragma once
#include "MemoryListener.h"
#include "Dependencies\glew\glew.h"
#include <vector>

using namespace std;

class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();
	GLuint LoadPNG(string file_path);
	GLuint LoadLinearPNG(string file_path);
	GLuint LoadPNG(string file_path, GLenum min_mode, GLenum mag_mode);

private:
	vector<GLuint> _textures;
};

