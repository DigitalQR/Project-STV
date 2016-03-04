#include "Shader.h"
#include <fstream>
#include <vector>

Shader::Shader(string name)
{
	//Load vertex/fragment shader
	string vertex_code = Shader::ReadShader("Shaders/" + name + ".vert");
	string frag_code = Shader::ReadShader("Shaders/" + name + ".frag");
	Init(name, vertex_code, frag_code);
}

void Shader::Init(string name, string vertex_source, string fragment_source)
{
	Shader::name = name;
	cout << "Creating shader for '" << name.c_str() << "'" << endl;

	//Compile vertex/fragment shader
	GLuint vertex_shader = Shader::CreateShader(GL_VERTEX_SHADER, vertex_source, name + ".vert");
	GLuint frag_shader = Shader::CreateShader(GL_FRAGMENT_SHADER, fragment_source, name + ".frag");

	int link_result = 0;

	//Create program and attempt to link vertex/fragment shader
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, frag_shader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);

	//Log if link failed
	if (link_result == GL_FALSE)
	{
		int log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

		vector<char> log(log_length);
		glGetProgramInfoLog(program, log_length, NULL, &log[0]);
		cout << "ERROR linking shader '" << &name[0] << "'" << endl << &log[0] << endl;
		return;
	}

	cout << "Program ID: " << program << endl;
	Shader::_program = program;
}


Shader::~Shader()
{
	glDeleteProgram(_program);
	cout << "Deleting '" << &Shader::name[0] << "' shader." << endl;
}

string Shader::ReadShader(string file_name)
{
	string source_code;
	ifstream file(file_name, ios::in);

	//Chech if file is accessable
	if (!file.good())
	{
		cout << "Cannot read source file '" << &file_name[0] << "'" << endl;
		terminate();
	}

	//Resize to fit content
	file.seekg(0, ios::end);
	source_code.resize((unsigned int)file.tellg());
	file.seekg(0, ios::beg);

	file.read(&source_code[0], source_code.size());
	file.close(); 

	return source_code;
}

GLuint Shader::CreateShader(GLenum shader_type, string source, string shader_name)
{
	int compilation_result = 0;

	GLuint shader_ID = glCreateShader(shader_type);
	const char* shader_ptr = source.c_str();
	const int shader_size = source.size();

	//Attempt to compile shader
	glShaderSource(shader_ID, 1, &shader_ptr, &shader_size);
	glCompileShader(shader_ID);
	glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &compilation_result);

	//Log if compilation failed
	if (compilation_result == GL_FALSE) 
	{
		int log_length = 0;
		glGetShaderiv(shader_ID, GL_INFO_LOG_LENGTH, &log_length);

		vector<char> log(log_length);
		glGetShaderInfoLog(shader_ID, log_length, NULL, &log[0]);
		cout << "ERROR compiling shader '" << &shader_name[0] << "'" << endl << &log[0] << endl;
		return 0;
	}

	return shader_ID;
}

void Shader::Render() 
{
	PrepareShader();

	for (map<const TexturedModel*, list<Element3D*>*>::iterator it = render_queue.queue.begin(); it != render_queue.queue.end(); it++)
	{
		TexturedModel* textured_model = (TexturedModel*)it->first;
		PrepareModel(textured_model);

		list<Element3D*>* element_list = render_queue.queue[textured_model];
		for (Element3D* element : *element_list)
		{
			PrepareInstance(element);
			glDrawElements(GL_TRIANGLES, textured_model->model->getIndiceCount(), GL_UNSIGNED_INT, 0);
		}
	}
}
