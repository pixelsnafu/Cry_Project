#pragma once

#ifndef OBJPARSER_H
#define OBJPARSER_H

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include "Vec3.h"
#include "Shape.h"

using namespace std;

class ObjParser{
	vector<Vec3f> points;
	vector<Vec3f> normals;
	vector<Vec3f> uv;
	vector<face> faces;
	map<Vec3f, vector<face>, Vec3Comp> vertexElements;

	void clear(){
		points.clear();
		normals.clear();
		uv.clear();
		faces.clear();
		vertexElements.clear();
	}

	void addVertexElements(face f){
		Vec3f v0 = points.at(f.a);
		Vec3f v1 = points.at(f.b);
		Vec3f v2 = points.at(f.c);
		Vec3f v3;
		if(face::quad)
			v3 = points.at(f.d);

		if(vertexElements.find(v0) != vertexElements.end()){
			vector<face>& tempList = vertexElements.at(v0);
			tempList.push_back(f);
		}else{
			vector<face> temp;
			temp.push_back(f);
			vertexElements.insert(pair<Vec3f, vector<face> >(v0, temp));
		}

		if(vertexElements.find(v1) != vertexElements.end()){
			vector<face>& tempList = vertexElements.at(v1);
			tempList.push_back(f);
		}else{
			vector<face> temp;
			temp.push_back(f);
			vertexElements.insert(pair<Vec3f, vector<face> >(v1, temp));
		}

		if(vertexElements.find(v2) != vertexElements.end()){
			vector<face>& tempList = vertexElements.at(v2);
			tempList.push_back(f);
		}else{
			vector<face> temp;
			temp.push_back(f);
			vertexElements.insert(pair<Vec3f, vector<face> >(v2, temp));
		}

		if(face::quad){
			if(vertexElements.find(v3) != vertexElements.end()){
				vector<face>& tempList = vertexElements.at(v3);
				tempList.push_back(f);
			}else{
				vector<face> temp;
				temp.push_back(f);
				vertexElements.insert(pair<Vec3f, vector<face> >(v3, temp));
			}
		}
	}

public:

	Shape parse(const char* filename){
		clear();
		cout<<"Loading mesh: "<<filename<<endl;
		ifstream ifs(filename);
		if(ifs.fail()){
			cout<<"Error opening obj file!"<<endl;
			exit(0);
		}
		string line;
		char ss = ' ';
		while(getline(ifs, line)){

			if(line[0] == 'v' && line[1] == ' '){
				Vec3f v;
				sscanf(line.c_str(), "%c %f %f %f", &ss, &v.x, &v.y, &v.z);
				points.push_back(v);

			}else if(line[0] == 'v' && line[1] == 'n'){
				Vec3f v;
				sscanf(line.c_str(), "%c %f %f %f", &ss, &v.x, &v.y, &v.z);
				normals.push_back(v);
			}else if(line[0] == 'v' && line[1] == 't'){
				Vec3f v;
				sscanf(line.c_str(), "%c %f %f %f", &ss, &v.x, &v.y, &v.z);
				uv.push_back(v);
			}else if(line[0] == 'f'){
				int slash_cnt = count(line.begin(), line.end(), '/');
				face f;
				if(slash_cnt == 4){
					face::quad = true;
					sscanf(line.c_str(), "%c %d/%d %d/%d %d/%d %d/%d", &ss, &f.a, &f.uv_a, &f.b, &f.uv_b, &f.c, &f.uv_c, &f.d, &f.uv_d);
					f.a--; f.b--; f.c--; f.d--;
					f.uv_a--; f.uv_b--; f.uv_c--; f.uv_d--;

					Vec3f v0 = points.at(f.a);
					Vec3f v1 = points.at(f.b);
					Vec3f v2 = points.at(f.c);
					Vec3f v3 = points.at(f.d);

					Vec3f u0 = v1 - v0;
					Vec3f u1 = v2 - v0;
					Vec3f u2 = v3 - v0;

					Vec3f n1 = cross(u0, u1);
					Vec3f n2 = cross(u1, u2);
					Vec3f n = (n1 + n2)/2;
					n.normalize();

					auto n_position = find(normals.begin(), normals.end(), n);
					if(n_position == normals.end()){
						normals.push_back(n);
						f.n = normals.size() - 1;
					}else{
						f.n = n_position - normals.begin();
					}

					faces.push_back(f);

					addVertexElements(f);

				}

			}else{
				cout<<"This particular formal of Obj file is not supported at the moment."<<endl;
				exit(1);
			}

		}
		/*cout<<points.size()<<endl;
		cout<<uv.size()<<endl;
		cout<<faces.size()<<endl;
		cout<<normals.size()<<endl;
		cout<<vertexElements.size()<<endl;
		cout<<vertexElements.at(points.at(100)).size()<<endl;*/
		return Shape(points, normals, faces, vertexElements);
		ifs.close();
	}
};

#endif