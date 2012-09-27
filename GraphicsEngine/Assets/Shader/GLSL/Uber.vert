uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

in vec3 InVertex;
out vec3 Vertex;

#ifdef PLUMB_NORMALS
uniform mat4 NormalMatrix;
in vec3 InNormal;
out vec3 Normal;
#endif

#ifdef PLUMB_UVS
in vec2 InUV;
out vec2 UV;
#endif

void main()
{
	gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(InVertex, 1.0);
	Vertex = (ModelViewMatrix * vec4(InVertex, 1.0)).xyz;

#ifdef PLUMB_NORMALS
	Normal = (NormalMatrix * vec4(InNormal, 0.0)).xyz;
#endif

#ifdef PLUMB_UVS
	UV = InUV;
#endif
}

