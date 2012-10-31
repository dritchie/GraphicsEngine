#include "Assets/Mesh/CommonMesh.h"
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include "ANN/ANN.h"

using namespace std;

namespace GraphicsEngine
{
	template<typename T, typename IntContainerType>
	void _slice(const vector<T>& allvals, const IntContainerType& indices, vector<T>& outvals)
	{
		outvals.resize(indices.size());
		UINT i = 0;
		for (auto it = indices.begin(); it != indices.end(); it++)
		{
			outvals[i] = allvals[*it];
			i++;
		}
	}

	void CommonMesh::ConnectedComponents(vector<CommonMesh*>& outComps, bool connectCollocatedVerts) const
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

		// If requested, augment the adjacency map with information about vertices that are collocated
		if (connectCollocatedVerts)
		{
			vector< vector<int> > eqClasses;
			VertexEquivalenceClasses(eqClasses, 0.01f);
			
			// Each vertex in an equivalence class is 'adjacent' to all other vertices in that class
			for (UINT i = 0; i < eqClasses.size(); i++)
			{
				vector<int>& eqc = eqClasses[i];
				for (UINT j = 0; j < eqc.size()-1; j++) for (UINT k = j+1; k < eqc.size(); k++)
				{
					UINT jj = (UINT)eqc[j];;
					UINT kk = (UINT)eqc[k];
					adjmap[jj].insert(kk);
					adjmap[kk].insert(jj);
				}
			}
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


	void CommonMesh::FuseCollocatedVertices()
	{
		// This operation definitely invalidates any GL data we have stored
		UnloadGLData();

		vector< vector<int > > eqClasses;
		VertexEquivalenceClasses(eqClasses);
		
		// The equivalence classes become the new vertices. We first need to map old vert indices
		// to new vert indices
		unordered_map<int,int> indexMap;
		for (UINT i = 0; i < eqClasses.size(); i++)
		{
			for (UINT j = 0; j < eqClasses[i].size(); j++)
			{
				indexMap[eqClasses[i][j]] = i;
			}
		}

		// We now rip out all vertices except the first one in each equivalence class.
		vector<UINT> keepIndices;
		for (UINT i = 0; i < eqClasses.size(); i++)
			keepIndices.push_back(eqClasses[i][0]);
		for (auto it = FloatAttribsBegin(); it != FloatAttribsEnd(); it++)
		{
			FloatList newlist;
			_slice(it->second, keepIndices, newlist);
			it->second = newlist;
		}
		for (auto it = Vec2AttribsBegin(); it != Vec2AttribsEnd(); it++)
		{
			Vec2List newlist;
			_slice(it->second, keepIndices, newlist);
			it->second = newlist;
		}
		for (auto it = Vec3AttribsBegin(); it != Vec3AttribsEnd(); it++)
		{
			Vec3List newlist;
			_slice(it->second, keepIndices, newlist);
			it->second = newlist;
		}


		// Finally, we iterate over the faces, replacing all old indices with new indices
		// according to indexMap.
		for (UINT i = 0; i < faces.size(); i++)
		{
			MeshFace& f = faces[i];
			f.i[0] = (UINT)indexMap[f.i[0]];
			f.i[1] = (UINT)indexMap[f.i[1]];
			f.i[2] = (UINT)indexMap[f.i[2]];
		}
	}


	void CommonMesh::VertexEquivalenceClasses(std::vector< std::vector<int> >& outClasses, float distThresh) const
	{
		/** Set up KD tree for nearest-neighbor search **/

		const int dim = 3;					// we're in 3 space
		const int k = 100000;				// we want *all* points within the radius
		const float r2 = distThresh*distThresh;

		int nPts = (int)NumVertices();		// actual number of data points
		ANNpointArray dataPts;				// data points
		ANNpoint queryPt;					// query point

		ANNidxArray nnIdx;					// near neighbor indices
		ANNdistArray dists;					// near neighbor distances
		ANNkd_tree* kdTree;					// search structure

		queryPt = annAllocPt(dim);			// allocate query point
		dataPts = annAllocPts(nPts, dim);	// allocate data points
		nnIdx = new ANNidx[k];				// allocate near neigh indices
		dists = new ANNdist[k];				// allocate near neighbor dists

		// Fill in data points array with my vertices
		const Vec3List& verts = Vertices();
		for (int i = 0; i < nPts; i++)
		{
			dataPts[i][0] = verts[i][0];
			dataPts[i][1] = verts[i][1];
			dataPts[i][2] = verts[i][2];
		}

		kdTree = new ANNkd_tree(			// build search structure
			dataPts,						// the data points
			nPts,							// number of points
			dim);							// dimension of space

		// Build 'equivalence classes' of vertices by doing nearest neighbor searches
		// from each vertex
		vector<bool> seen(nPts, false);
		for (int i = 0; i < nPts; i++)
		{
			// If we haven't seen this vertex already, make a new equivalence class
			if (!seen[i])
			{
				outClasses.push_back(vector<int>());

				// Copy vertex location into query point
				queryPt[0] = verts[i][0];
				queryPt[1] = verts[i][1];
				queryPt[2] = verts[i][2];

				int numPoints = kdTree->annkFRSearch(
					queryPt,						// query point
					r2,								// squared radius
					k,								// number of near neighbors to return
					nnIdx,							// nearest neighbor array (modified)
					dists,							// dist to near neighbors (modified)
					0.0);							// error bound

				if (numPoints == 0)
					FatalError(string("CommonMesh::VertexEquivalenceClasses - ANN returned 0 points for query!"))

				// Add all the retrieved points to the equivalence class, and mark them
				// all as seen
				for (int j = 0; j < numPoints; j++)
				{
					outClasses.back().push_back(nnIdx[j]);
					seen[nnIdx[j]] = true;
				}
			}
		}

		// Clean up
		delete [] nnIdx;
		delete [] dists;
		delete kdTree;
		annDeallocPt(queryPt);
		annDeallocPts(dataPts);
	}

}