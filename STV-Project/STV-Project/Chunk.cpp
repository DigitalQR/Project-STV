#include "Chunk.h"
#include "GameManager.h"
#include "Dependencies\glew\glew.h"


Chunk::Chunk(Terrain* terrain, int x, int y, int z) : _parent(terrain),
	VoxelMesh(Vectori(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z), Vectori(x, y, z))
{
	Generate();
}

Resource& Chunk::GetResourceAt(int x, int y, int z)
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
		
		return chunk.GetResourceAt(
			x + current_offset.x - new_offset.x,
			y + current_offset.y - new_offset.y,
			z + current_offset.z - new_offset.z
			);
	}

	return VoxelMesh::GetResourceAt(x, y, z);
}

void Chunk::ForceSetResourceAt(int x, int y, int z, resource_id res) 
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

		chunk.SetResourceAt(
			x + current_offset.x - new_offset.x,
			y + current_offset.y - new_offset.y,
			z + current_offset.z - new_offset.z,
			res
			);
	}
	else
		SetResourceAt(x, y, z, res);
}


void Chunk::BuildHouse(int spawn_height)
{
	const int width = 11;
	const int length = 11;
	const int height = 10;


	for (int x = -width / 2 - 3; x < width / 2 + 3; x++)
		for (int y = -1; y < height / 2; y++)
			for (int z = -length / 2 - 3; z < length / 2 + 3; z++)
			{
				ForceSetResourceAt(x + 8, y + spawn_height, z + 8, RES_AIR);
			}

	for (int x = -width / 2; x < width / 2; x++)
		for (int y = -height / 2; y < height / 2; y++)
			for (int z = -length / 2; z < length / 2; z++)
			{
				if (y < 0)
					ForceSetResourceAt(x + 8, y + spawn_height, z + 8, RES_BRICKS);
				else
				{
					if( (x == -width / 2 && z == -length / 2) ||
						(x == -width / 2 && z == length / 2 - 1) ||
						(x == width / 2 - 1 && z == -length / 2) ||
						(x == width / 2 - 1 && z == length / 2 - 1)
						)
						ForceSetResourceAt(x + 8, y + spawn_height, z + 8, RES_BRICKS);
					else
						ForceSetResourceAt(x + 8, y + spawn_height, z + 8, RES_PLANKS);
				}
			}

	for (int x = -width / 2 + 1; x < width / 2 - 1; x++)
		for (int y = 0; y < height / 2 - 1; y++)
			for (int z = -length / 2 + 1; z < length / 2 - 1; z++)
			{
				ForceSetResourceAt(x + 8, y + spawn_height, z + 8, RES_AIR);
			}

	//Door
	ForceSetResourceAt(8 - width / 2, 0 + spawn_height, 8, RES_AIR);
	ForceSetResourceAt(8 - width / 2, 1 + spawn_height, 8, RES_AIR);
	ForceSetResourceAt(8 - width / 2, 0 + spawn_height, 7, RES_AIR);
	ForceSetResourceAt(8 - width / 2, 1 + spawn_height, 7, RES_AIR);

	const int roof_gradient = 2;
	//Roof
	for (int y = 0; y < width/2 + 1; y++)
	{
		for (int x = -(width-y * roof_gradient) / 2 - 2; x < (width-y * roof_gradient) / 2 + 2; x++)
			for (int z = -(length-y * roof_gradient) / 2 - 2; z < (length-y * roof_gradient) / 2 + 2; z++)
				ForceSetResourceAt(8 + x, height/2 + y + spawn_height, 8 + z, RES_THATCH);
	}
}


void Chunk::Generate()
{
	const int height_offset = MESH_OFFSET.y * MESH_SIZE.y;
	bool spawn_house = false;
	int house_height;

	for (int x = 0; x < CHUNK_SIZE_X; x++)
		for (int z = 0; z < CHUNK_SIZE_Z; z++)
		{
			int height = GetHeight(x,z);

			if (height - height_offset < MESH_SIZE.y && height - height_offset >= 0)
			{
				if (MESH_OFFSET.x == 0 && MESH_OFFSET.z == 0 && x == 8 && z == 8)
				{
					spawn_house = true;
					house_height = height - height_offset;
				}
			}

			//Height ends in current chunk or is above
			if (height - height_offset < MESH_SIZE.y)
			{
				for (int y = height - height_offset; y >= 0; y--)
					GeneratePoint(height, x, y, z);
			}
				
			//Height is above current chunk
			else
				for (int y = 0; y < MESH_SIZE.y; y++)
					GeneratePoint(height, x, y, z);
				
		}

	if (spawn_house)
		BuildHouse(house_height);
}

void Chunk::GeneratePoint(int height, int x, int y, int z) 
{
	const int height_offset = MESH_OFFSET.y * MESH_SIZE.y;

	if (GEN_CAVE_SIZE < GetCaveChance(x, y, z))
	{
		if (y + height_offset >= GEN_SURFACE_START + GEN_SNOW_HEIGHT)
			SetResourceAt(x, y, z, RES_SNOW);
		else if (y + height_offset > GEN_SURFACE_START + GEN_MOUNTAIN_HEIGHT)
			SetResourceAt(x, y, z, RES_STONE);
		else if (y + height_offset == GEN_SURFACE_START + GEN_MOUNTAIN_HEIGHT)
			SetResourceAt(x, y, z, RES_DIRT);

		else if (y + height_offset == height)
			SetResourceAt(x, y, z, RES_GRASS);
		else if (y + height_offset >= height - 3)
			SetResourceAt(x, y, z, RES_DIRT);
		
		//Add patches of ore in stone
		else if (GEN_ORE_SIZE >= Get3DChance(x, y, z, 90.0f, 10))
				SetResourceAt(x, y, z, RES_ORE);
		else
			SetResourceAt(x, y, z, RES_STONE);
	}
}

int Chunk::GetHeight(int x, int z)
{
	x += MESH_OFFSET.x * CHUNK_SIZE_X;
	z += MESH_OFFSET.z * CHUNK_SIZE_Z;

	//Correct to be in height range
	const float half_height = GEN_SURFACE_HEIGHT / 2.0f;

	return GEN_SURFACE_START + floorf(GetSmoothNoise(x, z, 100.0f, GEN_SMOOTHNESS) * half_height + half_height);
}

int Chunk::GetCaveChance(int x, int y, int z)
{
	return Get3DChance(x,y,z, 100.0f, 30);
}

int Chunk::Get3DChance(int x, int y, int z, float frequency, int smoothness) 
{
	x += MESH_OFFSET.x * CHUNK_SIZE_X;
	y += MESH_OFFSET.y * CHUNK_SIZE_Y;
	z += MESH_OFFSET.z * CHUNK_SIZE_Z;

	//Correct to be between 0-100
	return floorf(GetSmoothNoise(x, y, z, frequency, smoothness)*50 + 50);
}

float Chunk::MwC_Rand(int x, int y, int z, int seed)
{

	srand(seed + x * 287673 + y * 73610 + z * 94782);
	uint32_t w = rand();
	w << 16;
	w |= rand();
	return float(w) / float(RAND_MAX);
}


float Chunk::GetRawNoise(int x, int y, int z, float frequency)
{
	const float x_freq = frequency;
	const float y_freq = frequency;
	const float z_freq = frequency;

	x *= x_freq / 2;
	y *= y_freq / 2;
	z *= z_freq / 2;

	const float value = (MwC_Rand(x, y, z, _parent->GetSeed()) - 0.5f)* 2.0f;
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