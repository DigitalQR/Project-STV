#include "Chunk.h"
#include "GameManager.h"
#include "Dependencies\glew\glew.h"


Chunk::Chunk(Terrain* terrain, int x, int y, int z) : _parent(terrain),
	VoxelMesh(Vectori(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z), Vectori(x, y, z))
{
	Generate();
}

block_id Chunk::GetBlockAt(int x, int y, int z)
{
	if (x < 0 || y < 0 || z < 0 || x >= MESH_SIZE.x || y >= MESH_SIZE.y || z >= MESH_SIZE.z) 
	{
		const Vectori current_offset(
			MESH_OFFSET.x * MESH_SIZE.x,
			MESH_OFFSET.y * MESH_SIZE.y,
			MESH_OFFSET.z * MESH_SIZE.z
			);

		const Vectori new_chunk_coords = GetChunkCoordsOf(
			x + current_offset.x,
			y + current_offset.y,
			z + current_offset.z
			);

		const Vectori new_offset(
			new_chunk_coords.x * MESH_SIZE.x,
			new_chunk_coords.y * MESH_SIZE.y,
			new_chunk_coords.z * MESH_SIZE.z
			);

		Chunk& chunk = *_parent->GetChunkLoader()->GetChunk(new_chunk_coords.x, new_chunk_coords.y, new_chunk_coords.z);
		
		return chunk.GetBlockAt(
			x + current_offset.x - new_offset.x,
			y + current_offset.y - new_offset.y,
			z + current_offset.z - new_offset.z
			);
	}

	return VoxelMesh::GetBlockAt(x, y, z);
}

void Chunk::Generate()
{
	const int height_offset = MESH_OFFSET.y * MESH_SIZE.y;

	for (int x = 0; x < CHUNK_SIZE_X; x++)
		for (int z = 0; z < CHUNK_SIZE_Z; z++)
		{
			int height = GetHeight(x,z) - height_offset;

			//Height ends in current chunk or is above
			if(height < MESH_SIZE.y)
				for (int y = height; y >= 0; y--){
					int cave_noise = GetCaveChance(x, y + height_offset, z);

					if (GEN_CAVE_SIZE < cave_noise)
						SetBlockAt(x, y, z, BLOCK_GRASS);
				}
			//Height is above current chunk
			else
				for (int y = 0; y < MESH_SIZE.y; y++) {
					int cave_noise = GetCaveChance(x, y + height_offset, z);

					if (GEN_CAVE_SIZE < cave_noise)
						SetBlockAt(x, y, z, BLOCK_GRASS);
				}
		}
}

int Chunk::GetHeight(int x, int z)
{
	x += MESH_OFFSET.x * CHUNK_SIZE_X;
	z += MESH_OFFSET.z * CHUNK_SIZE_Z;
	return floorf(GetSmoothNoise(x, z, 100.0f, 50) + GEN_SURFACE_HEIGHT / 2);
}

int Chunk::GetCaveChance(int x, int y, int z)
{
	x += MESH_OFFSET.x * CHUNK_SIZE_X;
	z += MESH_OFFSET.z * CHUNK_SIZE_Z;
	return floorf(GetSmoothNoise(x, y, z, 100.0f, 30) + GEN_MAX_HEIGHT / 2);
}

float Chunk::GetRawNoise(int x, int y, int z, float frequency)
{	
	srand(_parent->GetSeed() + (x * frequency * 1.1 + y* frequency * 1.0 + z *frequency * 1.2));
	rand(); //Throw away

	float value = GEN_SURFACE_START + rand() % GEN_SURFACE_HEIGHT - GEN_SURFACE_HEIGHT / 2;
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