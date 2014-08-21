#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Value_Slider.H>
#include <gl/glew.h>
#include <FL/gl.h>
#include "MyAppWindow.h"
#include "MyGlWindow.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// MAIN
int main() {
	MyAppWindow win(VIEW_SIZE_WIDTH, VIEW_SIZE_HEIGHT, "Cry");
	//MyGlWindow mygl(10, 10, win.w()-20, win.h()-20);
	//win.end();
	win.resizable(win);
	win.show();
	return(Fl::run());
}