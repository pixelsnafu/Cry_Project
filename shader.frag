#version 400 core
uniform vec4 lightColor;

in vec3 lPos;
in vec3 vPos;
in vec3 vNorm;

out vec4 frag_color;

void main(){
	//copper like material light settings
	vec4 ambient = vec4(0.19125, 0.0735, 0.0225, 1.0);
	vec4 diff = vec4(0.7038,	0.27048, 0.0828, 1.0);
	vec4 spec = vec4(0.256777, 0.137622, 0.086014, 1.0);

	vec3 L = normalize (lPos - vPos);
    vec3 N = normalize (vNorm);
    vec3 Emissive = normalize(-vPos);
    vec3 R = reflect(-L, N);
    float dotProd = max(dot(R, Emissive), 0.0);
    vec4 specColor = lightColor*spec*pow(dotProd,0.1 * 128);
    vec4 diffuse = lightColor * diff * (dot(N, L));
	frag_color = ambient + diffuse + specColor;
}