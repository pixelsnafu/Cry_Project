#include "Shape.h"

bool face::quad = false;

Shape::Shape(GLuint vao, vector<Vec3f> points, vector<Vec3f> normals, vector<face> faces){
	this->vao = vao;
	this->points = points;
	this->normals = normals;
	this->faces = faces;

	scale = new float[3]();
	translate = new float[3]();
	rotate = new float[16]();
	diffuseColor = new float[4]();

	for(int i = 0; i<3; i++){
		scale[i] = 1.0f;
	}

	for(int i = 0; i<4; i++){
		diffuseColor[i] = 1.0f;
	}

	rotate[0] = 1.0;
	rotate[5] = 1.0;
	rotate[10] = 1.0;
	rotate[15] = 1.0;

	smooth = false;

	createVertices();
}

Shape::Shape(vector<Vec3f> points, vector<Vec3f> normals, vector<face> faces){
	this->points = points;
	this->normals = normals;
	this->faces = faces;

	scale = new float[3]();
	translate = new float[3]();
	rotate = new float[16]();
	diffuseColor = new float[4]();

	for(int i = 0; i<3; i++){
		scale[i] = 1.0f;
	}

	for(int i = 0; i<4; i++){
		diffuseColor[i] = 1.0f;
	}

	rotate[0] = 1.0;
	rotate[5] = 1.0;
	rotate[10] = 1.0;
	rotate[15] = 1.0;

	smooth = false;

	createVertices();
}

Shape::Shape(vector<Vec3f> points, vector<Vec3f> normals, vector<face> faces, map<Vec3f, vector<face>, Vec3Comp> vertexElements){
	this->points = points;
	this->normals = normals;
	this->faces = faces;
	this->vertexElements = vertexElements;

	scale = new float[3]();
	translate = new float[3]();
	rotate = new float[16]();
	diffuseColor = new float[4]();

	for(int i = 0; i<3; i++){
		scale[i] = 1.0f;
	}

	for(int i = 0; i<4; i++){
		diffuseColor[i] = 1.0f;
	}

	rotate[0] = 1.0;
	rotate[5] = 1.0;
	rotate[10] = 1.0;
	rotate[15] = 1.0;

	smooth = false;

	createVertices();
}

void Shape::createVertices(){
	outVertices.clear();
	for(int i = 0; i<faces.size(); i++){
		outVertices.push_back(points[faces[i].a]);
		outVertices.push_back(points[faces[i].b]);
		outVertices.push_back(points[faces[i].c]);
		if(face::quad)
			outVertices.push_back(points[faces[i].d]);
	}

	//cout<<faces.size()<<" "<<outVertices.size()<<endl;

	outNormals.clear();
	for(int i = 0; i<faces.size(); i++){
		outNormals.push_back(normals[faces[i].n]);
		outNormals.push_back(normals[faces[i].n]);
		outNormals.push_back(normals[faces[i].n]);
		if(face::quad){
			outNormals.push_back(normals[faces[i].n]);
		}
	}

	vertices = new float[outVertices.size() * 3]();
	for(int i = 0; i < outVertices.size(); i++){
		vertices[i*3] = outVertices[i].x;
		vertices[i*3 + 1] = outVertices[i].y;
		vertices[i*3 + 2] = outVertices[i].z;
	}

	normalArray = new GLfloat[3 * outNormals.size()];
	for (int i = 0; i < outNormals.size(); i++){
		normalArray[i*3] = outNormals[i].x;
		normalArray[i*3 + 1] = outNormals[i].y;
		normalArray[i*3 + 2] = outNormals[i].z;
	}
}
/*
void Shape::smoothNormals(){

	smooth = true;

	for(int i = 0; i < points.size(); i++){
		int num = 0;
		Vec3f temp;

		for(int j = 0; j<faces.size(); j++){
			if(i == faces[j].a || i == faces[j].b || i == faces[j].c){

				temp += normals[faces[j].n];
				num++;
			}
		}

		if(num)
			temp /= num;

		temp.normalize();

		vertexNormals.push_back(temp);
	}

	cout<<points.size() <<" "<< vertexNormals.size()<<endl;

	for(int i = 0; i<faces.size(); i++){

		outVertexNormals.push_back(vertexNormals[faces[i].a]);
		outVertexNormals.push_back(vertexNormals[faces[i].b]);
		outVertexNormals.push_back(vertexNormals[faces[i].c]);
	}

	vertexNormalArray = new float[outVertexNormals.size() * 3];
	for(int i = 0; i<outVertexNormals.size(); i++){
		vertexNormalArray[i*3] = outVertexNormals[i].x;
		vertexNormalArray[i*3 + 1] = outVertexNormals[i].y;
		vertexNormalArray[i*3 + 2] = outVertexNormals[i].z;
	}
}

*/
void Shape::smoothNormals(){
	smooth = true;
	//cout<<vertexElements.size()<<endl;
	for(GLuint i = 0; i < points.size(); i++){
		Vec3f v = points.at(i);
		vector<face> faceList = vertexElements.at(v);
		Vec3f vn;
		for(unsigned j = 0; j < faceList.size(); j++){
			vn += normals.at(faceList.at(j).n);
		}
		vn /= faceList.size();
		vn.normalize();

		vertexNormals.push_back(vn);
	}

	for(int i = 0; i<faces.size(); i++){

		outVertexNormals.push_back(vertexNormals[faces[i].a]);
		outVertexNormals.push_back(vertexNormals[faces[i].b]);
		outVertexNormals.push_back(vertexNormals[faces[i].c]);

		if(face::quad)
			outVertexNormals.push_back(vertexNormals[faces[i].d]);
	}

	vertexNormalArray = new float[outVertexNormals.size() * 3];
	for(int i = 0; i<outVertexNormals.size(); i++){
		vertexNormalArray[i*3] = outVertexNormals[i].x;
		vertexNormalArray[i*3 + 1] = outVertexNormals[i].y;
		vertexNormalArray[i*3 + 2] = outVertexNormals[i].z;
	}
}

void Shape::setDiffuseColor(float r, float g, float b){
	diffuseColor[0] = r;
	diffuseColor[1] = g;
	diffuseColor[2] = b;
	diffuseColor[3] = 1.0f;
}

void Shape::setPosition(float x, float y, float z){
	translate[0] = x;
	translate[1] = y;
	translate[2] = z;
}

void Shape::setSize(float x, float y, float z){
	scale[0] = x;
	scale[1] = y;
	scale[2] = z;
}

void Shape::setRotate(float x, float y, float z, float theta){
	Vec3f axis = Vec3f(x, y, z);
	Quaternion r = Quaternion::FromAxisAngle(axis, theta);
	r.normalize();
	rotate = r.toMatrix();
}

void Shape::setRotate(Quaternion q){
	q.normalize();
	if(rotate)
		delete [] rotate;
	rotate = q.toMatrix();
}

void Shape::setVao(GLuint vao){
	this->vao = vao;
}

float* Shape::getPosition(){
	return translate;
}

float* Shape::getSize(){
	return scale;
}

void Shape::initBuffers(const GLuint& program, const char* vertexAttributeName, const char* normalAttributeName){
	vertexAttribute = glGetAttribLocation(program, vertexAttributeName);
	normalAttribute = glGetAttribLocation(program, normalAttributeName);

	glBindVertexArray(vao);

	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, outVertices.size() * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexAttribute);
	glVertexAttribPointer(vertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	if(smooth)
		glBufferData(GL_ARRAY_BUFFER, outVertexNormals.size() * 3 * sizeof(float), vertexNormalArray, GL_STATIC_DRAW);
	else
		glBufferData(GL_ARRAY_BUFFER, outNormals.size() * 3 * sizeof(float), normalArray, GL_STATIC_DRAW);
	glEnableVertexAttribArray(normalAttribute);
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void Shape::draw(const GLuint& program){
	//GLuint dColorID = glGetUniformLocation(program, "diffuseColor");
	//glUniform4fv(dColorID, 1, diffuseColor);

	GLuint translateID = glGetUniformLocation(program, "pos");
	glUniform3fv(translateID, 1, translate);

	GLuint scaleID = glGetUniformLocation(program, "size");
	glUniform3fv(scaleID, 1, scale);

	GLuint rotateID = glGetUniformLocation(program, "quaternion");
	glUniformMatrix4fv(rotateID, 1, true, rotate);

	glBindVertexArray(vao);
	if(face::quad){
		glDrawArrays(GL_QUADS, 0, outVertices.size() * 2);
	}
	else{
		glDrawArrays(GL_TRIANGLES, 0, outVertices.size());
	}
}