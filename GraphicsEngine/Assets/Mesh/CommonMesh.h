#pragma once

#ifndef GRAPHICSENGINE__COMMON_MESH_H
#define GRAPHICSENGINE__COMMON_MESH_H


#include "Assets/Mesh/GeneralMesh.h"

struct aiMesh;

namespace GraphicsEngine
{
	class Transform;

	/**
	Mesh that encapsulates lots of functionality commonly needed
	in graphics applications
	**/
	class CommonMesh : public GeneralMesh
	{
	public:

		CommonMesh();
		CommonMesh(const CommonMesh& other);
		CommonMesh& operator=(const CommonMesh& other);

		// Exposing commonly-used attributes
		// You can subclass CommonMesh if you want to expose new attributes in this way.
		// (Or subclass GeneralMesh to expose totally different sets of attributes).

		const Vec3List& Vertices() const;
		Vec3List& Vertices();
		static const std::string VertexAttribName;

		bool HasNormals() const;
		const Vec3List& Normals() const;
		Vec3List& Normals();
		static const std::string NormalAttribName;

		bool HasColors() const;
		const Vec3List& Colors() const;
		Vec3List& Colors();
		static const std::string ColorAttribName;

		bool HasUVs() const;
		const Vec2List& UVs() const;
		Vec2List& UVs();
		static const std::string UVAttribName;

		bool HasTangentSpace() const;
		const Vec3List& Tangents() const;
		Vec3List& Tangents();
		const Vec3List& Bitangents() const;
		Vec3List& Bitangents();
		static const std::string TangentAttribName;
		static const std::string BitangentAttribName;


		/* Simple geometry creation/manipulation */

		void ComputeNormals();

		virtual void Append(CommonMesh* other, const Transform& t);

		// Create an XY plane centered at the origin with the requested width,
		// height, and refinement.
		static CommonMesh* CreatePlane(float w, float h, int xdivs = 1, int ydivs = 1);

		// Create an axis-aligned box centered at the origin with the requested
		// width, height, depth, and refinement
		static CommonMesh* CreateBox(float w, float h, float d, int xdivs = 1, int ydivs = 1);

		// Create a sphere centered at the origin with the requested radius and refinement
		static CommonMesh* CreateSphere(float r, int slices, int stacks);

		// Create an XY disc with the requested radii and refinement
		static CommonMesh* CreateDisc(float ri, float ro, int slices);

		// Create an XY cylinder centered at the origin with the requested radius, height,
		// and refinement
		static CommonMesh* CreateCylinder(float r, float h, int slices, int stacks, bool capped = false);


		/* Getting meshes from files */

		void Load(const std::string& filename, bool genNormals = false, bool genTangentSpace = false);

		virtual void FromAssimpMesh(aiMesh* mesh);


		/* Topology */

		void ConnectedComponents(std::vector<CommonMesh*>& outComps);

	protected:

		Eigen::Vector3f Normal(const MeshFace& f, bool areaWeighted = false) const;
	};
}


#endif