#version 400 core

in vec3 vNeutral;
in vec3 nNeutral;
in vec3 vSmile_L;
in vec3 nSmile_L;
in vec3 vSmile_R;
in vec3 nSmile_R;
in vec3 vBrowsup;
in vec3 nBrowsup;
in vec3 vMouthopen;
in vec3 nMouthopen;
in vec3 vPuff;
in vec3 nPuff;
in vec3 vSneer;
in vec3 nSneer;
in vec3 vKiss;
in vec3 nKiss;

uniform float left;
uniform float right;
uniform float top;
uniform float bottom;
uniform float near;
uniform float far;

uniform vec3 cameraPosition;
uniform vec3 lookAtPosition;
uniform vec3 upVector;

uniform vec4 lightPosition;

uniform vec3 pos;
uniform vec3 size;
uniform mat4 quaternion;

uniform float smile_l_w;
uniform float smile_r_w;
uniform float browsup_w;
uniform float mouthopen_w;
uniform float puff_w;
uniform float sneer_w;
uniform float kiss_w;

out vec3 lPos;
out vec3 vPos;
out vec3 vNorm;


void main(){

	float neutral_w = 1.0 - smile_l_w - smile_r_w - browsup_w - mouthopen_w - puff_w - sneer_w - kiss_w;
	neutral_w = clamp(0.0, 1.0, neutral_w);

	float sum_w = neutral_w + smile_l_w + smile_r_w + browsup_w + mouthopen_w + puff_w + sneer_w + kiss_w;
	float smile_l_w_f = smile_l_w / sum_w;
	float smile_r_w_f = smile_r_w / sum_w;
	float browsup_w_f = browsup_w / sum_w;
	float mouthopen_w_f = mouthopen_w / sum_w;
	float puff_w_f = puff_w / sum_w;
	float sneer_w_f = sneer_w / sum_w;
	float kiss_w_f = kiss_w / sum_w;
	neutral_w = neutral_w / sum_w;

	vec3 vPosition = vNeutral * neutral_w + vSmile_L * smile_l_w_f + vSmile_R * smile_r_w_f + vBrowsup * browsup_w_f
					 + vMouthopen * mouthopen_w_f + vPuff * puff_w_f + vSneer * sneer_w_f + vKiss * kiss_w_f;
	vec3 vNormal = nNeutral * neutral_w + nSmile_L * smile_l_w_f + nSmile_R * smile_r_w_f + nBrowsup * browsup_w_f
					 + nMouthopen * mouthopen_w_f + nPuff * puff_w_f + nSneer * sneer_w_f + nKiss * kiss_w_f;

	mat4 translate = mat4(1.0, 0.0, 0.0, 0.0,
							0.0, 1.0, 0.0, 0.0,
							0.0, 0.0, 1.0, 0.0,
							pos.x, pos.y, pos.z, 1.0);

	mat4 scale = mat4(size.x, 0.0, 0.0, 0.0,
						0.0, size.y, 0.0, 0.0,
						0.0, 0.0, size.z, 0.0,
						0.0, 0.0, 0.0, 1.0);

	mat4 model = translate * scale * quaternion;

	vec3 n = normalize(cameraPosition - lookAtPosition);
	vec3 u = normalize(cross(upVector, n));
	vec3 v = cross(n, u);

	mat4 view=mat4(u.x,v.x,n.x,0,
					u.y,v.y,n.y,0,
					u.z,v.z,n.z,0,
					dot(-u,cameraPosition),dot(-v,cameraPosition),dot(-n,cameraPosition),1);
	
	mat4 modelView = view * model;

	float p11=((2.0*near)/(right-left));
	float p31=((right+left)/(right-left));
	float p22=((2.0*near)/(top-bottom));
	float p32=((top+bottom)/(top-bottom));
	float p33=-((far+near)/(far-near));
	float p43=-((2.0*far*near)/(far-near));
	
	mat4 projection = mat4(p11, 0, 0, 0,
							0, p22, 0, 0,
							p31, p32, p33, -1,
							0, 0, p43, 0);

	//lighting calculation
	vec4 vertexInEye = modelView * vec4(vPosition, 1.0);
	vec4 lightInEye = view * lightPosition;
	vec4 normalInEye = normalize(modelView * vec4(vNormal, 0.0));

					
	lPos = lightInEye.xyz;
	vPos = vertexInEye.xyz;
	vNorm = normalInEye.xyz;


	gl_Position = projection * modelView * vec4(vPosition, 1.0);
}