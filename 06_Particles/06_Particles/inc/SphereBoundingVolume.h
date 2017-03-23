#pragma once
#include <glm/vec3.hpp>
#include <vector>
#include <iostream>

class SphereBoundingVolume
{
public:

	SphereBoundingVolume() : centre(0), radius(0) {}
	~SphereBoundingVolume() = default;

	template<typename T>
	void fit(const std::vector<T>& verts)
	{
		// Set bounding box parameters
		glm::vec3 min(1e37f);	// 9.9999*10^36
		glm::vec3 max(-1e37f);

		for (auto& vert : verts)
		{
			// If Template has no position, error will arise.
			// If constructor cannot convert to vec3
			/*
			Verts all have a position, what space does this all take up.
			calculate min/max of the bounding box of the verts
			Is each vert close to edge of calculations - loop and check
			Move min/max to those vert positions
			Check all vrts through a loop, until bounds are found, and set.
			The average bn those, = centre of sphere.
			Center with distance to min/max wil lequal the radius.
			*/
			glm::vec3 position = glm::vec3(vert.position);
			if (p.x < min.x) min.x = p.x;
			if (p.y < min.x) min.y = p.y;
			if (p.z < min.x) min.z = p.z;
			if (p.x < max.x) max.x = p.x;
			if (p.y < max.x) max.y = p.y;
			if (p.z < max.x) max.z = p.z;
		}
		center = (min + max) * 0.5f;
		radius = glm::distance(centre, min);
	}

	const glm::vec3 GetCentre() const { return centre; }
	const float GetRadius() const { return radius; }

protected:

private:
	glm::vec3	centre;
	float		radius;

};
/*
Tutorial:
Fit function array of vec3, iterates for min/max?
Class mod - template the fit()

Go bk ot RenderData class and ADD
include sphereboundingvolume
RenderData.h 
public:
//Return this render data bounds as a const
const SphereBoundingVolume& GetBoundsAsConst() const {return m_bounds;}
const SphereBoundingVolume& GetBounds() {return m_bounds;}

private: 
// Store Bounds for this render data
SphereBoundingVolume m_bounds;		// every mesh will have a bounds assoc. with it.


GeometryHelper.cppp

renderData->SetNumberOfIndices(numberofIndices);
renderData->Unbind();

std::vector<SimpleVertex> verts(vertices, vertices + (rows * cols));
renderData->GetBounds().fit(verts);

delete[] vertices;	// before this
delete[]indices[]

return renderData;

LoadObjFromDisk()
renderData->GetBounds().fit(vertices);		// add this here
modelRenderData.push_back(renderData);		// before this

// add tocamera - check in frustrum
- take in bounds, checks, return true/false if in bounds
camera.h
#include sphereBoundingVolumes

public:
bool IsBoundsInFrustrum(const SphereBoundingVolume& bounds);

camera.cpp
// tutorial page 7 - iterate all 6 check instersect and return true
// printf changed to cout in tutorial

bool Camera::IsBoundsInFrustrum(const SphereBoundingVolume & bound)
{
for (int i = 0; i < 6; ++i)
{
// Plane has directoin facing, distance of nearest point of plane from origin of the world
//   |  \   .	Distance of point and world origin(0,0)
//   |	 \      Distance of plane
//  .|_________
const glm::vec4& plane = m_frustrumPlanes[i];
float d = glm::dot(glm::vec3(plane), bounds.GetCentre()) + plane.w;
if (d < -bounds.GetRadius())
{
 // behind plane .: outside plane, .: not inside frustrum .: dont render.
 std::cout << "outside of a plane, no need to render !\n";
 return false;
 }
 }
 std::cout << "inside frustum, render"\n;
 return true;
 }

 // GO TO: Application3D, draw()
 for (auto& renderData : m_soulspear)
 {
 if (m_camera->IsBoundsInFrustum(renderData->GetBoundsAsConst()))
 {
 renderData->Render();
 }
 }

 */
