#include "Assets/Mesh/GLStaticMeshData.h"
#include "Assets/Mesh/GeneralMesh.h"
#include "Assets/Shader/ShaderProgram.h"
#include "Common/GL.h"


using namespace std;


#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))


namespace GraphicsEngine
{
	GLStaticMeshData::GLStaticMeshData()
		: loaded(false)
	{
	}

	GLStaticMeshData::~GLStaticMeshData()
	{
		Unload();
	}

	void GLStaticMeshData::Load(const GeneralMesh& mesh)
	{
		UINT numVertices = mesh.NumVertices();
		UINT numElements = mesh.NumFaces();
		if (numElements == 0 || numVertices == 0)
			return;

		// Calculate size of buffers
		UINT numFloatAttribs = mesh.NumFloatAttribs();
		UINT numVec2Attribs = mesh.NumVec2Attribs();
		UINT numVec3Attribs = mesh.NumVec3Attribs();
		UINT floatsPerVertex = numFloatAttribs + 2*numVec2Attribs + 3*numVec3Attribs;
		vstride = floatsPerVertex * sizeof(float);
		UINT vdataSize = floatsPerVertex*numVertices;
		float* vertexData = new float[vdataSize];
		UINT idataSize = numIndices = 3*numElements;
		const UINT* indexData = (UINT*)(&mesh.Faces()[0]);

		// Fill in vertex data in interleaved order
		// Most efficient to go attribute-by-attribute, rather than vertex-by-vertex
		vertexAttribData.resize(numFloatAttribs+numVec2Attribs+numVec3Attribs);
		UINT offset = 0;
		UINT attribIndex = 0;
		for (auto it = mesh.Vec3AttribsBegin(); it != mesh.Vec3AttribsEnd(); it++, offset += 3, attribIndex++)
		{
			UINT j = offset;
			const Vec3List& vec3attrib = it->second;
			for (UINT i = 0; i < numVertices; i++, j += floatsPerVertex)
			{
				vertexData[j] = vec3attrib[i].x();
				vertexData[j+1] = vec3attrib[i].y();
				vertexData[j+2] = vec3attrib[i].z();
			}
			vertexAttribData[attribIndex].name = it->first;
			vertexAttribData[attribIndex].size = 3;
		}
		for (auto it = mesh.Vec2AttribsBegin(); it != mesh.Vec2AttribsEnd(); it++, offset += 2, attribIndex++)
		{
			UINT j = offset;
			const Vec2List& vec2attrib = it->second;
			for (UINT i = 0; i < numVertices; i++, j += floatsPerVertex)
			{
				vertexData[j] = vec2attrib[i].x();
				vertexData[j+1] = vec2attrib[i].y();
			}
			vertexAttribData[attribIndex].name = it->first;
			vertexAttribData[attribIndex].size = 2;
		}
		for (auto it = mesh.FloatAttribsBegin(); it != mesh.FloatAttribsEnd(); it++, offset += 1, attribIndex++)
		{
			UINT j = offset;
			const FloatList& fattrib = it->second;
			for (UINT i = 0; i < numVertices; i++, j += floatsPerVertex)
			{
				vertexData[j] = fattrib[i];
			}
			vertexAttribData[attribIndex].name = it->first;
			vertexAttribData[attribIndex].size = 1;
		}

		// Initialize vertex buffer
		glGenBuffers(1, &vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, vdataSize*sizeof(float), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vdataSize*sizeof(float), vertexData);

		// Initialize index buffer
		glGenBuffers(1, &iboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, idataSize*sizeof(UINT), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, idataSize*sizeof(UINT), indexData);

		// Clean up
		delete[] vertexData;

		loaded = true;
	}

	void GLStaticMeshData::Unload()
	{
		FreeVectorMemory(vertexAttribData);
		glDeleteBuffers(1, &vboID);
		glDeleteBuffers(1, &iboID);
		loaded = false;
	}

	void GLStaticMeshData::Render()
	{
		ShaderProgram* currProg = ShaderProgram::CurrentProgram();

		// Set up vertex buffer state
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		int offset = 0;
		int sizeoffloat = sizeof(float);
		UINT numAttribs = vertexAttribData.size();
		for (UINT i = 0; i < numAttribs; i++)
		{
			VertexAttribData& attribData = vertexAttribData[i];
			attribData.loc = currProg->GetAttributeLocation(attribData.name);
			if (attribData.loc != -1)
			{
				glVertexAttribPointer(attribData.loc, attribData.size, GL_FLOAT, GL_FALSE, vstride, BUFFER_OFFSET(offset));
				glEnableVertexAttribArray(attribData.loc);
			}
			offset += attribData.size*sizeoffloat;
		}

		// Set up the index buffer state
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);

		// Draw
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

		// Clean up
		for (UINT i = 0; i < numAttribs; i++)
			glDisableVertexAttribArray(vertexAttribData[i].loc);
	}
};