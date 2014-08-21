#include "MyGlWindow.h"
#include "fsbinarystream.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


MyGlWindow::MyGlWindow(int X,int Y,int W,int H,const char*L) : Fl_Gl_Window(X,Y,W,H,L) {
	program = 0;
	initGL = false;
	stream = false;
	ConnectSocket = INVALID_SOCKET;
	recvbuflen = DEFAULT_BUFLEN;
	for(int i = 0; i < NUM_BLENDSHAPES; i++){
		uniforms.push_back(0.0);
	}
}

void MyGlWindow::draw(){
	// First time? init viewport, etc.
	if (!valid() && !initGL) {
		valid(1);
		initGL = true;
		init();
	}
	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);

	cam->setupCamera(program);

	GLuint lPositionID = glGetUniformLocation(program, "lightPosition");
	glUniform4fv(lPositionID, 1, lightPosition);

	GLuint lColorID = glGetUniformLocation(program, "lightColor");
	glUniform4fv(lColorID, 1, lightColor);

	for(int i = 0; i < uniforms.size(); i++){
		glUniform1f(glGetUniformLocation(program, uniform_names[i]), uniforms.at(i));
	}

	shapes.at(0).draw(program);
}

void MyGlWindow::resize(int X,int Y,int W,int H) {
	Fl_Gl_Window::resize(X,Y,W,H);
	glViewport(0,0,W,H);
	float aspectRatio = (float)W/(float)H;
	cam->setAspectRatio(aspectRatio);
	redraw();
}

void MyGlWindow::setUniforms(vector<float> color){
	uniforms = color;
	redraw();
}

void MyGlWindow::init(){
	loadShader( program, "shader.vert", "shader.frag" );
	if(!program){
		cerr<<"Error setting up shaders!"<<endl;
		exit(1);
	}

	glGenVertexArrays(1, &vao);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0, 0.0, 0.33, 1.0);
	glCullFace(GL_BACK);

	float aspectRatio = (float)w()/(float)h();
	cam = new camera(l, r, t, b, n, f, aspectRatio);

	shared_ptr<ObjParser> parser = make_shared<ObjParser>();
	Shape s = parser->parse("Neutral.obj");
	s.setVao(vao);
	if(SMOOTH)
		s.smoothNormals();
	s.initBuffers(program, "vNeutral", "nNeutral");
	s.setRotate(0, 1, 0, 0);
	shapes.push_back(s);

	Shape s2 = parser->parse("28_MouthSmile_L.obj");
	s2.setVao(vao);
	if(SMOOTH)
		s2.smoothNormals();
	s2.initBuffers(program, "vSmile_L", "nSmile_L");
	s2.setRotate(0, 1, 0, 0);
	shapes.push_back(s2);

	Shape s3 = parser->parse("29_MouthSmile_R.obj");
	s3.setVao(vao);
	if(SMOOTH)
		s3.smoothNormals();
	s3.initBuffers(program, "vSmile_R", "nSmile_R");
	s3.setRotate(0, 1, 0, 0);
	shapes.push_back(s3);

	Shape s4 = parser->parse("16_BrowsU_C.obj");
	s4.setVao(vao);
	if(SMOOTH)
		s4.smoothNormals();
	s4.initBuffers(program, "vBrowsup", "nBrowsup");
	s4.setRotate(0, 1, 0, 0);
	shapes.push_back(s4);

	Shape s5 = parser->parse("21_JawOpen.obj");
	s5.setVao(vao);
	if(SMOOTH)
		s5.smoothNormals();
	s5.initBuffers(program, "vMouthopen", "nMouthopen");
	s5.setRotate(0, 1, 0, 0);
	shapes.push_back(s5);

	Shape s6 = parser->parse("45_Puff.obj");
	s6.setVao(vao);
	if(SMOOTH)
		s6.smoothNormals();
	s6.initBuffers(program, "vPuff", "nPuff");
	s6.setRotate(0, 1, 0, 0);
	shapes.push_back(s6);

	Shape s7 = parser->parse("44_Sneer.obj");
	s7.setVao(vao);
	if(SMOOTH)
		s7.smoothNormals();
	s7.initBuffers(program, "vSneer", "nSneer");
	s7.setRotate(0, 1, 0, 0);
	shapes.push_back(s7);

	Shape s8 = parser->parse("41_LipsPucker.obj");
	s8.setVao(vao);
	if(SMOOTH)
		s8.smoothNormals();
	s8.initBuffers(program, "vKiss", "nKiss");
	s8.setRotate(0, 1, 0, 0);
	shapes.push_back(s8);

}

const char* MyGlWindow::readShaderSource(const char* shaderFile){
	long lSize;
	char* buf;
	FILE* fp = fopen(shaderFile, "rb");
	if( fp == NULL )	// shaders not in current working directory
	{		
		printf("Couldn't find shader file\n");
		exit(1);
	}
	fseek(fp , 0 , SEEK_END);
	lSize = ftell(fp);
	rewind(fp);
	buf = (char*) malloc (lSize + 1);
	fread (buf,1,lSize,fp);
	buf[lSize] = '\0';
	fclose(fp);
	return buf;
}

void MyGlWindow::loadShader( GLhandleARB& program, const char* vertexFile, const char* fragmentFile ){
	// initialize glew
	int glewError = glewInit();
	if( glewError ) printf("Error: %s\n", glewGetErrorString(glewError));

	// create shader program object and both a vertex and fragment object
	program		= glCreateProgramObjectARB();
	GLhandleARB	vertex		= glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	GLhandleARB fragment	= glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	// read shaders into strings
	const char* vertexString = readShaderSource(vertexFile);
	const char* fragmentString = readShaderSource(fragmentFile);

	// give ogl the shader source string
	glShaderSourceARB(vertex, 1, &vertexString, NULL);
	glShaderSourceARB(fragment, 1, &fragmentString, NULL);

	// compile shaders
	glCompileShaderARB(vertex);
	printInfoLog(vertex);

	glCompileShaderARB(fragment);
	printInfoLog(fragment);

	// attach shaders to program object and link
	glAttachObjectARB(program, vertex);
	glAttachObjectARB(program, fragment);
	glLinkProgramARB(program);

	// display results
	GLint success;
	glGetObjectParameterivARB(program, GL_OBJECT_LINK_STATUS_ARB, &success);
	if( !success ) 
	{
		printf("%s and %s shader could not be linked!\n", vertexFile, fragmentFile );
		exit(-1); // bail out, display isn't going to happen at this point.
	}
	else
	{
		printf("%s and %s loaded and linked successfully.\n", vertexFile, fragmentFile );
	}
}

void MyGlWindow::printInfoLog(GLhandleARB obj){
	GLint infologLength = 0;
	GLsizei charsWritten  = 0;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
	ofstream ofs("shaderlog.txt", ios::app);
	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		ofs<<infoLog<<endl;
		free(infoLog);
	}

	ofs.close();
}

bool MyGlWindow::connectToServer(const char* IP, const char* PORT){
	int err = 0;
	WSADATA wsaData;
	WORD wVersionRequested;
	ConnectSocket = INVALID_SOCKET;
	//SOCKET ConnectSocket = INVALID_SOCKET;

	struct sockaddr_in clientService;


	wVersionRequested = MAKEWORD(2,2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		cout<<"WSAStartup Error!"<<endl;
		return false;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		WSACleanup();
		cout<<"Wsa Data wVersion Error!"<<endl;
		return false;
	}


	// Create a SOCKET for connecting to server
	ConnectSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) {	
		WSACleanup();
		cout<<"Socket connection error!"<<endl;
		return false;
	}

	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr( DEFAULT_IPADDR );
	clientService.sin_port = htons( atoi(DEFAULT_PORT) );

	iResult = connect( ConnectSocket, (SOCKADDR*) &clientService, sizeof(clientService));

	if ( iResult == SOCKET_ERROR) {
		cout<<iResult<<endl;
		closesocket (ConnectSocket);
		WSACleanup();
		cout<<"Socket error!"<<endl;
		return false;
	}



	if (ConnectSocket == INVALID_SOCKET) {
		closesocket(ConnectSocket);
		WSACleanup();
		cout<<"Invalid Socket!"<<endl;
		return false;
	}

	cout<<ConnectSocket<<endl;
	return true;
}

void MyGlWindow::disconnect(){
	closesocket(ConnectSocket);
	WSACleanup();
	Fl::remove_timeout(Timer_CB, (void*) this);
}

void MyGlWindow::streamData(){

	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if(iResult > 0){
		parserIn.received(iResult, recvbuf);
		if(msg = parserIn.get_message()){
			if(dynamic_cast<fs::fsMsgTrackingState*>(msg.get())){
				fs::fsMsgTrackingState *ts = dynamic_cast<fs::fsMsgTrackingState*>(msg.get());
				const fs::fsTrackingData &data = ts->tracking_data();
				//printf ("head translation: %f %f %f\n",data.m_headTranslation.x,data.m_headTranslation.y,data.m_headTranslation.z);
				//printf ("head rotation: %f %f %f %f\n",data.m_headRotation.x,data.m_headRotation.y,data.m_headRotation.z,data.m_headRotation.w);
				Quaternion q(data.m_headRotation.x, data.m_headRotation.y, data.m_headRotation.z, data.m_headRotation.w);
				shapes.at(0).setRotate(q);
				uniforms.at(0) = data.m_coeffs.at(28);
				uniforms.at(1) = data.m_coeffs.at(29);
				uniforms.at(2) = data.m_coeffs.at(16);
				uniforms.at(3) = data.m_coeffs.at(21);
				uniforms.at(4) = data.m_coeffs.at(45);
				uniforms.at(5) = data.m_coeffs.at(44);
				uniforms.at(6) = data.m_coeffs.at(41);
				for(int i = 0; i < uniforms.size(); i++){
					sliders.at(i)->value(uniforms.at(i));
				}
			}
		}
	}
	if(!parserIn.valid()) {
		printf("parser in invalid state\n");
		parserIn.clear();
	}
}

void MyGlWindow::setStream(bool value){
	stream = value;
}

vector<GLfloat> MyGlWindow::getUniforms(){
	return uniforms;
}

void MyGlWindow::setSliders(vector<Fl_Value_Slider*> sliders){
	this->sliders = sliders;
}
