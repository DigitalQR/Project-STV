#pragma once
#include "BlockGlossary.h"
#include "TerrainTools.h"
#include <array>

class MarchingCube {

public:
	static MarchingCube* getMain()
	{
		static MarchingCube* MAIN = new MarchingCube();
		return MAIN;
	}

	MarchingCube()
	{
		for (vector<float> values : _case_vertices)
		{
			_case_normals.push_back(values);
			for (float& f : _case_normals[_case_normals.size() - 1])
				f -= 0.5f;

			_case_uvs.push_back(vector<float>{0, 0});
		}
		GenerateParts();
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

	void GenerateParts();
	void BuildFaces(int x, int y, int z, array<array<array<block_id, 2>, 2>, 2>& states, ModelData& model_data);

protected:
	void SetData(ModelData& model_data, bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6, bool v7);
	void RotateAndSetData(ModelData& model_data, unsigned int x, unsigned int y, unsigned int z, bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6, bool v7);
	void SetAllData(unsigned int state_case, bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6, bool v7);

private:
#define e0	0.5f,0.0f,0.0f
#define e1	1.0f,0.0f,0.5f
#define e2	0.5f,0.0f,1.0f
#define e3	0.0f,0.0f,0.5f
#define e4	0.0f,0.5f,0.0f
#define e5	1.0f,0.5f,0.0f
#define e6	1.0f,0.5f,1.0f
#define e7	0.0f,0.5f,1.0f
#define e8	0.5f,1.0f,0.0f
#define e9	1.0f,1.0f,0.5f
#define e10 0.5f,1.0f,1.0f
#define e11 0.0f,1.0f,0.5f
	//Cases appear in order as seen by http://users.polytech.unice.fr/~lingrand/MarchingCubes/resources/MCcases.gif
	//14 Cases: +ve heavy, where cube is defined from (0,0,0) to (1,1,1)
	//                 v7__________e10_________v6 (1,1,1)
	//                  /|                    /|
	//              e11/ |                e9 / |
	//                /  |                  /  |
	//               /___|______e8_________/   |
	//    (0,1,0) v4|    |e7               |v5 |e6
	//              |    |                 |   |
	//              |    |                 |   |
	//              |    |                 |e5 |
	//              |e4  |__________e2_____|___|
	//              |   / v3 (0,0,1)       |   /v2
	//              |  /e3                 |  /
	//              | /                    | /e1
	//              |/__________e0_________|/
	//              v0 (0,0,0)              v1 (1,0,0)

	const vector<vector<float>> _case_vertices
	{
		{},//None
		{ e0,e4,e3 },//v0 --1
		{ e1,e5,e3,e4 },//v0,v1 --2
		{ e0,e4,e3, e5,e9,e8 },//v0,v5 --3
		{ e0,e4,e3, e9,e10,e6 },//v0,v6 --4
		{ e0,e3,e5,e7,e6 },//v3,v1,v2 --5
		{ e1,e5,e3,e4, e9,e10,e6 },//v0,v1,v6 --6
		{ e1,e5,e0, e8,e11,e4, e9,e10,e6 },//v1,v4,v6 --7
		{ e4,e5,e6,e7 },//v0,v1,v2,v3 --8
		{ e0,e1,e4,e6,e11,e10 },//v0,v2,v3,v7 --9
		{ e0,e3,e8,e11, e2,e1,e9,e10 },//v0,v2,v4,v6 --10
		{ e0,e4,e1,e7,e10,e9 },//v0,v3,v2,v6 --11
		{ e0,e3,e5,e7,e6, e11,e8,e4 },//v1,v2,v3,v4 --12
		{ e0,e4,e3, e5,e9,e8, e1,e2,e6, e11,e10,e7 },//v0,v2,v7,v5 --13
		{ e0,e3,e11,e5,e6,e10 },//v1,v2,v3,v7 --14
	};

	vector<vector<float>> _case_normals;
	vector<vector<float>> _case_uvs;

	const vector<vector<unsigned int>> _case_indices
	{
		{},//None
		{ 0,1,2 },//v0
		{ 1,3,2, 1,2,0 },//v0,v1
		{ 0,1,2,  3,4,5 },//v0,v5
		{ 0,1,2,  5,4,3 },//v0,v6
		{ 0,1,3, 0,3,2, 2,3,4 },//v3,v1,v2
		{ 1,3,2, 1,2,0,  6,5,4 },//v0,v1,v6
		{ 0,1,2,  3,4,5,  6,7,8 },//v1,v4,v6
		{ 3,2,1, 3,1,0 },//v0,v1,v2,v3
		{ 0,3,1, 0,5,3, 2,5,0, 2,4,5 },//v0,v2,v3,v7
		{ 0,2,1, 2,3,1,  4,7,6, 4,6,5 },//v0,v2,v4,v6
		{ 0,1,3, 0,3,5, 0,2,5, 3,5,4 },//v0,v3,v2,v6
		{ 0,1,3, 0,3,2, 2,3,4,  6,5,7 },//v1,v2,v3,v4
		{ 0,1,2,  3,4,5,  6,8,7,  9,10,11 },//v0,v2,v7,v5
		{ 0,1,2, 0,2,4, 0,4,3, 2,5,4 },//v1,v2,v3,v7
	};

	array<array<array<array<array<array<array<array<ModelData, 2>, 2>, 2>, 2>, 2>, 2>, 2>, 2> _model_parts;
};

