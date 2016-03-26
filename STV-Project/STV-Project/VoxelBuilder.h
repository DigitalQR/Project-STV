#pragma once
#include "MemoryListener.h"

#include "BlockGlossary.h"
#include "TerrainTools.h"
#include <array>

using namespace std;

class VoxelBuilder 
{
public:
	void Init() 
	{
		SetDefaults();
		GenerateParts();
	}

	virtual void GenerateParts() {};

	void BuildFaces(int X, int Y, int Z, array<array<array<block_id, 2>, 2>, 2>& states, ModelData& model_data) 
	{
		unsigned int state_count = 0;
		array<array<array<bool, 2>, 2>, 2> state_case{ 0 };

		for (int x = 0; x < 2; x++)
			for (int y = 0; y < 2; y++)
				for (int z = 0; z < 2; z++)
				{
					if (states[x][y][z] != BLOCK_AIR)
					{
						state_count++;
						state_case[x][y][z] = true;
					}
				}

		if (state_count == 0)
			return;

		ModelData data = model_parts
			[state_case[0][0][0]][state_case[1][0][0]][state_case[1][0][1]][state_case[0][0][1]]
			[state_case[0][1][0]][state_case[1][1][0]][state_case[1][1][1]][state_case[0][1][1]];

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


protected:
	array<array<array<array<array<array<array<array<ModelData, 2>, 2>, 2>, 2>, 2>, 2>, 2>, 2> model_parts;

	void SetDefaults()
	{
		for (bool v0 = false; !v0; v0 = true)
			for (bool v1 = false; !v1; v1 = true)
				for (bool v2 = false; !v2; v2 = true)
					for (bool v3 = false; !v3; v3 = true)
						for (bool v4 = false; !v4; v4 = true)
							for (bool v5 = false; !v5; v5 = true)
								for (bool v6 = false; !v6; v6 = true)
									for (bool v7 = false; !v7; v7 = true)
										model_parts[v0][v1][v2][v3][v4][v5][v6][v7] = ModelData();

	}

	void SetData(ModelData& model_data, bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6, bool v7)
	{
		model_parts[v0][v1][v2][v3][v4][v5][v6][v7] = model_data;
	}

};