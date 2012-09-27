in vec3 Vertex;
in vec3 Normal;

void main()
{
	// "Parameters"
	vec4 ambient = vec4(0.0, 0.0, 0.1, 1.0);
	vec4 diffuse = vec4(0.0, 0.0, 0.9, 1.0);
	vec4 specular = vec4(1.0, 1.0, 1.0, 1.0);
	float shine = 30.0;
	vec4 lightIntensity = vec4(1.0, 1.0, 1.0, 1.0);


	vec3 p = Vertex;
	vec3 n = normalize(Normal);
	vec3 wEye = normalize(-p);

	vec4 color = ambient;

	vec3 vecToLight = vec3(4.0, 4.0, 4.0) - p;
	vec3 wLight = normalize(vecToLight);

	vec4 diffuseComp = diffuse * max(dot(n, wLight), 0.0);
	vec3 r = -reflect(wLight, n);
	vec4 specComp = specular * pow(max(dot(r, wEye), 0.0), shine);
	vec4 bsdfVal = diffuseComp + specComp;

	color += bsdfVal * lightIntensity;

	gl_FragColor = color;
}