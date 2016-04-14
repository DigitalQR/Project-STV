#pragma once
#include "MemoryListener.h"

#include "BlockGlossary.h"
#include "TerrainTools.h"
#include <array>

using namespace std;

class VoxelBuilder 
{
public:
	static const bool SMOOTHMODE = true;

	void Init() 
	{
		SetDefaults();
		GenerateParts();
	}

	virtual void GenerateParts() {};

	int GetIndex(bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6, bool v7) 
	{
		int i = 0;
		if (v7)
			i += 1;
		if (v6)
			i += 2;
		if (v5)
			i += 4;
		if (v4)
			i += 8;
		if (v3)
			i += 16;
		if (v2)
			i += 32;
		if (v1)
			i += 64;
		if (v0)
			i += 128;

		return i;
	}

	void BuildFaces(int X, int Y, int Z, array<array<array<Resource, 2>, 2>, 2>& states, ModelData& model_data)
	{
		unsigned int state_count = 0;
		array<array<array<bool, 2>, 2>, 2> state_case{ 0 };

		for (int x = 0; x < 2; x++)
			for (int y = 0; y < 2; y++)
				for (int z = 0; z < 2; z++)
				{
					if (IsSolid(states[x][y][z]))
					{
						state_count++;
						state_case[x][y][z] = true;
					}
				}

		if (state_count == 0 || state_count == 8)
			return;

		ModelData data = GetData(
			state_case[0][0][0], state_case[1][0][0], state_case[1][0][1], state_case[0][0][1],
			state_case[0][1][0], state_case[1][1][0], state_case[1][1][1], state_case[0][1][1]);

		data.ReplaceTextureIDs(states[0][0][0].resource, states[1][0][0].resource, states[1][0][1].resource, states[0][0][1].resource,
			states[0][1][0].resource, states[1][1][0].resource, states[1][1][1].resource, states[0][1][1].resource);


		if (SMOOTHMODE)
		{
				data.Smooth(states[0][0][0], states[1][0][0], states[1][0][1], states[0][0][1],
					states[0][1][0], states[1][1][0], states[1][1][1], states[0][1][1]);

			data.BuildNormals();
			data.BuildUVs();
		}


		data += Vectori(X, Y, Z);
		model_data += data;
	};

	void RotateIndices(unsigned int x, unsigned int y, unsigned int z, bool& v0, bool& v1, bool& v2, bool& v3, bool& v4, bool& v5, bool& v6, bool& v7)
	{
		bool t0, t1, t2, t3, t4, t5, t6, t7;

		while (x != 0)
		{
			t0 = v0;
			t1 = v1;
			t2 = v2;
			t3 = v3;
			t4 = v4;
			t5 = v5;
			t6 = v6;
			t7 = v7;

			v0 = t3;
			v1 = t2;
			v2 = t6;
			v3 = t7;
			v4 = t0;
			v5 = t1;
			v6 = t5;
			v7 = t4;

			x--;
		}
		while (y != 0)
		{
			t0 = v0;
			t1 = v1;
			t2 = v2;
			t3 = v3;
			t4 = v4;
			t5 = v5;
			t6 = v6;
			t7 = v7;

			v0 = t1;
			v1 = t2;
			v2 = t3;
			v3 = t0;
			v4 = t5;
			v5 = t6;
			v6 = t7;
			v7 = t4;

			y--;
		}
		while (z != 0)
		{
			t0 = v0;
			t1 = v1;
			t2 = v2;
			t3 = v3;
			t4 = v4;
			t5 = v5;
			t6 = v6;
			t7 = v7;

			v0 = t1;
			v1 = t5;
			v2 = t6;
			v3 = t2;
			v4 = t0;
			v5 = t4;
			v6 = t7;
			v7 = t3;
			z--;
		}
	}

	void MirrorIndices(bool x, bool y, bool z, bool& v0, bool& v1, bool& v2, bool& v3, bool& v4, bool& v5, bool& v6, bool& v7) 
	{
		bool t0, t1, t2, t3, t4, t5, t6, t7;
		if (x)
		{
			t0 = v0;
			t1 = v1;
			t2 = v2;
			t3 = v3;
			t4 = v4;
			t5 = v5;
			t6 = v6;
			t7 = v7;

			v0 = t1;
			v1 = t0;
			v2 = t3;
			v3 = t2;
			v4 = t5;
			v5 = t4;
			v6 = t7;
			v7 = t6;
		}
		if (y)
		{
			t0 = v0;
			t1 = v1;
			t2 = v2;
			t3 = v3;
			t4 = v4;
			t5 = v5;
			t6 = v6;
			t7 = v7;

			v0 = t4;
			v1 = t5;
			v2 = t6;
			v3 = t7;
			v4 = t0;
			v5 = t1;
			v6 = t2;
			v7 = t3;
		}
		if (z)
		{
			t0 = v0;
			t1 = v1;
			t2 = v2;
			t3 = v3;
			t4 = v4;
			t5 = v5;
			t6 = v6;
			t7 = v7;

			v0 = t3;
			v1 = t2;
			v2 = t1;
			v3 = t0;
			v4 = t7;
			v5 = t6;
			v6 = t5;
			v7 = t4;
		}
	}

protected:
	array<ModelData, 256> model_parts;

	void SetDefaults()
	{
		for (int i = 0; i < 256; i++)
			model_parts[i] = ModelData();
	}

	void SetData(ModelData& model_data, bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6, bool v7)
	{
		int index = GetIndex(v0, v1, v2, v3, v4, v5, v6, v7);

		if (model_parts[index].verts.size() != 0)
			return;
		
		model_data.BuildNormals();
		model_data.BuildUVs();
		model_parts[index] = model_data;
	}

	ModelData GetData(bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6, bool v7)
	{
		return model_parts[GetIndex(v0, v1, v2, v3, v4, v5, v6, v7)];
	}
};