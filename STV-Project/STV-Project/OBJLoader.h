#pragma once
#include "MemoryListener.h"

#include "Model.h"
#include <map>
#include <vector>

using namespace std;

class OBJLoader
{
public:
	static map<string, Model*> LoadOBJ(string path);

private:
	static Model* Build(
		vector<float>& raw_vertex_buffer,
		vector<float>& raw_uv_buffer,
		vector<float>& raw_normal_buffer,
		vector<float>& raw_face_buffer );
};

