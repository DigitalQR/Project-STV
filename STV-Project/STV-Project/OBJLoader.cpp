#include "OBJLoader.h"

#include "GameManager.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

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
			vector<float> values = getValues(line.substr(2), ' ');
			raw_vertex_buffer.push_back(values[0]);
			raw_vertex_buffer.push_back(values[1]);
			raw_vertex_buffer.push_back(values[2]);
		}
		//Vertex normal
		else if (sub_code == "vn")
		{
			vector<float> values = getValues(line.substr(2), ' ');
			raw_normal_buffer.push_back(values[0]);
			raw_normal_buffer.push_back(values[1]);
			raw_normal_buffer.push_back(values[2]);
		}
		//Vertex UV
		else if (sub_code == "vt")
		{
			vector<float> values = getValues(line.substr(2), ' ');
			raw_uv_buffer.push_back(values[0]);
			raw_uv_buffer.push_back(values[1]);
		}
		//Object name
		else if (sub_code == "g ")
		{
			current_model_name = &line[2];
			cout << "\tObject '" << &current_model_name[0] << "' ";
			building = true;
		}
		//Face
		else if (sub_code == "f ")
		{
			vector<string> faces = getSubText(line.substr(2), ' ');

			for (string face: faces)
			{
				vector<float> values = getValues(face, '/');

				raw_face_buffer.push_back((int)values[0] - total_vertex_count - 1);
				raw_face_buffer.push_back((int)values[1] - total_uv_count - 1);
				raw_face_buffer.push_back((int)values[2] - total_normal_count - 1);
			}
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
		total_normal_count += raw_normal_buffer.size()/ 3;

		raw_vertex_buffer.clear();
		raw_uv_buffer.clear();
		raw_normal_buffer.clear();
		raw_face_buffer.clear();
	}

	return object_map;
}

vector<float> OBJLoader::getValues(string raw_text, char split)
{
	vector<float> content;
	string current_line = "";

	for (char c : raw_text)
	{
		if (c == split)
		{
			if (current_line != "") content.push_back(stof(current_line));
			current_line = "";
		}
		else
			current_line += c;
	}
	if (current_line != "") content.push_back(stof(current_line));

	return content;
}

vector<string> OBJLoader::getSubText(string raw_text, char split)
{
	vector<string> content;
	string current_line = "";

	for (char c : raw_text)
	{
		if (c == split)
		{
			if (current_line != "") content.push_back(current_line);
			current_line = "";
		}
		else
			current_line += c;
	}
	if (current_line != "") content.push_back(current_line);

	return content;
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
		unsigned int position_index = raw_face_buffer[i];
		unsigned int uv_index = raw_face_buffer[i+1];
		unsigned int normal_index = raw_face_buffer[i+2];
		
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