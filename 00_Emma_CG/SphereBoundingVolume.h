#pragma once
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <vector>

class SphereBoundingVolume
{
public:
	SphereBoundingVolume() : centre(0), radius(0) {}
	~SphereBoundingVolume() = default;

	template<typename T>
	void fit(const std::vector<T>& verts)
	{
		glm::vec3 min(1e37f);
		glm::vec3 max(-1e37f);
		for (auto& vert : verts)
		{
			glm::vec3 p = glm::vec3(vert.position);
			if (p.x < min.x) min.x = p.x;
			if (p.y < min.y) min.y = p.y;
			if (p.z < min.z) min.z = p.z;

			if (p.x > max.x) max.x = p.x;
			if (p.y > max.y) max.y = p.y;
			if (p.z > max.z) max.z = p.z;
		}
		centre = (min + max) * 0.5f;
		radius = glm::distance(centre, min);
	}
	const glm::vec3 GetCentre() const { return centre; }
	const float GetRadius() const { return radius; }

protected:
private:
	glm::vec3 centre;
	float radius;
};
