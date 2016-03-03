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

	int total_vertex_count = 0;
	int total_uv_count = 0;
	int total_normal_count= 0;

	cout << "Attempting to load OBJ '" << &path[0] << "'" << endl;
	ifstream input(path);
	if (!input.good()) 
	{
		cout << "\tError: Cannot locate file" << endl;
		terminate();
	}

	map<string, Model*> object_map;

	string current_model_name = "nullptr::object name";
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

				int track = 0;
				while (getline(ops, operands, '/'))
				{
					int index = (unsigned int)stoi(operands);
					bool yes = false;

					//Correcting offset for object
					switch (track) 
					{
					case 0:
						index -= total_vertex_count;
						if (yes)
							cout << " v " << index  << endl;
						break;
					case 1:
						index -= total_uv_count;
						break;
					case 2:
						index -= total_normal_count;
						break;
					}

					index -= 1; //Correct offest for OpenGL
					raw_int.push_back(index);

					track++; 
					if (track == 3)
					{
						track = 0;
					}
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

			//Updating offset for next object
			total_vertex_count += raw_vertex_buffer.size()/3;
			total_uv_count += raw_uv_buffer.size()/2;
			total_normal_count += raw_normal_buffer.size()/3;

			raw_vertex_buffer.clear();
			raw_uv_buffer.clear();
			raw_normal_buffer.clear();
			raw_face_buffer.clear();
		}
	}

	//In the event object has no name
	if (current_model_name == "nullptr::object name") 
	{
		cout << "\tNo object name found; using 'default' as key. ";
		cout << "building.. ";
		object_map[current_model_name] = Build(
			raw_vertex_buffer,
			raw_uv_buffer,
			raw_normal_buffer,
			raw_face_buffer
			);

		building = false;
		cout << " Done." << endl;

		//Updating offset for next object
		total_vertex_count += raw_vertex_buffer.size() / 3;
		total_uv_count += raw_uv_buffer.size() / 2;
		total_normal_count += raw_normal_buffer.size() / 3;

		raw_vertex_buffer.clear();
		raw_uv_buffer.clear();
		raw_normal_buffer.clear();
		raw_face_buffer.clear();
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

	//cout << endl;
	for (int i = 0; i < raw_face_buffer.size(); i+=3)
	{
		unsigned int position_index = raw_face_buffer[i];
		unsigned int uv_index = raw_face_buffer[i+1];
		unsigned int normal_index = raw_face_buffer[i+2];

		//cout << position_index + 1 << "/" << uv_index + 1 << "/" << normal_index + 1 << endl;

		uv[position_index * 2 + 0] = raw_uv_buffer[uv_index * 2 + 0];
		uv[position_index * 2 + 1] = raw_uv_buffer[uv_index * 2 + 1];

		normal[position_index * 3] = raw_normal_buffer[normal_index * 3];
		normal[position_index * 3 + 1] = raw_normal_buffer[normal_index * 3 + 1];
		normal[position_index * 3 + 2] = raw_normal_buffer[normal_index * 3 + 2];

		indices.push_back(position_index);
	}

	Model* model = GameManager::getMain()->model_loader->CreateModel(raw_vertex_buffer, uv, normal, indices);
	return model;
}