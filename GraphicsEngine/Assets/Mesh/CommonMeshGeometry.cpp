#include "Assets/Mesh/CommonMesh.h"
#include "Math/Math.h"
#include "Math/Transform.h"
#include <Eigen/Geometry>


using namespace std;
using namespace Eigen;


namespace GraphicsEngine
{
	void CommonMesh::ComputeNormals()
	{
		UINT numv = NumVertices();

		// Build adjacency map
		vector< vector<UINT> > vert2face(numv);
		for (UINT i = 0; i < faces.size(); i++)
		{
			const MeshFace& f = faces[i];
			vert2face[f.i[0]].push_back(i);
			vert2face[f.i[1]].push_back(i);
			vert2face[f.i[2]].push_back(i);
		}

		// Prepare normal attribute
		if (!HasNormals())
			AddVec3Attrib(NormalAttribName);
		else
			Normals().clear();
		Vec3List& normals = Normals();
		normals.resize(numv, Vector3f::Zero());

		// Aggregate face normals
		for (UINT i = 0; i < numv; i++)
		{
			Vector3f& normal = normals[i];
			const vector<UINT>& adjFaces = vert2face[i];
			for (UINT j = 0; j < adjFaces.size(); j++)
			{
				normal += Normal(faces[adjFaces[j]], true);
			}
		}

		// Normalize
		for (UINT i = 0; i < NumVertices(); i++)
		{
			normals[i].normalize();
		}
	}

	Vector3f CommonMesh::Normal(const MeshFace& f, bool areaWeighted) const
	{
		const Vec3List& vertices = Vertices();
		Vector3f v01 = vertices[f.i[1]] - vertices[f.i[0]];
		Vector3f v02 = vertices[f.i[2]] - vertices[f.i[0]];
		if (areaWeighted)
			return v01.cross(v02);
		else
			return v01.cross(v02).normalized();
	}

	void CommonMesh::Append(CommonMesh* other, const Transform& t)
	{
		if (other->HasUVs())
			this->AddVec2Attrib(UVAttribName);
		if (other->HasNormals())
			this->AddVec3Attrib(NormalAttribName);
		if (other->HasColors())
			this->AddVec3Attrib(ColorAttribName);
		if (other->HasTangentSpace())
		{
			this->AddVec3Attrib(TangentAttribName);
			this->AddVec3Attrib(BitangentAttribName);
		}

		Transform invTranspT = t.Inverse().Transpose();

		UINT offset = this->Vertices().size();
		Vec3List& verts = other->Vertices();
		for (UINT i = 0; i < verts.size(); i++)
		{
			this->Vertices().push_back(t.TransformPoint(verts[i]));
			if (this->HasUVs())
				this->UVs().push_back(other->UVs()[i]);
			if (this->HasColors())
				this->Colors().push_back(other->Colors()[i]);
			if (this->HasNormals())
				this->Normals().push_back(invTranspT.TransformVector(other->Normals()[i]));
			if (this->HasTangentSpace())
			{
				this->Tangents().push_back(t.TransformVector(other->Tangents()[i]));
				this->Bitangents().push_back(t.TransformVector(other->Bitangents()[i]));
			}
		}

		FaceList& faces = other->Faces();
		for (UINT i = 0; i < faces.size(); i++)
		{
			const MeshFace& f = faces[i];
			this->Faces().push_back(MeshFace(f.i[0] + offset, f.i[1] + offset, f.i[2] + offset));
		}
	}

	CommonMesh* CommonMesh::CreatePlane(float w, float h, int xdivs, int ydivs)
	{
		CommonMesh* plane = new CommonMesh;
		plane->AddVec2Attrib(UVAttribName);

		// Compute vertex attributes
		Vec3List& vertices = plane->Vertices();
		Vec2List& uvs = plane->UVs();
		vertices.clear();
		uvs.clear();
		float xlo = -w/2; float xhi = w/2;
		float ylo = -h/2; float yhi = h/2;
		for (int y = 0; y <= ydivs; y++)
		{
			for (int x = 0; x <= xdivs; x++)
			{
				float xt = ((float)x)/xdivs;
				float yt = ((float)y)/ydivs;
				vertices.push_back(Vector3f(Lerp(xlo, xhi, xt), Lerp(ylo, yhi, yt), 0));
				uvs.push_back(Vector2f(Lerp(0.0f, 1.0f, xt), Lerp(0.0f, 1.0f, yt)));
			}
		}

		// Compute faces
		FaceList& faces = plane->Faces();
		for (int y = 0; y < ydivs; y++)
		{
			for (int x = 0; x < xdivs; x++)
			{
				int v = y*(xdivs+1)+x;
				faces.push_back(MeshFace(v, v+xdivs+2, v+xdivs+1));
				faces.push_back(MeshFace(v, v+1, v+xdivs+2));
			}
		}

		plane->ComputeNormals();

		return plane;
	}

	CommonMesh* CommonMesh::CreateBox(float w, float h, float d, int xdivs, int ydivs)
	{
		CommonMesh* box = new CommonMesh;
		box->AddVec2Attrib(UVAttribName);
		CommonMesh* plane = CreatePlane(w, h, xdivs, ydivs);

		/** Append 6 properly transformed copies of the plane to the box **/

		Transform t;

		// Front face
		t = Transform::Translation(0, 0, d/2);
		box->Append(plane, t);

		// Back face
		t = Transform::Translation(0, 0, -d/2) * Transform::RotationX(DegreesToRadians(180.0f));
		box->Append(plane, t);

		// Left face
		t = Transform::Translation(-w/2, 0, 0) * Transform::RotationY(DegreesToRadians(-90.0f));
		box->Append(plane, t);

		// Right face
		t = Transform::Translation(w/2, 0, 0) * Transform::RotationY(DegreesToRadians(90.0f));
		box->Append(plane, t);

		// Top face
		t = Transform::Translation(0, h/2, 0) * Transform::RotationX(DegreesToRadians(-90.0f));
		box->Append(plane, t);

		// Bottom face
		t = Transform::Translation(0, -h/2, 0) * Transform::RotationX(DegreesToRadians(90.0f));
		box->Append(plane, t);

		box->ComputeNormals();

		delete plane;
		return box;
	}

	CommonMesh* CommonMesh::CreateSphere(float r, int slices, int stacks)
	{
		CommonMesh* sphere = new CommonMesh;
		sphere->AddVec2Attrib(UVAttribName);
		
		Vec3List& verts = sphere->Vertices();
		Vec2List& uvs = sphere->UVs();
		for (int stack = 1; stack < stacks; stack++)
		{
			float theta = Lerp(0.0f, PI, ((float)stack)/stacks);
			float sintheta = sin(theta);
			float costheta = cos(theta);
			float costhetainterp = (costheta + 1) / 2;
			for (int slice = 0; slice < slices; slice++)
			{
				float phi = Lerp(0.0f, 2*PI, ((float)slice)/slices);
				verts.push_back(Vector3f(r*sintheta*cos(phi), r*sintheta*sin(phi), Lerp(-r, r, costhetainterp)));
				uvs.push_back(Vector2f(phi/(2*PI), theta/PI));
			}
		}
		// Singularities
		verts.push_back(Vector3f(0, 0, r));
		uvs.push_back(Vector2f(0, 0));
		verts.push_back(Vector3f(0, 0, -r));
		uvs.push_back(Vector2f(0, 1));

		FaceList& faces = sphere->Faces();
		for (int stack = 1; stack < stacks-1; stack++)
		{
			for (int slice = 0; slice < slices; slice++)
			{
				int v = (stack-1)*slices + slice;
				int v0 = v;
				int v1 = v+slices;
				int v2 = ((v+slices+1) % slices == 0 ? v+1 : v+slices+1);
				int v3 = ((v+1) % slices == 0 ? v+1-slices : v+1);
				MeshFace f1(v0, v1, v2);
				MeshFace f2(v0, v2, v3);
				faces.push_back(f1);
				faces.push_back(f2);
			}
		}
		// Faces involving singularities
		for (int slice = 0; slice < slices; slice++)
		{
			// Bottom
			int stack = 1;
			int v = (stack-1)*slices + slice;
			int v1 = ((v+1) % slices == 0 ? v+1-slices : v+1);
			int v2 = verts.size()-2;
			MeshFace flo(v, v1, v2);
			faces.push_back(flo);

			// Top
			stack = stacks-1;
			v = (stack-1)*slices + slice;
			v1 = ((v+1) % slices == 0 ? v+1-slices : v+1);
			v2 = verts.size()-1;
			MeshFace fhi(v2, v1, v);
			faces.push_back(fhi);
		}

		sphere->ComputeNormals();
		return sphere;
	}

	CommonMesh* CommonMesh::CreateDisc(float ri, float ro, int slices)
	{
		CommonMesh* disc = new CommonMesh;
		disc->AddVec2Attrib(UVAttribName);

		Vec3List& verts = disc->Vertices();
		Vec2List& uvs = disc->UVs();
		for (int slice = 0; slice < slices; slice++)
		{
			float phi = Lerp(0.0f, 2*PI, ((float)slice)/slices);
			verts.push_back(Vector3f(ri*cos(phi), ri*sin(phi), 0));
			uvs.push_back(Vector2f(phi/(2*PI), 0.0f));
			verts.push_back(Vector3f(ro*cos(phi), ro*sin(phi), 0));
			uvs.push_back(Vector2f(phi/(2*PI), 1.0f));
		}

		FaceList& faces = disc->Faces();
		int n = 2*slices;
		for (int slice = 0; slice < slices; slice++)
		{
			int v = 2*slice;
			faces.push_back(MeshFace((v+3)%n, (v+2)%n, v));
			faces.push_back(MeshFace(v+1, (v+3)%n, v));
		}

		disc->ComputeNormals();
		return disc;
	}

	CommonMesh* CommonMesh::CreateCylinder(float r, float h, int slices, int stacks, bool capped)
	{
		CommonMesh* cyl = new CommonMesh;
		cyl->AddVec2Attrib(UVAttribName);

		/** Main cylinder **/

		Vec3List& verts = cyl->Vertices();
		Vec2List& uvs = cyl->UVs();
		for (int stack = 0; stack <= stacks; stack++)
		{
			float v = ((float)stack)/stacks;
			float z = Lerp(-h/2, h/2, v);
			for (int slice = 0; slice < slices; slice++)
			{
				float phi = Lerp(0.0f, 2*PI, ((float)slice)/slices);
				verts.push_back(Vector3f(r*cos(phi), r*sin(phi), z));
				uvs.push_back(Vector2f(phi/(2*PI), v));
			}
		}

		FaceList& faces = cyl->Faces();
		for (int stack = 0; stack < stacks; stack++)
		{
			for (int slice = 0; slice < slices; slice++)
			{
				int v = stack*slices + slice;
				int v0 = v;
				int v1 = v+slices;
				int v2 = ((v+slices+1) % slices == 0 ? v+1 : v+slices+1);
				int v3 = ((v+1) % slices == 0 ? v+1-slices : v+1);
				MeshFace f1(v2, v1, v0);
				MeshFace f2(v3, v2, v0);
				faces.push_back(f1);
				faces.push_back(f2);
			}
		}

		/** Disc caps **/
		if (capped)
		{
			CommonMesh* disc = CreateDisc(0, r, slices);
			Transform t;
			t = Transform::Translation(0, 0, h/2);
			cyl->Append(disc, t);
			t = Transform::Translation(0, 0, -h/2) * Transform::RotationX(DegreesToRadians(180.0f));
			cyl->Append(disc, t);
			delete disc;
		}

		cyl->ComputeNormals();
		return cyl;
	}
};