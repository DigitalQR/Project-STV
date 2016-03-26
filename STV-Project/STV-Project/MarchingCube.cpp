#include "MarchingCube.h"

void MarchingCube::GenerateParts()
{
	SetAllData(1, 1, 0, 0, 0, 0, 0, 0, 0);
	SetAllData(2, 1, 1, 0, 0, 0, 0, 0, 0);
	SetAllData(3, 1, 0, 0, 0, 0, 1, 0, 0);
	SetAllData(4, 1, 0, 0, 0, 0, 0, 1, 0);
	SetAllData(5, 0, 1, 1, 1, 0, 0, 0, 0);
	SetAllData(6, 1, 1, 0, 0, 0, 0, 1, 0);
	SetAllData(7, 0, 1, 0, 0, 1, 0, 1, 0);
	SetAllData(8, 1, 1, 1, 1, 0, 0, 0, 0);
	SetAllData(9, 1, 0, 1, 1, 0, 0, 0, 1);
	SetAllData(10, 1, 0, 1, 0, 1, 0, 1, 0);
	SetAllData(11, 1, 0, 1, 1, 0, 0, 1, 0);
	SetAllData(12, 0, 1, 1, 1, 1, 0, 0, 0);
	SetAllData(13, 1, 0, 1, 0, 0, 1, 0, 1);
	SetAllData(14, 0, 1, 1, 1, 0, 0, 0, 1);
	SetAllData(15, 0, 0, 1, 1, 1, 1, 0, 1);
}


void MarchingCube::SetAllData(unsigned int state_case, bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6, bool v7)
{
	ModelData model_data = ModelData(_case_vertices[state_case], _case_uvs[state_case], _case_normals[state_case], _case_indices[state_case]);

	for (int xr = 0; xr < 7; xr++)
		for (int yr = 0; yr < 7; yr++)
			for (int zr = 0; zr < 7; zr++)
				RotateAndSetData(model_data, xr, yr, zr, v0, v1, v2, v3, v4, v5, v6, v7);

	model_data.Flip();
	for (int xr = 0; xr < 7; xr++)
		for (int yr = 0; yr < 7; yr++)
			for (int zr = 0; zr < 7; zr++)
				RotateAndSetData(model_data, xr, yr, zr, !v0, !v1, !v2, !v3, !v4, !v5, !v6, !v7);
}

void MarchingCube::RotateAndSetData(ModelData& model_data, unsigned int x, unsigned int y, unsigned int z, bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6, bool v7)
{
	bool t0 = v0;
	bool t1 = v1;
	bool t2 = v2;
	bool t3 = v3;
	bool t4 = v4;
	bool t5 = v5;
	bool t6 = v6;
	bool t7 = v7;
	ModelData new_model_data = model_data;
	new_model_data.Rotate(x, y, z, 0.5f, 0.5f, 0.5f);

	RotateIndices(x, y, z, t0, t1, t2, t3, t4, t5, t6, t7);
	SetData(new_model_data, t0, t1, t2, t3, t4, t5, t6, t7);
}

