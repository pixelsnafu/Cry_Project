#pragma once

#ifndef _3DSHAPE_H_
#define _3DSHAPE_H_


#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include "Vec3.h"
#include "Quaternion.h"
#include <algorithm>

using namespace std;

class Shape{
	float* scale;
	float* translate;
	float* rotate;

	float* diffuseColor;

	bool smooth;

	GLuint vao;
	GLuint vbo[10];
	GLuint vertexAttribute;
	GLuint normalAttribute;

	float* vertices;
	float* normalArray;
	float* vertexNormalArray;

	vector<Vec3f> points;
	vector<face> faces;
	vector<Vec3f> normals;
	vector<Vec3f> vertexNormals;
	map<Vec3f, vector<face>, Vec3Comp> vertexElements;

	vector<Vec3f> outVertices;
	vector<Vec3f> outNormals;
	vector<Vec3f> outVertexNormals;

	void createVertices();

public:

	Shape(GLuint vao, vector<Vec3f> points, vector<Vec3f> normals, vector<face> faces);
	Shape(vector<Vec3f> points, vector<Vec3f> normals, vector<face> faces);
	Shape(vector<Vec3f> points, vector<Vec3f> normals, vector<face> faces, map<Vec3f, vector<face>, Vec3Comp> vertexElements);
	void smoothNormals();
	void setDiffuseColor(float r, float g, float b);
	void setPosition(float x, float y, float z);
	void setSize(float x, float y, float z);
	void setRotate(float x, float y, float z, float theta);
	void setRotate(Quaternion q);
	void setVao(GLuint vao);
	float* getPosition();
	float* getSize();
	void initBuffers(const GLuint& program, const char* vertexAttributeName, const char* normalAttributeName);
	void draw(const GLuint& program);

};

#endif