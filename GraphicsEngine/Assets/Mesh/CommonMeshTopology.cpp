#include "Assets/Mesh/CommonMesh.h"
#include <unordered_map>
#include <unordered_set>
#include <stack>

using namespace std;

namespace GraphicsEngine
{
	template<typename T>
	void _slice(const vector<T>& allvals, const unordered_set<UINT> indices, vector<T>& outvals)
	{
		outvals.resize(indices.size());
		UINT i = 0;
		for (auto it = indices.begin(); it != indices.end(); it++)
		{
			outvals[i] = allvals[*it];
			i++;
		}
	}

	void CommonMesh::ConnectedComponents(vector<CommonMesh*>& outComps)
	{
		// Build vertex adjacency map
		unordered_map<UINT, unordered_set<UINT> > adjmap;
		for (UINT i = 0; i < faces.size(); i++)
		{
			const MeshFace& f = faces[i];
			adjmap[f.i[0]].insert(f.i[1]);
			adjmap[f.i[0]].insert(f.i[2]);
			adjmap[f.i[1]].insert(f.i[0]);
			adjmap[f.i[1]].insert(f.i[2]);
			adjmap[f.i[2]].insert(f.i[0]);
			adjmap[f.i[2]].insert(f.i[1]);
		}

		// Compute connected components on vertex ids
		vector< unordered_set<UINT> > comps;
		UINT n = NumVertices();
		vector<bool> visited(n, false);
		stack<UINT> fringe;
		for (UINT i = 0; i < n; i++)
		{
			if (!visited[i])
			{
				comps.push_back(unordered_set<UINT>());
				fringe.push(i);
				while (!fringe.empty())
				{
					UINT cur = fringe.top();
					fringe.pop();
					visited[cur] = true;
					comps.back().insert(cur);
					const unordered_set<UINT>& neighbors = adjmap[cur];
					for (auto it = neighbors.begin(); it != neighbors.end(); it++)
					{
						UINT n = *it;
						if (!visited[n])
							fringe.push(n);
					}
				}
			}
		}

		// Copy data into new submeshes; use index maps to keep things
		// consistent
		for (UINT c = 0; c < comps.size(); c++)
		{
			const unordered_set<UINT>& comp = comps[c];
			CommonMesh* mesh = new CommonMesh;
			unordered_map<UINT, UINT> indexMap;
			UINT newIndex = 0;
			for (auto it = comp.begin(); it != comp.end(); it++)
			{
				indexMap[*it] = newIndex;
				newIndex++;
			}

			// Copy vertex attribs
			for (auto it = FloatAttribsBegin(); it != FloatAttribsEnd(); it++)
			{
				mesh->AddFloatAttrib(it->first);
				FloatList& fa = mesh->FloatAttrib(it->first);
				_slice(it->second, comp, fa);
			}
			for (auto it = Vec2AttribsBegin(); it != Vec2AttribsEnd(); it++)
			{
				mesh->AddVec2Attrib(it->first);
				Vec2List& fa = mesh->Vec2Attrib(it->first);
				_slice(it->second, comp, fa);
			}
			for (auto it = Vec3AttribsBegin(); it != Vec3AttribsEnd(); it++)
			{
				mesh->AddVec3Attrib(it->first);
				Vec3List& fa = mesh->Vec3Attrib(it->first);
				_slice(it->second, comp, fa);
			}

			// Copy faces
			for (UINT i = 0; i < faces.size(); i++)
			{
				const MeshFace& f = faces[i];
				if (comp.count(f.i[0]))		// Only have to check one vertex, since they're all connected
				{
					mesh->faces.resize(mesh->faces.size()+1);
					MeshFace& newf = mesh->faces.back();
					newf.i[0] = indexMap[f.i[0]];
					newf.i[1] = indexMap[f.i[1]];
					newf.i[2] = indexMap[f.i[2]];
				}
			}

			outComps.push_back(mesh);
		}
	}

}