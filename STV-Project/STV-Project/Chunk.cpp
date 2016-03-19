#include "Chunk.h"
#include "GameManager.h"

Chunk::Chunk(Terrain* terrain, int x, int z) : _parent(terrain),
	VoxelMesh(Vectori(CHUNK_SIZE, CHUNK_MAX_HEIGHT, CHUNK_SIZE), Vectori(x, 0, z))
{

}

Chunk::~Chunk()
{
}

void Chunk::BuildTerrain()
{
	Generate();
	BuildModel();
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

