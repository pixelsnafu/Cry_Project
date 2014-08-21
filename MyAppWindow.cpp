#include "MyAppWindow.h"

MyAppWindow::MyAppWindow(int W, int H, const char* L) : Fl_Window(W, H, L){
	myGL = new MyGlWindow(10, 10, w() - 20, h() - 200);

	ip_address = new Fl_Input(100, h() - 180, w() / 4, 20, "IP Address");
	ip_address->value(DEFAULT_IPADDR);

	stream_check = new Fl_Check_Button(300, h() - 180, w() / 4, 20, "Stream from Server");
	stream_check->callback(static_callback, (void*) ip_address);

	left_smile = new Fl_Value_Slider(120, h() - 160, w() / 2, 20, "Left Smile");
	left_smile->align(FL_ALIGN_LEFT);
	left_smile->type(FL_HOR_NICE_SLIDER);
	left_smile->range(0.0, 1.0);
	left_smile->value(0.0);
	left_smile->callback(valueChanged_callback, (void*) this);
	sliders.push_back(left_smile);

	right_smile = new Fl_Value_Slider(120, h() - 140, w() / 2, 20, "Right Smile");
	right_smile->align(FL_ALIGN_LEFT);
	right_smile->type(FL_HOR_NICE_SLIDER);
	right_smile->range(0.0, 1.0);
	right_smile->value(0.0);
	right_smile->callback(valueChanged_callback, (void*) this);
	sliders.push_back(right_smile);

	browsup_slider = new Fl_Value_Slider(120, h() - 120, w() / 2, 20, "Brows Up");
	browsup_slider->align(FL_ALIGN_LEFT);
	browsup_slider->type(FL_HOR_NICE_SLIDER);
	browsup_slider->range(0.0, 1.0);
	browsup_slider->value(0.0);
	browsup_slider->callback(valueChanged_callback, (void*) this);
	sliders.push_back(browsup_slider);

	mouthopen_slider = new Fl_Value_Slider(120, h() - 100, w() / 2, 20, "Mouth Open");
	mouthopen_slider->align(FL_ALIGN_LEFT);
	mouthopen_slider->type(FL_HOR_NICE_SLIDER);
	mouthopen_slider->range(0.0, 1.0);
	mouthopen_slider->value(0.0);
	mouthopen_slider->callback(valueChanged_callback, (void*) this);
	sliders.push_back(mouthopen_slider);

	puff_slider = new Fl_Value_Slider(120, h() - 80, w() / 2, 20, "Puff");
	puff_slider->align(FL_ALIGN_LEFT);
	puff_slider->type(FL_HOR_NICE_SLIDER);
	puff_slider->range(0.0, 1.0);
	puff_slider->value(0.0);
	puff_slider->callback(valueChanged_callback, (void*) this);
	sliders.push_back(puff_slider);

	sneer_slider = new Fl_Value_Slider(120, h() - 60, w() / 2, 20, "Sneer");
	sneer_slider->align(FL_ALIGN_LEFT);
	sneer_slider->type(FL_HOR_NICE_SLIDER);
	sneer_slider->range(0.0, 1.0);
	sneer_slider->value(0.0);
	sneer_slider->callback(valueChanged_callback, (void*) this);
	sliders.push_back(sneer_slider);

	kiss_slider = new Fl_Value_Slider(120, h() - 40, w() / 2, 20, "Kiss");
	kiss_slider->align(FL_ALIGN_LEFT);
	kiss_slider->type(FL_HOR_NICE_SLIDER);
	kiss_slider->range(0.0, 1.0);
	kiss_slider->value(0.0);
	kiss_slider->callback(valueChanged_callback, (void*) this);
	sliders.push_back(kiss_slider);

	valueChanged();
}

void MyAppWindow::valueChanged(){
	vector<float> uniforms;
	uniforms.push_back(left_smile->value());
	uniforms.push_back(right_smile->value());
	uniforms.push_back(browsup_slider->value());
	uniforms.push_back(mouthopen_slider->value());
	uniforms.push_back(puff_slider->value());
	uniforms.push_back(sneer_slider->value());
	uniforms.push_back(kiss_slider->value());
	myGL->setUniforms(uniforms);
}

void MyAppWindow::checkbutton_cb(Fl_Widget* w, Fl_Input* target){
	Fl_Check_Button* cb = ((Fl_Check_Button*)w);
	if(cb->value() == 1){
		//target->value("checked");
		if(myGL->connectToServer(target->value(), DEFAULT_PORT)){
			myGL->addTimeout();
			myGL->setSliders(sliders);
		}
	}
	else{
		myGL->disconnect();
	}
}

void MyAppWindow::setSliders(vector<GLfloat> uniforms){
	for(int i = 0; i < sliders.size(); i++){
		sliders.at(i)->value(uniforms.at(i));
	}
}

