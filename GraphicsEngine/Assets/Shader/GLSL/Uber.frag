in vec3 Vertex;

#ifdef PLUMB_NORMALS
in vec3 Normal;
#endif

#ifdef PLUMB_UVS
in vec2 UV;
#endif

#block MATERIAL

void main()
{
	gl_FragColor = Material_Evaluate();
}