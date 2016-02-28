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

private:
	vector<GLuint> _textures;
};

