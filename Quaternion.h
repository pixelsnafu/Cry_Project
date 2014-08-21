#pragma once

#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#define TOLERANCE 0.00001f
#define PI 3.14159

#include <cmath>
#include "Vec3.h"

using namespace std;

class Quaternion{
    float x, y, z, w;

public:

    Quaternion(){}

    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w){}

    Quaternion(float angle, const Vec3f& axis): x(axis.x), y(axis.y), z(axis.z), w(angle){}

    static inline Quaternion FromAxisAngle(const Vec3f& axis, float angle){
        float theta = angle * (PI / 180);
        theta *= 0.5f;
        float sinTheta = sinf(theta);
        float cosTheta = cosf(theta);

        Vec3f v = axis;
        v.normalize();
        
        return Quaternion(cosTheta, v * sinTheta);
    }

    float* toMatrix() const{

        float x2 = x * x;
        float y2 = y * y;
        float z2 = z * z;
        float xy = x * y;
        float xz = x * z;
        float yz = y * z;
        float wx = w * x;
        float wy = w * y;
        float wz = w * z;

        float* matrix = new float[16];
        matrix[0] = 1.0f - 2.0f * (y2 + z2); matrix[4] = 2.0f * (xy + wz);          matrix[8] = 2.0f * (xz - wy);           matrix[12] = 0.0f;
        matrix[1] = 2.0f * (xy - wz);        matrix[5] = 1.0f - 2.0f * (x2 + z2);   matrix[9] = 2.0f * (yz + wx);           matrix[13] = 0.0f;
        matrix[2] = 2.0f * (xz + wy);        matrix[6] = 2.0f * (yz - wx);          matrix[10] = 1.0f - 2.0f * (x2 + y2);   matrix[14] = 0.0f;
        matrix[3] = 0.0f;                    matrix[7] = 0.0f;                      matrix[11] = 0.0f;                      matrix[15] = 1.0f;

        /*float matrix[] = { 1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
			               2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
			               2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
                           0.0f,             0.0f,             0.0f,                    1.0f};*/

        return matrix;
    }

    Quaternion& operator=(const Quaternion &q){

        w = q.w;
        x = q.x;
        y = q.y;
        z = q.z;

        return *this;
    }

    Quaternion operator+(const Quaternion& q) const{
        return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
    }

    Quaternion operator-(const Quaternion& q) const{
        return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w);
    }

    Quaternion operator-() const{
        return Quaternion(-x, -y, -z, -w);
    }

    Quaternion operator*(const Quaternion& q) const{

        float xx, yy, zz, ww;

        xx = w * q.getX() + x * q.getW() + y * q.getZ() - z * q.getY();
        yy = w * q.getY() + y * q.getW() + z * q.getX() - x * q.getZ();
        zz = w * q.getZ() + z * q.getW() + x * q.getY() - y * q.getX();
        ww = w * q.getW() - x * q.getX() - y * q.getY() - z * q.getZ();

        return Quaternion(xx, yy, zz, ww);
    }

    Quaternion operator* (float scale) const{
        return Quaternion(x * scale, y * scale, z * scale, w * scale);
    }

    Quaternion operator/(float scale) const{
        return Quaternion(x / scale, y / scale, z / scale, w / scale);
    }

    static Quaternion lerp(const Quaternion& q1, const Quaternion& q2, float t){
        Quaternion temp = q1 * (1-t) + q2 * t;
        temp.normalize();
        return temp;
    }

    static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t){
        Quaternion q2new;
        float dotProd = Quaternion::dot(q1, q2);
        if(dotProd < 0){
            dotProd = -dotProd;
            q2new = -q2;
        }else 
            q2new = q2;

        if(dotProd < 0.95){
            float theta = acosf(dotProd);
            return (q1 * sinf(theta*(1-t)) + q2new * sinf(theta*t))/sinf(theta);
        }else
            return lerp(q1, q2new, t);

    }

    void normalize(){

        float magnitude = x * x + y * y + z * z + w * w;

        if(fabs(magnitude) > TOLERANCE && fabs(magnitude - 1.0) > TOLERANCE){

            float mag = sqrt(magnitude);
            x /= mag;
            y /= mag;
            z /= mag;
            w /= mag;

        }
    }

    Quaternion getConjugate(){
        return Quaternion(-x, -y, -z, w);
    }

    static inline float dot(const Quaternion& q1, const Quaternion& q2){
        return (q1.getX() * q2.getX() + q1.getY() * q2.getY() + q1.getZ() * q2.getZ() + q1.getW() * q2.getW());
    }

    friend ostream& operator<<(ostream& o, const Quaternion& q){
        o<<q.getX()<<" "<<q.getY()<<" "<<q.getZ()<<" "<<q.getW()<<endl;
        return o;
    }

    float getW() const{
        return w;
    }

    float getX() const{
        return x;
    }

    float getY() const{
        return y;
    }

    float getZ() const{
        return z;
    }

    void setW(float w){
        this->w = w;
    }

    void setX(float x){
        this->x = x;
    }

    void setY(float y){
        this->y = y;
    }

    void setZ(float z){
        this->z = z;
    }
};

#endif