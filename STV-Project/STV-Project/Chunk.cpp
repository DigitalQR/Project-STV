#include "Chunk.h"
#include "GameManager.h"

Chunk::Chunk(Terrain* terrain, int x, int z) : _parent(terrain), _X(x), _Z(z)
{

}

Chunk::~Chunk()
{
	delete _model;
}

void Chunk::BuildTerrain()
{
	Generate();
	BuildModel();
}

void Chunk::Generate()
{
	int cave_size = 10;

	for (int x = 0; x < CHUNK_SIZE; x++)
		for (int z = 0; z < CHUNK_SIZE; z++)
		{
			int height = GetHeight(x,z);

			for (int y = height; y >= 0; y--){
				int cave_noise = GetCaveChance(x, y, z);

				if(cave_size < cave_noise)
					_blocks[x][y][z] = BLOCK_GRASS;
			}

		}
}

int Chunk::GetHeight(int x, int z)
{
	x += _X * CHUNK_SIZE;
	z += _Z * CHUNK_SIZE;
	return floorf(GetSmoothNoise(x, z, 100.0f, 50) + CHUNK_MAX_HEIGHT / 2);
}

int Chunk::GetCaveChance(int x, int y, int z)
{
	x += _X * CHUNK_SIZE;
	z += _Z * CHUNK_SIZE;
	return floorf(GetSmoothNoise(x, y, z, 100.0f, 10) + CHUNK_MAX_HEIGHT / 2);
}

void Chunk::BuildModel()
{
	vector<float> verts{};
	vector<float> uvs{};
	vector<float> normals{};
	vector<unsigned int> indices{};

	unsigned int index_track = 0;

	const int X_OFFSET = _X * CHUNK_SIZE;
	const int Z_OFFSET = _Z * CHUNK_SIZE;

	for (int x = 0; x < CHUNK_SIZE; x++)
		for (int y = 0; y < CHUNK_MAX_HEIGHT; y++)
			for (int z = 0; z < CHUNK_SIZE; z++) 
			{

				if (GetBlockAt(x + X_OFFSET, y, z + Z_OFFSET) != BLOCK_AIR) {

					if (GetBlockAt(x + X_OFFSET + 1, y, z + Z_OFFSET) == BLOCK_AIR)
						AddToPanel(x + X_OFFSET, y, z + Z_OFFSET, 1, 0, 0, verts, uvs, normals, indices, index_track);

					if (GetBlockAt(x + X_OFFSET - 1, y, z + Z_OFFSET) == BLOCK_AIR)
						AddToPanel(x + X_OFFSET, y, z + Z_OFFSET, -1, 0, 0, verts, uvs, normals, indices, index_track);

					if (GetBlockAt(x + X_OFFSET, y + 1, z + Z_OFFSET) == BLOCK_AIR)
						AddToPanel(x + X_OFFSET, y, z + Z_OFFSET, 0, 1, 0, verts, uvs, normals, indices, index_track);

					if (GetBlockAt(x + X_OFFSET, y - 1, z + Z_OFFSET) == BLOCK_AIR)
						AddToPanel(x + X_OFFSET, y, z + Z_OFFSET, 0, -1, 0, verts, uvs, normals, indices, index_track);

					if (GetBlockAt(x + X_OFFSET, y, z + Z_OFFSET + 1) == BLOCK_AIR)
						AddToPanel(x + X_OFFSET, y, z + Z_OFFSET, 0, 0, 1, verts, uvs, normals, indices, index_track);

					if (GetBlockAt(x + X_OFFSET, y, z + Z_OFFSET - 1) == BLOCK_AIR)
						AddToPanel(x + X_OFFSET, y, z + Z_OFFSET, 0, 0, -1, verts, uvs, normals, indices, index_track);
					
				}
			}


	_model = GameManager::getMain()->model_loader->CreateModel(verts, uvs, normals, indices);
}

float Chunk::GetRawNoise(int x, int y, int z, float frequency)
{	
	srand(_parent->GetSeed() + x* frequency + y* frequency + z*frequency);
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


void Chunk::AddToPanel(int x, int y, int z, int x_point, int y_point, int z_point, vector<float>& verts, vector<float>& uvs, vector<float>& normals, vector<unsigned int>& indices, unsigned int& index_track)
{
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


