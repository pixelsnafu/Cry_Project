#pragma once

#ifndef __MYGLWINDOW__
#define __MYGLWINDOW__

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Check_Button.H>
#include <gl/glew.h>
#include <FL/gl.h>
#include <iostream>
#include <vector>
#include "camera.h"
#include "Shape.h"
#include "ObjParser.h"
#include "fsbinarystream.h"

using namespace std;
using namespace fs;

static const GLfloat triangle[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f,  1.0f, 0.0f,
};

#define DEFAULT_PORT "33433"
#define DEFAULT_IPADDR "129.21.91.34"
#define DEFAULT_BUFLEN 1024


//display variables
#define VIEW_SIZE_WIDTH 640
#define VIEW_SIZE_HEIGHT 680

#define FULLSCREEN_WIDTH GetSystemMetrics(SM_CXSCREEN)
#define FULLSCREEN_HEIGHT GetSystemMetrics(SM_CYSCREEN)

const float FULLSCREEN_ASPECT_RATIO = (float)FULLSCREEN_WIDTH/(float)FULLSCREEN_HEIGHT;
const float VIEW_ASPECT_RATIO = (float)VIEW_SIZE_WIDTH/(float)VIEW_SIZE_HEIGHT;

const float l = -0.25;
const float r = 0.25;
const float t = 0.25;
const float b = -0.25;
const float n = 0.5;
const float f = 10000.5;

//light variables
const float lightPosition[4] = {1.0, 4.0, 250.0, 1.0};
const float lightColor[4] = {1.35, 1.35, 1.35, 1.0};

const char uniform_names[][50] = {"smile_l_w", "smile_r_w", "browsup_w", "mouthopen_w", "puff_w", "sneer_w", "kiss_w"};
#define NUM_BLENDSHAPES 7

#define SMOOTH 1

class MyGlWindow : public Fl_Gl_Window {

	GLhandleARB program;
	GLuint vao;
	GLuint vertexAttribute;
	GLuint vbo[5];
	vector<GLfloat> uniforms;
	vector<Shape> shapes;
	camera* cam;
	SOCKET ConnectSocket;
	int iResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen;
	fs::fsBinaryStream parserIn, parserOut;
	fs::fsMsgPtr msg;
	bool initGL;
	bool stream;
	vector<Fl_Value_Slider*> sliders;

	void draw();

	void resize(int X,int Y,int W,int H);

	void init();

public:
	// CONSTRUCTOR
	MyGlWindow(int X,int Y,int W,int H,const char*L=0);

	void setUniforms(vector<float> color);

	const char* readShaderSource(const char* shaderFile);

	void loadShader( GLhandleARB& program, const char* vertexFile, const char* fragmentFile );

	void printInfoLog(GLhandleARB obj);

	bool connectToServer(const char* IP, const char* PORT);

	void disconnect();

	void streamData();

	void setStream(bool value);

	vector<GLfloat> getUniforms();

	void addTimeout(){
		Fl::add_timeout(0, Timer_CB, (void*) this);
	}

	void setSliders(vector<Fl_Value_Slider*> sliders);

	static void Timer_CB(void* data){
		MyGlWindow* app = (MyGlWindow*) data;
		app->streamData();
		app->redraw();
		Fl::repeat_timeout(0 , Timer_CB, data);
	}
};


#endif