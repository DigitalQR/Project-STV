#pragma once
#include "EllipsoidBody.h"
#include "TriMesh.h"

namespace EllipsoidSolver 
{
	struct UnitBody 
	{
	private:
		vec3 last_normal_velocity;
		vec3 _normal_velocity;

	public:
		vec3 location;
		vec3 velocity;

		UnitBody(EllipsoidBody& body)
		{
			location = body.parent->location;
			velocity = body.velocity;

			location /= body.GetSize();
			velocity /= body.GetSize();
		}

		vec3& GetNormalizedVelocity() 
		{
			if (last_normal_velocity != velocity)
			{
				_normal_velocity = normalize(velocity);
				last_normal_velocity = velocity;
			}

			return _normal_velocity;
		}

		void UpdateVelocity(EllipsoidBody& body) 
		{
			body.velocity = velocity * body.GetSize();
			body.parent->location += body.velocity;
		}

		bool collision_found = false;
		float nearest_distance;
		vec3 intersection_point;
	
	};


	float SquaredLength(vec3& v) 
	{
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}


	bool GetLowestRoot(float a, float b, float c, float max_root, float* root) 
	{
		float determinant = b*b - 4.0f * a * c;
		if (determinant < 0.0f)
			return false;

		float sqrt_d = sqrt(determinant);
		float r1 = (-b - sqrt_d) / 2 * a;
		float r2 = (-b + sqrt_d) / 2 * a;

		if (r1 > r2) 
		{
			float temp = r2;
			r2 = r1;
			r1 = temp;
		}

		if (r1 > 0 && r1 < max_root) 
		{
			*root = r1;
			return true;
		}

		if (r2 > 0 && r2 < max_root) 
		{
			*root = r2;
			return true;
		}

		return false;
	}


	void SolveCase(UnitBody& u_body, Triangle& triangle)
	{
		//Get intersection interval
		float t0, t1;
		bool embedded_in_plane = false;

		float signed_distance_to_plane = triangle.SignedDistanceToTrisPlane(u_body.location);
		float normal_dot_velocity = dot(triangle.GetNormal(), u_body.velocity);
		

		//Sphere is travelling parrallel to plane
		if (normal_dot_velocity == 0.0f)
		{
			//Sphere is not embedded
			if (fabs(signed_distance_to_plane) >= 1.0f)
				return;

			//Sphere is embedded
			else
			{
				embedded_in_plane = true;
				t0 = 0.0f;
				t1 = 1.0f;
			}
		}

		//Sphere is NOT travelling parrallel to plane
		else
		{
			t0 = (-1.0f - signed_distance_to_plane) / normal_dot_velocity;
			t1 = (1.0f - signed_distance_to_plane) / normal_dot_velocity;
			
			if (t0 > t1)
			{
				float temp = t0;
				t0 = t1;
				t1 = temp;
			}

			if (t0 > 1.0f || t1 < 0.0f)
				return;

			//Clamp
			if (t0 < 0.0f) t0 = 0.0f;
			if (t1 < 0.0f) t1 = 0.0f;
			if (t0 > 1.0f) t0 = 1.0f;
			if (t1 > 1.0f) t1 = 1.0f;

		}

		vec3 collision_point;
		bool collision_found = false;
		float t = 1.0f;

		//Check if collision is inside triangle
		if (!embedded_in_plane)
		{
			vec3 plane_intersection_point = (u_body.location - triangle.GetNormal() * signed_distance_to_plane) + t0 * u_body.velocity;

			if (triangle.IsPointInside(plane_intersection_point))
			{
				collision_found = true;
				t = t0;
				collision_point = plane_intersection_point;
			}
		}

		//Check against points
		if (!collision_found) 
		{
			vec3& velocity = u_body.velocity;
			vec3& location = u_body.location;
			float velocity_squared_length = SquaredLength(velocity);
			float a, b, c;
			float new_t;

			a = velocity_squared_length;

			//Triangle point a
			b = 2.0f * dot(velocity, location - triangle.a);
			c = SquaredLength(triangle.a - location) - 1.0f;

			if (GetLowestRoot(a, b, c, t, &new_t))
			{
				t = new_t;
				collision_found = true;
				collision_point = triangle.a;
			}

			//Triangle point b
			b = 2.0f * dot(velocity, location - triangle.b);
			c = SquaredLength(triangle.b - location) - 1.0f;

			if (GetLowestRoot(a, b, c, t, &new_t))
			{
				t = new_t;
				collision_found = true;
				collision_point = triangle.b;
			}

			//Triangle point c
			b = 2.0f * dot(velocity, location - triangle.c);
			c = SquaredLength(triangle.c - location) - 1.0f;

			if (GetLowestRoot(a, b, c, t, &new_t))
			{
				t = new_t;
				collision_found = true;
				collision_point = triangle.c;
			}

			//Check edges
			if (!collision_found) 
			{
				//a-b
				vec3 edge = triangle.b - triangle.a;
				vec3 base_to_vertex = triangle.a - u_body.location;

				float edge_squared_length = SquaredLength(edge);
				float edge_dot_velocity = dot(edge, velocity);
				float edge_dot_base_to_vertex = dot(edge, base_to_vertex);

				a = edge_squared_length * -velocity_squared_length + edge_dot_velocity * edge_dot_base_to_vertex;
				b = edge_squared_length * (2.0f * dot(velocity, base_to_vertex)) - 2.0f * edge_dot_velocity * edge_dot_base_to_vertex;
				c = edge_squared_length * (1.0f - SquaredLength(base_to_vertex)) + edge_dot_base_to_vertex * edge_dot_base_to_vertex;

				if (GetLowestRoot(a, b, c, t, &new_t))
				{
					//Check if intersection is inside line segment
					float f = (edge_dot_velocity * new_t - edge_dot_base_to_vertex) / edge_squared_length;

					if (f >= 0.0f && f <= 1.0f)
					{
						t = new_t;
						collision_found = true;
						collision_point = triangle.a + f*edge;
					}
				}

				//b-c
				edge = triangle.c - triangle.b;
				base_to_vertex = triangle.b - u_body.location;

				edge_squared_length = SquaredLength(edge);
				edge_dot_velocity = dot(edge, velocity);
				edge_dot_base_to_vertex = dot(edge, base_to_vertex);

				a = edge_squared_length * -velocity_squared_length + edge_dot_velocity * edge_dot_base_to_vertex;
				b = edge_squared_length * (2.0f * dot(velocity, base_to_vertex)) - 2.0f * edge_dot_velocity * edge_dot_base_to_vertex;
				c = edge_squared_length * (1.0f - SquaredLength(base_to_vertex)) + edge_dot_base_to_vertex * edge_dot_base_to_vertex;

				if (GetLowestRoot(a, b, c, t, &new_t))
				{
					//Check if intersection is inside line segment
					float f = (edge_dot_velocity * new_t - edge_dot_base_to_vertex) / edge_squared_length;

					if (f >= 0.0f && f <= 1.0f)
					{
						t = new_t;
						collision_found = true;
						collision_point = triangle.b + f*edge;
					}
				}

				//c-a
				edge = triangle.a - triangle.c;
				base_to_vertex = triangle.c - u_body.location;

				edge_squared_length = SquaredLength(edge);
				edge_dot_velocity = dot(edge, velocity);
				edge_dot_base_to_vertex = dot(edge, base_to_vertex);

				a = edge_squared_length * -velocity_squared_length + edge_dot_velocity * edge_dot_base_to_vertex;
				b = edge_squared_length * (2.0f * dot(velocity, base_to_vertex)) - 2.0f * edge_dot_velocity * edge_dot_base_to_vertex;
				c = edge_squared_length * (1.0f - SquaredLength(base_to_vertex)) + edge_dot_base_to_vertex * edge_dot_base_to_vertex;

				if (GetLowestRoot(a, b, c, t, &new_t))
				{
					//Check if intersection is inside line segment
					float f = (edge_dot_velocity * new_t - edge_dot_base_to_vertex) / edge_squared_length;

					if (f >= 0.0f && f <= 1.0f)
					{
						t = new_t;
						collision_found = true;
						collision_point = triangle.c + f*edge;
					}
				}
			}
		}


		if (collision_found == true)
		{
			float distance_to_collision = t * u_body.velocity.length();

			if (!u_body.collision_found || distance_to_collision < u_body.nearest_distance)
			{
				u_body.nearest_distance = distance_to_collision;
				u_body.intersection_point = collision_point;
				u_body.collision_found = true;
			}
		}
	}


	void CheckAgainstMesh(UnitBody& u_body, TriMesh& mesh)
	{
		for (Triangle triangle : mesh.GetTriangles())
			//Only care about front facing tris
			if (triangle.IsFrontFacingTo(u_body.GetNormalizedVelocity()))
				SolveCase(u_body, triangle);
	}


	void ResolvePossibleCollisions(EllipsoidBody& original_body, TriMesh& mesh, unsigned int recursion_depth)
	{
		const float units_per_meter = 1.0f;
		const float unit_scale = units_per_meter / 100.0f;
		const float close_distance = 0.005f * unit_scale;

		if (recursion_depth == 0)
			return;
		recursion_depth--;


		//Reset
		UnitBody u_body = UnitBody(original_body);		
		CheckAgainstMesh(u_body, mesh);


		//No collision occured
		if (!u_body.collision_found)
		{
			u_body.UpdateVelocity(original_body);
			return;
		}


		//Collision occured
		vec3 destination = u_body.location + u_body.velocity;
		vec3 base = u_body.location;

		//To close (Give a small buffer)
		if (u_body.nearest_distance >= close_distance) 
		{
			vec3 v = u_body.velocity;
			normalize(v);

			base = u_body.location + v * (u_body.nearest_distance - close_distance);
			u_body.intersection_point -= close_distance * v;
		}

		//Calculating sliding
		vec3 slide_plane_origin = u_body.intersection_point;
		vec3 slide_plane_normal = normalize(base - u_body.intersection_point);

		Plane slide_plane(slide_plane_origin, slide_plane_normal);

		vec3 new_destination = destination - slide_plane_normal * slide_plane.SignedDistance(destination);
		vec3 new_velocity = new_destination - u_body.intersection_point;

		u_body.velocity = new_velocity;
		u_body.UpdateVelocity(original_body);

		if (new_velocity.length() < close_distance)
			return;
	}



	Triangle& ConvertToEllipsoidSpace(vec3& size, Triangle triangle)
	{
		return Triangle(
			triangle.a / size,
			triangle.b / size,
			triangle.c / size
			);
	}


	void ResolvedCollisions(EllipsoidBody& body, TriMesh mesh) 
	{

		//Convert mesh into elipsoid space
		for (Triangle& t : mesh.GetTriangles())
			t = ConvertToEllipsoidSpace(body.GetSize(), t);

		ResolvePossibleCollisions(body, mesh, 1);
	}
	
};
