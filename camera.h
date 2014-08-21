#pragma once

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/GL.h>
#include <iostream>

#define PI 3.14159

using namespace std; 

class camera{
    float l, r, t, b, n, f;
    float cameraPosition[3];
    float upVector[3];
    float lookAtPosition[3];
    int theta, phi;
    float aspectRatio;

public:
    
    camera(float l, float r, float t, float b, float n, float f, float aspectRatio){
        this->l = l * aspectRatio;
        this->r = r * aspectRatio;
        this->t = t;
        this->b = b;
        this->n = n;
        this->f = f;
        this->aspectRatio = aspectRatio;

        cameraPosition[0] = 0.0f;
        cameraPosition[1] =	0.0f;
        cameraPosition[2] = 275.0f;

        upVector[0] = 0.0f;
        upVector[1] = 1.0f;
        upVector[2] = 0.0f;

        lookAtPosition[0] = 0.0f;
        lookAtPosition[1] = 0.0f;
        lookAtPosition[2] = 0.0f;

        theta = 0;
        phi = 0;
    }

    void setupCamera(GLuint program){
        int leftID = glGetUniformLocation(program, "left");
        int rightID = glGetUniformLocation(program, "right");
        int topID = glGetUniformLocation(program, "top");
        int bottomID = glGetUniformLocation(program, "bottom");
        int nearID = glGetUniformLocation(program, "near");
        int farID = glGetUniformLocation(program, "far");

        int camLocID = glGetUniformLocation(program, "cameraPosition");
        int lookAtID = glGetUniformLocation(program, "lookAtPosition");
        int upID = glGetUniformLocation(program, "upVector");

        glUniform3fv(upID, 1, upVector);
        glUniform3fv(lookAtID, 1, lookAtPosition);
        glUniform3fv(camLocID, 1, cameraPosition);

        glUniform1f(leftID, l);
        glUniform1f(rightID, r);
        glUniform1f(topID, t);
        glUniform1f(bottomID, b);
        glUniform1f(nearID, n);
        glUniform1f(farID, f);

        //cout<<lookAtPosition[0]<<" "<<lookAtPosition[1]<<" "<<lookAtPosition[2]<<endl;
    }

    void moveCameraForward(){
        float moveDirection[] = {lookAtPosition[0] - cameraPosition[0], 
                                 lookAtPosition[1] - cameraPosition[1], 
                                 lookAtPosition[2] - cameraPosition[2]};

        normalize(moveDirection);

        cameraPosition[0] += moveDirection[0] * 0.1;
        cameraPosition[1] += moveDirection[1] * 0.1;
        cameraPosition[2] += moveDirection[2] * 0.1;

        lookAtPosition[0] += moveDirection[0] * 0.1;
        lookAtPosition[1] += moveDirection[1] * 0.1;
        lookAtPosition[2] += moveDirection[2] * 0.1;
    }

    void moveCameraBackward(){
        float moveDirection[] = {lookAtPosition[0] - cameraPosition[0], 
                                 lookAtPosition[1] - cameraPosition[1], 
                                 lookAtPosition[2] - cameraPosition[2]};

        normalize(moveDirection);

        cameraPosition[0] -= moveDirection[0] * 0.1;
        cameraPosition[1] -= moveDirection[1] * 0.1;
        cameraPosition[2] -= moveDirection[2] * 0.1;

        lookAtPosition[0] -= moveDirection[0] * 0.1;
        lookAtPosition[1] -= moveDirection[1] * 0.1;
        lookAtPosition[2] -= moveDirection[2] * 0.1;
    }

    void moveCameraLeft(){
        float view[] = {lookAtPosition[0] - cameraPosition[0], 
                        lookAtPosition[1] - cameraPosition[1], 
                        lookAtPosition[2] - cameraPosition[2]};
        float* moveDirection = crossProduct(view, upVector);
        normalize(moveDirection);

        cameraPosition[0] -= moveDirection[0] * 0.1;
        cameraPosition[1] -= moveDirection[1] * 0.1;
        cameraPosition[2] -= moveDirection[2] * 0.1;

        lookAtPosition[0] -= moveDirection[0] * 0.1;
        lookAtPosition[1] -= moveDirection[1] * 0.1;
        lookAtPosition[2] -= moveDirection[2] * 0.1;
    }

    void moveCameraRight(){
        float view[] = {lookAtPosition[0] - cameraPosition[0], 
                        lookAtPosition[1] - cameraPosition[1], 
                        lookAtPosition[2] - cameraPosition[2]};
        float* moveDirection = crossProduct(view, upVector);
        normalize(moveDirection);

        cameraPosition[0] += moveDirection[0] * 0.1;
        cameraPosition[1] += moveDirection[1] * 0.1;
        cameraPosition[2] += moveDirection[2] * 0.1;

        lookAtPosition[0] += moveDirection[0] * 0.1;
        lookAtPosition[1] += moveDirection[1] * 0.1;
        lookAtPosition[2] += moveDirection[2] * 0.1;
    }

    void cameraUpdateRotX(int x){
        theta += x;
        float radius = sqrt(pow(cameraPosition[0] - lookAtPosition[0], 2) + pow(cameraPosition[2] - lookAtPosition[2], 2));
        float angle = theta*2*PI/360;
        lookAtPosition[0] = cameraPosition[0] + radius*cos(angle);
        lookAtPosition[2] = cameraPosition[2] + radius*sin(angle);
    }

    void cameraUpdateRotY(int y){
        phi -= y;
        if(phi > -10)
            phi = -10;
        if(phi <= -170)
            phi = -170;
        float radius = sqrt(pow(cameraPosition[1] - lookAtPosition[1], 2) + pow(cameraPosition[2] - lookAtPosition[2], 2));
        float angle = phi*PI/180;
        lookAtPosition[1] = cameraPosition[1] + radius*cos(angle);
    }

    void setCameraPosition(const float position[]){
        cameraPosition[0] = position[0];
        cameraPosition[1] = position[1];
        cameraPosition[2] = position[2];
    }

    void setCameraLookAtPosition(const float position[]){
        lookAtPosition[0] = position[0];
        lookAtPosition[1] = position[1];
        lookAtPosition[2] = position[2];
    }

    void setAspectRatio(float aspectRatio){
        l = l/this->aspectRatio;
        r = r/this->aspectRatio;

        l *= aspectRatio;
        r *= aspectRatio;

        this->aspectRatio = aspectRatio;
    }

    float* getCamPosition(){
        return cameraPosition;
    }

    void normalize(float* vector){
        float magnitude = sqrt(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);

        vector[0] /= magnitude;
        vector[1] /= magnitude;
        vector[2] /= magnitude;
    }

    float* crossProduct(float* a, float* b){
        float* result = new float[3];
        result[0] = a[1]*b[2] - b[1]*a[2];
        result[1] = b[0]*a[2] - a[0]*b[2];
        result[2] = a[0]*b[1] - b[0]*a[1];
        return result;
    }

    float dotProduct(float* a, float* b){
        return (a[0]*b[0] + a[1]*b[1] + a[2]*b[2]);
    }
};

#endif