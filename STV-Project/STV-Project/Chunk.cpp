#include "Chunk.h"
#include "GameManager.h"
#include "Dependencies\glew\glew.h"

Chunk::Chunk(Terrain* terrain, int x, int z) : _parent(terrain),
	VoxelMesh(Vectori(CHUNK_SIZE, CHUNK_MAX_HEIGHT, CHUNK_SIZE), Vectori(x, 0, z))
{

}

Chunk::~Chunk()
{
	delete _debug_model;
}

void Chunk::BuildTerrain()
{
	//Generate();

	//*
	//TEST//////////////////////
	for (int x = 0; x < MESH_SIZE.x; x++)
		for (int z = 0; z < MESH_SIZE.z; z++) 
		{
			SetBlockAt(x, CHUNK_MAX_HEIGHT - 1, z, BLOCK_GRASS);
			SetBlockAt(x, 0, z, BLOCK_GRASS);
		}

	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
			for (int z = 0; z < 3; z++)
				SetBlockAt(1 + x, 64 + y, 1 + z, BLOCK_GRASS);


	SetBlockAt(2, 30, 1, BLOCK_GRASS);
	SetBlockAt(1, 31, 2, BLOCK_GRASS);
	SetBlockAt(2, 31, 2, BLOCK_GRASS);
	SetBlockAt(1, 31, 1, BLOCK_GRASS);
	SetBlockAt(1, 30, 2, BLOCK_GRASS);


	SetBlockAt(1, 40, 2, BLOCK_GRASS);

	/*
	SetBlockAt(8, 34, 8, BLOCK_GRASS);
	SetBlockAt(8, 34, 9, BLOCK_GRASS);
	SetBlockAt(9, 34, 8, BLOCK_GRASS);
	SetBlockAt(9, 33, 8, BLOCK_GRASS);
	SetBlockAt(9, 33, 9, BLOCK_GRASS);

	SetBlockAt(9, 37, 8, BLOCK_GRASS);
	SetBlockAt(9, 37, 9, BLOCK_GRASS);
	SetBlockAt(8, 37, 9, BLOCK_GRASS);
	SetBlockAt(8, 36, 8, BLOCK_GRASS);
	SetBlockAt(8, 36, 9, BLOCK_GRASS);

	SetBlockAt(8, 28, 9, BLOCK_GRASS);
	SetBlockAt(9, 28, 9, BLOCK_GRASS);
	SetBlockAt(9, 28, 8, BLOCK_GRASS);
	SetBlockAt(8, 27, 8, BLOCK_GRASS);
	SetBlockAt(9, 27, 8, BLOCK_GRASS);*/
	///////////////////////////*/

	BuildModel();
	if(_DEBUG) GenerateDebugModel();
}

void Chunk::GenerateDebugModel()
{
	vector<float> verts{};
	vector<float> uvs{};
	vector<float> normals{};
	vector<unsigned int> indices{};

	unsigned int index_track = 0; 
	const Vectori offset = MESH_SIZE * MESH_OFFSET;

	for (int x = 0; x < MESH_SIZE.x; x++)
		for (int y = 0; y < MESH_SIZE.y; y++)
			for (int z = 0; z < MESH_SIZE.z; z++)
			{
				if (GetBlockAt(x, y, z) != BLOCK_AIR) {
					if (GetBlockAt(x + 1, y, z) == BLOCK_AIR)
						AddDebugPanel(x + offset.x, y + offset.y, z + offset.z, 1, 0, 0, verts, uvs, normals, indices, index_track);

					if (GetBlockAt(x - 1, y, z) == BLOCK_AIR)
						AddDebugPanel(x + offset.x, y + offset.y, z + offset.z, -1, 0, 0, verts, uvs, normals, indices, index_track);

					if (GetBlockAt(x, y + 1, z) == BLOCK_AIR)
						AddDebugPanel(x + offset.x, y + offset.y, z + offset.z, 0, 1, 0, verts, uvs, normals, indices, index_track);

					if (GetBlockAt(x, y - 1, z) == BLOCK_AIR)
						AddDebugPanel(x + offset.x, y + offset.y, z + offset.z, 0, -1, 0, verts, uvs, normals, indices, index_track);

					if (GetBlockAt(x, y, z + 1) == BLOCK_AIR)
						AddDebugPanel(x + offset.x, y + offset.y, z + offset.z, 0, 0, 1, verts, uvs, normals, indices, index_track);

					if (GetBlockAt(x, y, z - 1) == BLOCK_AIR)
						AddDebugPanel(x + offset.x, y + offset.y, z + offset.z, 0, 0, -1, verts, uvs, normals, indices, index_track);
				}
			}


	_debug_model = GameManager::getMain()->model_loader->CreateModel(verts, uvs, normals, indices);
	_debug_model->polygon_mode = GL_LINE;
}

void Chunk::Generate()
{
	for (int x = 0; x < CHUNK_SIZE; x++)
		for (int z = 0; z < CHUNK_SIZE; z++)
		{
			int height = GetHeight(x,z);

			for (int y = height; y >= 0; y--){
				int cave_noise = GetCaveChance(x, y, z);

				if (CHUNK_CAVE_SIZE < cave_noise)
					SetBlockAt(x, y, z, BLOCK_GRASS);
			}

		}
}

int Chunk::GetHeight(int x, int z)
{
	x += MESH_OFFSET.x * CHUNK_SIZE;
	z += MESH_OFFSET.z * CHUNK_SIZE;
	return floorf(GetSmoothNoise(x, z, 100.0f, 50) + CHUNK_MAX_HEIGHT / 2);
}

int Chunk::GetCaveChance(int x, int y, int z)
{
	x += MESH_OFFSET.x * CHUNK_SIZE;
	z += MESH_OFFSET.z * CHUNK_SIZE;
	return floorf(GetSmoothNoise(x, y, z, 100.0f, 30) + CHUNK_MAX_HEIGHT / 2);
}

float Chunk::GetRawNoise(int x, int y, int z, float frequency)
{	
	srand(_parent->GetSeed() + (x * frequency * 1.1 + y* frequency * 1.0 + z *frequency * 1.2));
	rand(); //Throw away

	float value = rand() % CHUNK_SURFACE_HEIGHT - CHUNK_SURFACE_HEIGHT / 2;
	return value;
}

float Chunk::GetNoise(int x, int y, float frequency, int smoothness)
{
	int x_whole = floorf((float)(x) / (float)(smoothness));
	int y_whole = floorf((float)(y) / (float)(smoothness));
	float x_fract = (float)(x) / (float)(smoothness)-x_whole;
	float y_fract = (float)(y) / (float)(smoothness)-y_whole;

	float v1 = GetRawNoise(x_whole, y_whole, 0, frequency);
	float v2 = GetRawNoise(x_whole + 1, y_whole, 0, frequency);
	float v3 = GetRawNoise(x_whole, y_whole + 1, 0, frequency);
	float v4 = GetRawNoise(x_whole + 1, y_whole + 1, 0, frequency);

	float h1 = CosineInterpolate(v1, v2, x_fract);
	float h2 = CosineInterpolate(v3, v4, x_fract);

	return CosineInterpolate(h1, h2, y_fract);
}

float Chunk::GetNoise(int x, int y, int z, float frequency, int smoothness)
{
	int x_whole = floorf((float)(x) / (float)(smoothness));
	int y_whole = floorf((float)(y) / (float)(smoothness));
	int z_whole = floorf((float)(z) / (float)(smoothness));
	float x_fract = (float)(x) / (float)(smoothness)-x_whole;
	float y_fract = (float)(y) / (float)(smoothness)-y_whole;
	float z_fract = (float)(z) / (float)(smoothness)-z_whole;

	float v000 = GetRawNoise(x_whole, y_whole, z_whole, frequency);
	float v100 = GetRawNoise(x_whole + 1, y_whole, z_whole, frequency);
	float v010 = GetRawNoise(x_whole, y_whole + 1, z_whole, frequency);
	float v110 = GetRawNoise(x_whole + 1, y_whole + 1, z_whole, frequency);

	float v001 = GetRawNoise(x_whole, y_whole, z_whole + 1, frequency);
	float v101 = GetRawNoise(x_whole + 1, y_whole, z_whole + 1, frequency);
	float v011 = GetRawNoise(x_whole, y_whole + 1, z_whole + 1, frequency);
	float v111 = GetRawNoise(x_whole + 1, y_whole + 1, z_whole + 1, frequency);

	float v1 = CosineInterpolate(v000, v001, z_fract);
	float v2 = CosineInterpolate(v100, v101, z_fract);
	float v3 = CosineInterpolate(v010, v011, z_fract);
	float v4 = CosineInterpolate(v110, v111, z_fract);

	float h1 = CosineInterpolate(v1, v2, x_fract);
	float h2 = CosineInterpolate(v3, v4, x_fract);

	return CosineInterpolate(h1, h2, y_fract);
}


float Chunk::GetSmoothNoise(int x, int y, float frequency, int smoothness)
{
	return GetNoise(x, y, frequency, smoothness) + GetNoise(x, y, frequency, smoothness / 2) / 4.0f + GetNoise(x, y, frequency, smoothness / 4) / 8.0f;
}

float Chunk::GetSmoothNoise(int x, int y, int z, float frequency, int smoothness)
{
	return GetNoise(x, y, z, frequency, smoothness) + GetNoise(x, y, z, frequency, smoothness / 2) / 4.0f + GetNoise(x, y, z, frequency, smoothness / 4) / 8.0f;
}

void Chunk::AddDebugPanel(int x, int y, int z, int x_point, int y_point, int z_point, vector<float>& verts, vector<float>& uvs, vector<float>& normals, vector<unsigned int>& indices, unsigned int& index_track)
{
	const float SQRT_3 = sqrt(3);

	for (int a_sign = -1; a_sign <= 1; a_sign += 2)
		for (int b_sign = -1; b_sign <= 1; b_sign += 2)
		{
			if (x_point != 0)
			{
				verts.push_back(x + 0.5f * x_point);
				verts.push_back(y + 0.5f * a_sign);
				verts.push_back(z + 0.5f * b_sign);
				normals.push_back(1.0f / SQRT_3 * x_point);
				normals.push_back(1.0f / SQRT_3 * a_sign);
				normals.push_back(1.0f / SQRT_3 * b_sign);
			}
			if (y_point != 0)
			{
				verts.push_back(x + 0.5f * a_sign);
				verts.push_back(y + 0.5f * y_point);
				verts.push_back(z + 0.5f * b_sign);
				normals.push_back(1.0f / SQRT_3 * a_sign);
				normals.push_back(1.0f / SQRT_3 * y_point);
				normals.push_back(1.0f / SQRT_3 * b_sign);
			}
			if (z_point != 0)
			{
				verts.push_back(x + 0.5f * a_sign);
				verts.push_back(y + 0.5f * b_sign);
				verts.push_back(z + 0.5f * z_point);
				normals.push_back(1.0f / SQRT_3 * a_sign);
				normals.push_back(1.0f / SQRT_3 * b_sign);
				normals.push_back(1.0f / SQRT_3 * z_point);
			}

		}
	uvs.push_back(0.0);
	uvs.push_back(1.0);

	uvs.push_back(0.0);
	uvs.push_back(0.0);

	uvs.push_back(1.0);
	uvs.push_back(1.0);

	uvs.push_back(1.0);
	uvs.push_back(0.0);


	if (x_point > 0 || y_point < 0 || z_point > 0)
	{
		indices.push_back(index_track);
		indices.push_back(index_track + 3);
		indices.push_back(index_track + 1);

		indices.push_back(index_track + 2);
		indices.push_back(index_track + 3);
		indices.push_back(index_track);
	}
	else
	{
		indices.push_back(index_track + 1);
		indices.push_back(index_track + 3);
		indices.push_back(index_track);

		indices.push_back(index_track);
		indices.push_back(index_track + 3);
		indices.push_back(index_track + 2);
	}


	index_track += 4;
}