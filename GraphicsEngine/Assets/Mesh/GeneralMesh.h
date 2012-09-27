#pragma once

#ifndef GRAPHICSENGINE__GENERAL_MESH_H
#define GRAPHICSENGINE__GENERAL_MESH_H


#include "Common/Common.h"
#include "Assets/Mesh/GLStaticMeshData.h"
#include "Eigen/Core"
#include <unordered_map>


namespace GraphicsEngine
{
	struct MeshFace
	{
		MeshFace() {}
		MeshFace(UINT i1, UINT i2, UINT i3)
		{
			i[0] = i1; i[1] = i2; i[2] = i3;
		}

		UINT i[3];
	};

	typedef std::vector<float> FloatList;
	typedef std::vector< Eigen::Vector2f > Vec2List;
	typedef std::vector< Eigen::Vector3f > Vec3List;
	typedef std::vector< MeshFace > FaceList;
	
	typedef std::unordered_map< std::string, FloatList > FloatAttributes;
	typedef std::unordered_map< std::string, Vec2List > Vec2Attributes;
	typedef std::unordered_map< std::string, Vec3List > Vec3Attributes;

	typedef FloatAttributes::iterator FloatAttributesIterator;
	typedef FloatAttributes::const_iterator FloatAttributesConstIterator;
	typedef Vec2Attributes::iterator Vec2AttributesIterator;
	typedef Vec2Attributes::const_iterator Vec2AttributesConstIterator;
	typedef Vec3Attributes::iterator Vec3AttributesIterator;
	typedef Vec3Attributes::const_iterator Vec3AttributesConstIterator;

	/**
	General-purpose triangle mesh class.
	**/
	class GeneralMesh
	{
	public:

		// Constructors & Destructor
		GeneralMesh();
		virtual ~GeneralMesh();

		// Basic property accessors
		UINT NumVertices() const;
		UINT NumFaces() const;
		UINT NumFloatAttribs() const;
		UINT NumVec2Attribs() const;
		UINT NumVec3Attribs() const;
		const FaceList& Faces() const;
		FaceList& Faces();

		// Vertex attribute existence checks
		bool HasFloatAttrib(const std::string& attrib) const;
		bool HasVec2Attrib(const std::string& attrib) const;
		bool HasVec3Attrib(const std::string& attrib) const;

		// Vertex attribute accessors
		const FloatList& FloatAttrib(const std::string& attrib) const;
		FloatList& FloatAttrib(const std::string& attrib);
		const Vec2List& Vec2Attrib(const std::string& attrib) const;
		Vec2List& Vec2Attrib(const std::string& attrib);
		const Vec3List& Vec3Attrib(const std::string& attrib) const;
		Vec3List& Vec3Attrib(const std::string& attrib);

		// Adding vertex attributes
		bool AddFloatAttrib(const std::string& attrib);
		bool AddVec2Attrib(const std::string& attrib);
		bool AddVec3Attrib(const std::string& attrib);

		// Iterating over vertex attributes
		FloatAttributesConstIterator FloatAttribsBegin() const;
		FloatAttributesIterator FloatAttribsBegin();
		FloatAttributesConstIterator FloatAttribsEnd() const;
		FloatAttributesIterator FloatAttribsEnd();
		Vec2AttributesConstIterator Vec2AttribsBegin() const;
		Vec2AttributesIterator Vec2AttribsBegin();
		Vec2AttributesConstIterator Vec2AttribsEnd() const;
		Vec2AttributesIterator Vec2AttribsEnd();
		Vec3AttributesConstIterator Vec3AttribsBegin() const;
		Vec3AttributesIterator Vec3AttribsBegin();
		Vec3AttributesConstIterator Vec3AttribsEnd() const;
		Vec3AttributesIterator Vec3AttribsEnd();

		// Rendering
		void Render();

		// Memory management
		void LoadGLData();
		void UnloadGLData();
		void RefreshGLData();		// Slow; not optimized for dynamic meshes
		void UnloadClientData();	// If we only need to draw the mesh and never
									// need to access its contents again

	protected:

		// Client-side mesh data
		FaceList faces;
		FloatAttributes floatAttribs;
		Vec2Attributes vec2Attribs;
		Vec3Attributes vec3Attribs;

	private:

		// GL-side mesh data
		GLStaticMeshData glData;
	};
}



#endif