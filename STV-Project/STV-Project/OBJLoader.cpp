#include "OBJLoader.h"

#include "GameManager.h"

#include <fstream>
#include <sstream>
#include <iostream>

map<string, Model*> OBJLoader::LoadOBJ(string path)
{
	vector<float> raw_vertex_buffer;
	vector<float> raw_uv_buffer;
	vector<float> raw_normal_buffer;
	vector<float> raw_face_buffer;

	cout << "Attempting to load OBJ '" << &path[0] << "'" << endl;
	ifstream input(path);
	if (!input.good()) 
	{
		cout << "\tError: Cannot locate file" << endl;
		terminate();
	}

	map<string, Model*> object_map;

	string current_model_name;
	string line;
	bool building = false;

	//Line by line
	while (getline(input, line)) 
	{
		string sub_code = line.substr(0, 2);

		//Vertex position
		if (sub_code == "v ")
		{
			istringstream line_stream(&line[2]);
			float x, y, z;
			line_stream >> x >> y >> z;
			raw_vertex_buffer.push_back(x);
			raw_vertex_buffer.push_back(y);
			raw_vertex_buffer.push_back(z);
		}
		//Vertex normal
		else if (sub_code == "vn")
		{
			istringstream line_stream(&line[2]);
			float x, y, z;
			line_stream >> x >> y >> z;
			raw_normal_buffer.push_back(x);
			raw_normal_buffer.push_back(y);
			raw_normal_buffer.push_back(z);
		}
		//Vertex UV
		else if (sub_code == "vt")
		{
			istringstream line_stream(&line[2]);
			float u, v;
			line_stream >> u >> v;
			raw_uv_buffer.push_back(u);
			raw_uv_buffer.push_back(v);
		}
		//Object name
		else if (sub_code == "g ")
		{
			istringstream line_stream(&line[2]);
			string name;
			line_stream >> name;
			current_model_name = name;
			cout << "\tObject '" << name << "' ";
			building = true;
		}
		//Face
		else if (sub_code == "f ")
		{
			stringstream faces(&line[2]);
			vector<float> raw_int;

			string face;
			string operands;
			
			while (getline(faces, face, ' ')) {
				stringstream ops(face);

				while (getline(ops, operands, '/'))
				{
					raw_int.push_back((unsigned int)stoi(operands));
				}
			}
			raw_face_buffer.insert(raw_face_buffer.end(), raw_int.begin(), raw_int.end());
		}
		//Look out for comment at end of face section
		else if (sub_code == "# " && building)
		{
			cout << "building.. ";
			object_map[current_model_name] = Build(
				raw_vertex_buffer,
				raw_uv_buffer,
				raw_normal_buffer,
				raw_face_buffer
				);

			building = false; 
			cout << " Done." << endl;

			raw_vertex_buffer.clear();
			raw_uv_buffer.clear();
			raw_normal_buffer.clear();
			raw_face_buffer.clear();

		}

	}
	return object_map;
}

Model* OBJLoader::Build(
	vector<float>& raw_vertex_buffer,
	vector<float>& raw_uv_buffer,
	vector<float>& raw_normal_buffer,
	vector<float>& raw_face_buffer)
{

	vector<float> uv; 
	for (int i = 0; i < raw_vertex_buffer.size()* 2.0/3.0; i++)
		uv.push_back(0);

	vector<float> normal;
	for (int i = 0; i < raw_vertex_buffer.size(); i++)
		normal.push_back(0);

	vector<unsigned int> indices;
	
	for (int i = 0; i < raw_face_buffer.size(); i+=3)
	{
		//Correct offset
		unsigned int position_index = raw_face_buffer[i] - 1;
		unsigned int uv_index = raw_face_buffer[i+1] - 1;
		unsigned int normal_index = raw_face_buffer[i+2] - 1;

		uv[position_index * 2 + 0] = raw_uv_buffer[uv_index * 2 + 0];
		uv[position_index * 2 + 1] = raw_uv_buffer[uv_index * 2 + 1];

		normal[position_index * 3] = raw_normal_buffer[normal_index * 3];
		normal[position_index * 3 + 1] = raw_normal_buffer[normal_index * 3 + 1];
		normal[position_index * 3 + 2] = raw_normal_buffer[normal_index * 3 + 2];

		indices.push_back(position_index);
	}

	Model* model = GameManager::getMain()->model_loader->CreateModel(raw_vertex_buffer, uv, indices);
	return model;
}