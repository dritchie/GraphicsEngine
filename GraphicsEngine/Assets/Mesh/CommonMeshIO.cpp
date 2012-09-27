#include "Assets/Mesh/CommonMesh.h"
#include "Math/Transform.h"
#include "assimp.hpp"
#include "aiScene.h"
#include "aiPostProcess.h"
#include <queue>

using namespace std;
using Eigen::Vector2f;
using Eigen::Vector3f;
using Eigen::Matrix4f;

namespace GraphicsEngine
{
	void CommonMesh::Load(const std::string& filename, bool genNormals, bool genTangentSpace)
	{
		Assimp::Importer imp;

		UINT pflags = aiProcess_JoinIdenticalVertices |
					  aiProcess_Triangulate |
					  aiProcess_FixInfacingNormals |
					  aiProcess_GenUVCoords |
					  aiProcess_ImproveCacheLocality |
					  aiProcess_OptimizeMeshes |
					  aiProcess_RemoveRedundantMaterials |
					  aiProcess_PreTransformVertices;
		if (genNormals)
			pflags |= aiProcess_GenSmoothNormals;
		if (genTangentSpace)
			pflags |= aiProcess_CalcTangentSpace;

		const aiScene* scene = imp.ReadFile(filename, pflags);

		// First, convert all the meshes into CommonMeshes
		vector<CommonMesh> meshes; meshes.resize(scene->mNumMeshes);
		for (UINT i = 0; i < scene->mNumMeshes; i++)
		{
			meshes[i].FromAssimpMesh(scene->mMeshes[i]);
		}

		// After using the above preprocessing flags, most files should be collapsed to a single node.
		// At most, we'll have a root node with one level of children.
		queue<aiNode*> fringe;
		fringe.push(scene->mRootNode);
		while (!fringe.empty())
		{
			aiNode* node = fringe.front();
			fringe.pop();
			if (node->mNumMeshes > 0)
			{
				Transform t(node->mTransformation);
				for (UINT i = 0; i < node->mNumMeshes; i++)
				{
					this->Append(&meshes[node->mMeshes[i]], t);
				}
			}
			for (UINT i = 0; i < node->mNumChildren; i++)
				fringe.push(node->mChildren[i]);
		}
	}

	void CommonMesh::FromAssimpMesh(aiMesh* mesh)
	{
		// If this mesh has no faces or vertex positions,
		// there's nothing to render, so we're done.
		if (!mesh->HasFaces() || !mesh->HasPositions())
			return;

		// Vertex attribs
		VectorArrayCopy(Vertices(), (Vector3f*)mesh->mVertices, mesh->mNumVertices);
		if (mesh->HasNormals())
		{
			AddVec3Attrib(NormalAttribName);
			VectorArrayCopy(Normals(), (Vector3f*)mesh->mNormals, mesh->mNumVertices);
		}
		if (mesh->HasTextureCoords(0) && mesh->GetNumUVChannels() == 2)
		{
			AddVec2Attrib(UVAttribName);
			Vec2List& uvs = UVs(); uvs.resize(mesh->mNumVertices);
			for (UINT i = 0; i < mesh->mNumVertices; i++)
				uvs[i] = Vector2f(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		if (mesh->HasVertexColors(0))
		{
			AddVec3Attrib(ColorAttribName);
			Vec3List& colors = Colors(); colors.resize(mesh->mNumVertices);
			for (UINT i = 0; i < mesh->mNumVertices; i++)
				colors[i] = Vector3f(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b);

		}
		if (mesh->HasTangentsAndBitangents())
		{
			AddVec3Attrib(TangentAttribName);
			VectorArrayCopy(Tangents(), (Vector3f*)mesh->mTangents, mesh->mNumVertices);
			AddVec3Attrib(BitangentAttribName);
			VectorArrayCopy(Normals(), (Vector3f*)mesh->mBitangents, mesh->mNumVertices);
		}

		// Faces
		FaceList& faces = Faces();
		for (UINT i = 0; i < mesh->mNumFaces; i++)
		{
			const aiFace& f = mesh->mFaces[i];
			if (f.mNumIndices == 3)
				faces.push_back(MeshFace(f.mIndices[0], f.mIndices[1], f.mIndices[2]));
		}
	}
}