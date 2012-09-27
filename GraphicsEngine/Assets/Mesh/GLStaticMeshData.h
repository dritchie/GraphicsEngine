#pragma once

#ifndef GRAPHICSENGINE__GL_STATIC_MESH_DATA_H
#define GRAPHICSENGINE__GL_STATIC_MESH_DATA_H


#include "Common/Common.h"


namespace GraphicsEngine
{
	class GeneralMesh;

	/**
	Handles mesh data for OpenGL to render. Intended for static
	meshes, so it should not be updated frequently.
	**/
	class GLStaticMeshData
	{
	public:

		GLStaticMeshData();
		~GLStaticMeshData();

		void Load(const GeneralMesh& mesh);
		void Unload();
		bool Loaded() const;
		void Render();

	private:

		struct VertexAttribData
		{
			std::string name;
			UINT size;
			int loc;
		};

		std::vector< VertexAttribData > vertexAttribData;
		UINT vboID, iboID;
		UINT vstride;
		UINT numIndices;
		bool loaded;
	};

	__forceinline bool GLStaticMeshData::Loaded() const
	{
		return loaded;
	}
};


#endif