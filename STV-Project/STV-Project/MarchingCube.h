#pragma once
#include "MemoryListener.h"
#include "BlockGlossary.h"
#include "TerrainTools.h"
#include "VoxelBuilder.h"
#include <array>


class MarchingCube : public VoxelBuilder
{
public:
	MarchingCube() 
	{
		for (vector<float> values : _case_vertices)
		{
			_case_normals.push_back(values);
			for (float& f : _case_normals[_case_normals.size() - 1])
				f = 0;

			_case_uvs.push_back(vector<float>{0, 0});
		}
		Init();
	}

	void GenerateParts();

protected:
	void RotateAndSetData(ModelData& model_data, unsigned int x, unsigned int y, unsigned int z, bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6, bool v7);
	void RotateAndSetData(unsigned int state_case, bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6, bool v7);
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
	//Ambiguous cases appear appeneded to the end in the order they appear here http://users.polytech.unice.fr/~lingrand/MarchingCubes/resources/MCAmb.gif
	//14 Cases: +ve heavy, where cube is defined from (0,0,0) to (1,1,1)
	//Defined treating reds as solid
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
		{ e1,e5,e0, e8,e11,e4, e10,e9,e6 },//v1,v4,v6 --7
		{ e4,e5,e6,e7 },//v0,v1,v2,v3 --8
		{ e0,e1,e4,e6,e11,e10 },//v0,v2,v3,v7 --9
		{ e0,e3,e8,e11, e2,e1,e9,e10 },//v0,v2,v4,v6 --10
		{ e0,e4,e1,e7,e10,e9 },//v0,v3,v2,v6 --11
		{ e0,e3,e5,e7,e6, e11,e8,e4 },//v1,v2,v3,v4 --12
		{ e0,e4,e3, e5,e9,e8, e1,e2,e6, e11,e10,e7 },//v0,v2,v7,v5 --13
		{ e0,e3,e11,e5,e6,e10 },//v1,v2,v3,v7 --14

		//Ambiguous cases
		{ e0,e3,e4,e8,e5,e9 },//v1,v2,v3,v4,v6,v7 --3c (15)
		{ e4,e3,e1,e5,e10,e9,e6 },//v3,v2,v4,v5,v7 --6c (16)
		{ e0,e4,e11,e10,e6,e1, e5,e8,e9 },//v0,v3,v2,v7,v5 -- 7c (17)
		{ e0,e1,e8,e9, e3,e2,e10,e11 }, //v1,v5,v3,v7 --10c (18)
		{ e0,e4,e3, e7,e11,e8,e5,e6 }, //v0,v7,v6,v5 --12c(19)
		{ e0,e4,e3, e2,e6,e1, e11,e10,e7, e9,e8,e5 }, //v0,v2,v7,v5 --13c(20)
	};

	vector<vector<float>> _case_normals;
	vector<vector<float>> _case_uvs;

	const vector<vector<unsigned int>> _case_indices
	{
		{},//None
		{ 0,1,2 },//v0 --1
		{ 1,3,2, 1,2,0 },//v0,v1 --2
		{ 0,1,2,  3,4,5 },//v0,v5 --3
		{ 0,1,2,  5,4,3 },//v0,v6 --4
		{ 0,1,3, 0,3,2, 2,3,4 },//v3,v1,v2 --5
		{ 1,3,2, 1,2,0,  6,5,4 },//v0,v1,v6 --6
		{ 0,1,2,  3,4,5,  6,7,8 },//v1,v4,v6 --7
		{ 3,2,1, 3,1,0 },//v0,v1,v2,v3 --8
		{ 0,3,1, 0,5,3, 2,5,0, 2,4,5 },//v0,v2,v3,v7 --9
		{ 0,2,1, 2,3,1,  4,7,6, 4,6,5 },//v0,v2,v4,v6 --10
		{ 0,1,3, 0,3,5, 0,5,2, 3,4,5 },//v0,v3,v2,v6 --11
		{ 0,1,3, 0,3,2, 2,3,4,  6,5,7 },//v1,v2,v3,v4 --12
		{ 0,1,2,  3,4,5,  7,8,6,  9,10,11 },//v0,v2,v7,v5 --13
		{ 0,1,2, 0,2,4, 2,5,4, 4,3,0 },//v1,v2,v3,v7 --14

		//Ambiguous cases
		{ 2,3,1, 1,3,5, 1,4,0, 1,5,4 },//v1,v2,v3,v4,v6,v7 --3c (15)
		{ 3,5,4, 3,4,0, 0,4,1, 1,4,2, 2,4,6 },//v3,v2,v4,v5,v7 --6c (16)
		{ 0,1,2, 0,2,3, 0,3,5, 5,3,4,  6,8,7 },//v0,v3,v2,v7,v5 -- 7c (17)
		{ 0,1,2, 1,3,2,  4,7,5, 5,7,6 }, //v1,v5,v3,v7 --10c (18)
		{ 0,1,2,  3,4,5, 3,5,6, 3,6,7 }, //v0,v7,v6,v5 --12c(19)
		{ 0,1,2,  3,4,5,  6,7,8,  9,10,11 }, //v0,v2,v7,v5 --13c(20)
	};

};

