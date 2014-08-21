#pragma once

#ifndef _VEC3_H_
#define _VEC3_H_

#include <iostream>

#define PI 3.14159

template<typename T>
class Vec3{

public:
	T x, y, z;

	Vec3():x(T(0)), y(T(0)), z(T(0)){}
	Vec3(T xx):x(xx), y(xx), z(xx){}
	Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}

	T magnitude() const { 
		return sqrt(x * x + y * y + z * z); 
	}

	void normalize() { 
		T mag = magnitude(); 
		if (mag) 
			*this *= 1 / mag; 
	}

	Vec3<T> normalized(){
		T mag = magnitude();

		return *this / mag;
	}

	T dot(const Vec3<T> &v) const{ 
		return x * v.x + y * v.y + z * v.z; 
	}

	Vec3 cross(const Vec3<T> &v) const{
		return Vec3(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x);
	}

	Vec3 operator+ (const Vec3<T> &v) const{ 
		return Vec3<T>(x + v.x, y + v.y, z + v.z); 
	}

	Vec3& operator+= (const Vec3<T> &v){
		*this = *this + v;
		return *this;
	}

	Vec3 operator* (const T &val) const{ 
		return Vec3<T>(x * val, y * val, z * val); 
	}
	Vec3 operator/ (const T &val) const{ 
		T invVal = T(1) / val; 
		return Vec3<T>(x * invVal, y * invVal, z * invVal); 
	}

	Vec3& operator/= (const T &val) {
		*this = *this / val;
		return *this;
	}

	Vec3 operator/ (const Vec3<T> &v) const{ 
		return Vec3<T>(x / v.x, y / v.y, z / v.z); 
	}

	Vec3 operator* (const Vec3<T> &v) const{
		return Vec3<T>(x * v.x, y * v.y, z * v.z); 
	}

	bool operator==(const Vec3<T> &v) const{
		return (x == v.x && y == v.y && z == v.z);
	}

	Vec3 operator- (const Vec3<T> &v) const{
		return Vec3<T>(x - v.x, y - v.y, z - v.z); 
	}

	Vec3 operator- () const{
		return Vec3<T>(-x, -y, -z); 
	}

	Vec3& operator*=(const T& n){
		*this = *this * n;
		return *this;
	}

	friend Vec3<T> operator/ (const T& val, const Vec3<T>& v){
		return Vec3<T>(val/v.x, val/v.y, val/v.z);
	}

	friend Vec3<T> operator* (const T& val, const Vec3<T>& v){
		return Vec3<T>(val * v.x, val * v.y, val * v.z);
	}

	friend std::ostream& operator<<(std::ostream& os, const Vec3<T>& v){
		os << v.x << " " << v.y << " " << v.z << std::endl;
		return os;
	}
};

struct face{
    GLuint a, b, c, d, n,uv_a, uv_b, uv_c, uv_d;
	static bool quad;

	face(){
        this->a = -1;
        this->b = -1;
        this->c = -1;
		this->d = -1;
        this->n = -1;
		this->uv_a = -1;
		this->uv_b = -1;
		this->uv_c = -1;
		this->uv_d = -1;
    }

    face(GLuint a, GLuint b, GLuint c, GLuint n){
        this->a = a;
        this->b = b;
        this->c = c;
		this->d = -1;
        this->n = n;
		this->uv_a = -1;
		this->uv_b = -1;
		this->uv_c = -1;
		this->uv_d = -1;
    }

	face(GLuint a, GLuint b, GLuint c, GLuint uv_a, GLuint uv_b, GLuint uv_c, GLuint n){
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = -1;
		this->uv_a = uv_a;
		this->uv_b = uv_b;
		this->uv_c = uv_c;
		this->uv_d = -1;
		this->n = n;
	}

	face(GLuint a, GLuint b, GLuint c, GLuint d, GLuint uv_a, GLuint uv_b, GLuint uv_c, GLuint uv_d, GLuint n){
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
		this->uv_a = uv_a;
		this->uv_b = uv_b;
		this->uv_c = uv_c;
		this->uv_d = uv_d;
		this->n = n;
	}
};


template<typename T>
Vec3<T> cross(const Vec3<T> &va, const Vec3<T> &vb)
{
	return Vec3<T>(
		va.y * vb.z - va.z * vb.y,
		va.z * vb.x - va.x * vb.z,
		va.x * vb.y - va.y * vb.x);
}

template <typename T>
inline T dot(const Vec3<T>& a, const Vec3<T>& b){
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

typedef Vec3<float> Vec3f;

struct Vec3Comp{
	bool operator()(const Vec3f& v1, const Vec3f& v2){
		if(v1.x < v2.x){
			return true;
		}else if(v1.x > v2.x){
			return false;
		}else if(v1.x == v2.x){
			if(v1.y < v2.y)
				return true;
			else if(v1.y > v2.y)
				return false;
			else if(v1.y == v2.y){
				if(v1.z < v2.z)
					return true;
				else if(v1.z > v2.z)
					return false;
				else{
					return v1.z < v2.z;
				}
			}
		}
	}
};

#endif