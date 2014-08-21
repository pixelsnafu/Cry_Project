#pragma once

#ifndef __MYAPPWINDOW__
#define __MYAPPWINDOW__

#include "MyGlWindow.h"


class MyAppWindow : public Fl_Window{
	MyGlWindow* myGL;
	Fl_Input* ip_address;
	Fl_Value_Slider* left_smile;
	Fl_Value_Slider* right_smile;
	Fl_Value_Slider* browsup_slider;
	Fl_Value_Slider* mouthopen_slider;
	Fl_Value_Slider* puff_slider;
	Fl_Value_Slider* sneer_slider;
	Fl_Value_Slider* kiss_slider;
	Fl_Check_Button* stream_check;
	vector<Fl_Value_Slider*> sliders;

	void valueChanged();

	static void valueChanged_callback(Fl_Widget*, void* userData){
		MyAppWindow* app = (MyAppWindow*)userData;
		app->valueChanged();
	}

	static void static_callback(Fl_Widget* w, void* data){
		Fl_Input* target = (Fl_Input*)data;
		MyAppWindow* app = (MyAppWindow*)target->parent();
		app->checkbutton_cb(w, target);
	}

	

public:

	MyAppWindow(int W, int H, const char* L = 0);
	void setSliders(vector<GLfloat> uniforms);
	void checkbutton_cb(Fl_Widget* w, Fl_Input* target);
};

#endif