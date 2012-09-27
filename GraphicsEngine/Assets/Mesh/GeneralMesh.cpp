#include "Assets/Mesh/GeneralMesh.h"


using namespace std;


namespace GraphicsEngine
{
	GeneralMesh::GeneralMesh()
	{
	}

	GeneralMesh::~GeneralMesh()
	{
		UnloadGLData();
	}

	UINT GeneralMesh::NumVertices() const
	{
		if (!vec3Attribs.empty())
			return vec3Attribs.begin()->second.size();
		if (!vec2Attribs.empty())
			return vec2Attribs.begin()->second.size();
		if (!floatAttribs.empty())
			return floatAttribs.begin()->second.size();
		return 0;
	}

	UINT GeneralMesh::NumFaces() const
	{
		return faces.size();
	}

	UINT GeneralMesh::NumFloatAttribs() const
	{
		return floatAttribs.size();
	}

	UINT GeneralMesh::NumVec2Attribs() const
	{
		return vec2Attribs.size();
	}

	UINT GeneralMesh::NumVec3Attribs() const
	{
		return vec3Attribs.size();
	}

	const FaceList& GeneralMesh::Faces() const
	{
		return faces;
	}

	FaceList& GeneralMesh::Faces()
	{
		return faces;
	}

	bool GeneralMesh::HasFloatAttrib(const string& attrib) const
	{
		return floatAttribs.count(attrib) != 0;
	}

	bool GeneralMesh::HasVec2Attrib(const string& attrib) const
	{
		return vec2Attribs.count(attrib) != 0;
	}

	bool GeneralMesh::HasVec3Attrib(const string& attrib) const
	{
		return vec3Attribs.count(attrib) != 0;
	}

	const FloatList& GeneralMesh::FloatAttrib(const string& attrib) const
	{
		auto it = floatAttribs.find(attrib);
		if (it == floatAttribs.end())
			FatalError(string("GraphicsEngine::GeneralMesh::FloatAttrib - No attribute '") + attrib + string("'"))
		else
			return it->second;
	}

	FloatList& GeneralMesh::FloatAttrib(const string& attrib)
	{
		auto it = floatAttribs.find(attrib);
		if (it == floatAttribs.end())
			FatalError(string("GraphicsEngine::GeneralMesh::FloatAttrib - No attribute '") + attrib + string("'"))
		else
			return it->second;
	}

	const Vec2List& GeneralMesh::Vec2Attrib(const string& attrib) const
	{
		auto it = vec2Attribs.find(attrib);
		if (it == vec2Attribs.end())
			FatalError(string("GraphicsEngine::GeneralMesh::Vec2Attrib - No attribute '") + attrib + string("'"))
		else
			return it->second;
	}

	Vec2List& GeneralMesh::Vec2Attrib(const string& attrib)
	{
		auto it = vec2Attribs.find(attrib);
		if (it == vec2Attribs.end())
			FatalError(string("GraphicsEngine::GeneralMesh::Vec2Attrib - No attribute '") + attrib + string("'"))
		else
			return it->second;
	}

	const Vec3List& GeneralMesh::Vec3Attrib(const string& attrib) const
	{
		auto it = vec3Attribs.find(attrib);
		if (it == vec3Attribs.end())
			FatalError(string("GraphicsEngine::GeneralMesh::Vec3Attrib - No attribute '") + attrib + string("'"))
		else
			return it->second;
	}

	Vec3List& GeneralMesh::Vec3Attrib(const string& attrib)
	{
		auto it = vec3Attribs.find(attrib);
		if (it == vec3Attribs.end())
			FatalError(string("GraphicsEngine::GeneralMesh::Vec3Attrib - No attribute '") + attrib + string("'"))
		else
			return it->second;
	}

	bool GeneralMesh::AddFloatAttrib(const string& attrib)
	{
		auto it = floatAttribs.find(attrib);
		if (it != floatAttribs.end())
			return false;
		else
		{
			floatAttribs[attrib] = FloatList();
			return true;
		}
	}

	bool GeneralMesh::AddVec2Attrib(const string& attrib)
	{
		auto it = vec2Attribs.find(attrib);
		if (it != vec2Attribs.end())
			return false;
		else
		{
			vec2Attribs[attrib] = Vec2List();
			return true;
		}
	}

	bool GeneralMesh::AddVec3Attrib(const string& attrib)
	{
		auto it = vec3Attribs.find(attrib);
		if (it != vec3Attribs.end())
			return false;
		else
		{
			vec3Attribs[attrib] = Vec3List();
			return true;
		}
	}

	FloatAttributesConstIterator GeneralMesh::FloatAttribsBegin() const
	{
		return floatAttribs.begin();
	}

	FloatAttributesIterator GeneralMesh::FloatAttribsBegin()
	{
		return floatAttribs.begin();
	}

	FloatAttributesConstIterator GeneralMesh::FloatAttribsEnd() const
	{
		return floatAttribs.end();
	}

	FloatAttributesIterator GeneralMesh::FloatAttribsEnd()
	{
		return floatAttribs.end();
	}

	Vec2AttributesConstIterator GeneralMesh::Vec2AttribsBegin() const
	{
		return vec2Attribs.begin();
	}

	Vec2AttributesIterator GeneralMesh::Vec2AttribsBegin()
	{
		return vec2Attribs.begin();
	}

	Vec2AttributesConstIterator GeneralMesh::Vec2AttribsEnd() const
	{
		return vec2Attribs.end();
	}

	Vec2AttributesIterator GeneralMesh::Vec2AttribsEnd()
	{
		return vec2Attribs.end();
	}

	Vec3AttributesConstIterator GeneralMesh::Vec3AttribsBegin() const
	{
		return vec3Attribs.begin();
	}

	Vec3AttributesIterator GeneralMesh::Vec3AttribsBegin()
	{
		return vec3Attribs.begin();
	}

	Vec3AttributesConstIterator GeneralMesh::Vec3AttribsEnd() const
	{
		return vec3Attribs.end();
	}

	Vec3AttributesIterator GeneralMesh::Vec3AttribsEnd()
	{
		return vec3Attribs.end();
	}

	void GeneralMesh::Render()
	{
		if (!glData.Loaded())
			LoadGLData();
		glData.Render();
	}

	void GeneralMesh::LoadGLData()
	{
		glData.Load(*this);
	}

	void GeneralMesh::UnloadGLData()
	{
		glData.Unload();
	}

	void GeneralMesh::RefreshGLData()
	{
		UnloadGLData();
		LoadGLData();
	}

	void GeneralMesh::UnloadClientData()
	{
		// Free memory for faces and all vertex attributes

		FreeVectorMemory(faces);
		for (auto it = floatAttribs.begin(); it != floatAttribs.end(); it++)
			FreeVectorMemory(it->second);
		for (auto it = vec2Attribs.begin(); it != vec2Attribs.end(); it++)
			FreeVectorMemory(it->second);
		for (auto it = vec3Attribs.begin(); it != vec3Attribs.end(); it++)
			FreeVectorMemory(it->second);
	}
}