#include "Assets/Mesh/CommonMesh.h"


using namespace std;


namespace GraphicsEngine
{
	const std::string CommonMesh::VertexAttribName = "InVertex";
	const std::string CommonMesh::NormalAttribName = "InNormal";
	const std::string CommonMesh::ColorAttribName = "InColor";
	const std::string CommonMesh::UVAttribName = "InUV";
	const std::string CommonMesh::TangentAttribName = "InTangent";
	const std::string CommonMesh::BitangentAttribName = "InBitangent";

	CommonMesh::CommonMesh()
		: GeneralMesh()
	{
		AddVec3Attrib(VertexAttribName);
	}

	bool CommonMesh::HasNormals() const
	{
		return HasVec3Attrib(NormalAttribName);
	}

	bool CommonMesh::HasColors() const
	{
		return HasVec3Attrib(ColorAttribName);
	}

	bool CommonMesh::HasUVs() const
	{
		return HasVec2Attrib(UVAttribName);
	}

	bool CommonMesh::HasTangentSpace() const
	{
		return HasVec3Attrib(TangentAttribName) && 
			   HasVec3Attrib(BitangentAttribName);
	}

	const Vec3List& CommonMesh::Vertices() const
	{
		return Vec3Attrib(VertexAttribName);
	}

	Vec3List& CommonMesh::Vertices()
	{
		return Vec3Attrib(VertexAttribName);
	}

	const Vec3List& CommonMesh::Normals() const
	{
		return Vec3Attrib(NormalAttribName);
	}

	Vec3List& CommonMesh::Normals()
	{
		return Vec3Attrib(NormalAttribName);
	}

	const Vec3List& CommonMesh::Colors() const
	{
		return Vec3Attrib(ColorAttribName);
	}

	Vec3List& CommonMesh::Colors()
	{
		return Vec3Attrib(ColorAttribName);
	}

	const Vec2List& CommonMesh::UVs() const
	{
		return Vec2Attrib(UVAttribName);
	}

	Vec2List& CommonMesh::UVs()
	{
		return Vec2Attrib(UVAttribName);
	}

	const Vec3List& CommonMesh::Tangents() const
	{
		return Vec3Attrib(TangentAttribName);
	}

	Vec3List& CommonMesh::Tangents()
	{
		return Vec3Attrib(TangentAttribName);
	}

	const Vec3List& CommonMesh::Bitangents() const
	{
		return Vec3Attrib(BitangentAttribName);
	}

	Vec3List& CommonMesh::Bitangents()
	{
		return Vec3Attrib(BitangentAttribName);
	}
}