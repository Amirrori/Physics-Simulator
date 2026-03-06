// source https://antongerdelan.net/opengl/hellotriangle.html
// source https://moderncprogramming.com/loading-a-glsl-shader-from-file-in-opengl-using-pure-c/
#include <stdio.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// loading shaders

void error_callback_glfw(int error, const char* description) {
	fprintf( stderr, "GLFW ERROR: code %i msg: %s. \n", error, description);
}

char* get_shader_content(const char* fileName)
{
	FILE *fp;
	long size = 0;
	char* shaderContent;

	// Read file size
	fp = fopen(fileName, "rb");
	if(fp==NULL) {
		return "";
	}

	fseek(fp, 0L, SEEK_END);
	size = ftell(fp) + 1;
	fclose(fp);

	// Read file for content
	//
	fp = fopen(fileName, "r");
	shaderContent = memset(malloc(size), '\0', size);
	if(fread(shaderContent, 1, size-1, fp) < size-1)
	{
		return "";
	}

	fclose(fp);

	return shaderContent;
}

GLFWwindow* initgl() {
	
	// INITIALIZATION PROCESS
	printf("Starting GLFW %s. \n", glfwGetVersionString());
	 glfwSetErrorCallback( error_callback_glfw );
	// start context with glfw lib
	if ( !glfwInit()) {
		fprintf( stderr, "ERROR: could not start GLFW3. \n");
		exit(0);
	}
	// Version information
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	// Multisample Initialising (MSAA)
	glfwWindowHint( GLFW_SAMPLES, 8 );
	
	bool full_screen = false;

	GLFWmonitor *mon = NULL;
	int win_w = 800;
	int win_h = 600;

	if ( full_screen ) {
		mon = glfwGetPrimaryMonitor();

		const GLFWvidmode* mode = glfwGetVideoMode(mon);

		glfwWindowHint( GLFW_RED_BITS, mode->redBits );
		glfwWindowHint( GLFW_GREEN_BITS, mode->greenBits );
		glfwWindowHint( GLFW_BLUE_BITS, mode->blueBits );
		glfwWindowHint( GLFW_REFRESH_RATE, mode->refreshRate );

		win_w = mode->width;  // Use our 'desktop' resolution for window size
		win_h = mode->height; // to get a 'full screen borderless' window.
	}

	// Window Creation
	GLFWwindow* window = glfwCreateWindow( win_w, win_h, "Physics Simulator", mon, NULL);

	if (!window) {
		fprintf( stderr, "ERROR: Could not open window with GLFW3.\n" );
		glfwTerminate();
		exit(0);
	}
	glfwMakeContextCurrent(window);


	int version_glad = gladLoadGL (glfwGetProcAddress);

	if (version_glad == 0) {
		fprintf( stderr, "ERROR: Failed to initialize OpenGL context.\n" );
		exit(0);
	}

	glViewport(0, 0, 800, 600);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	printf("Loaded OpenGL %i. %i\n", GLAD_VERSION_MAJOR( version_glad ), GLAD_VERSION_MINOR( version_glad) );


	printf( "Renderer: %s.\n", glGetString( GL_RENDERER ) );
	printf( "OpenGL version supported %s.\n", glGetString( GL_VERSION ) );


	return window;
}
//int vOPS(GLuint vbo, GLuint vao, float points[] ) {
//
//
//	glGenBuffers(1, &vbo);
//	glBindBuffer( GL_ARRAY_BUFFER, vbo);
//	glBufferData( GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW );
//
//	glGenVertexArrays( 1, &vao );
//	glBindVertexArray( vao );
//	glEnableVertexAttribArray( 0 );
//	glBindBuffer( GL_ARRAY_BUFFER, vbo );
//	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
//
//}

int vOPS(GLuint *vbo, GLuint *vao, float points[] ) {


	glGenBuffers(1, vbo);
	glBindBuffer( GL_ARRAY_BUFFER, *vbo);
	glBufferData( GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW );

	glGenVertexArrays( 1, vao );
	glBindVertexArray( *vao );
	glEnableVertexAttribArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, *vbo );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );

}


int main(void) {

	printf("hello world!\n");

	GLFWwindow* window = initgl();
	// We do our stuff here
	
	// Points of a triangle [placeholder for now]
	float points[] = {
		-0.5f,  0.5f,  0.0f, // x,y,z of first point.
		0.5f, -0.5f,  0.0f, // x,y,z of second point.
		-0.5f, -0.5f,  0.0f  // x,y,z of third point.
	};


	float points2[] = {
		-0.5f, 0.5f, 0.0f, // first
		0.5f, 0.5f, 0.0f, // second
		0.5f, -0.5f, 0.0f // third
	};


	// Vertex Buffer Object (vbo)
	
	GLuint vbo = 0;
	GLuint vbo2 = 0;
	GLuint vao = 0;
	GLuint vao2 = 0;
	
	vOPS(&vbo, &vao, points);
	vOPS(&vbo2, &vao2, points2);

	
	// vertex array object (vao)

//	glGenBuffers(1, &vbo);
//	glBindBuffer( GL_ARRAY_BUFFER, vbo);
//	glBufferData( GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW );
//
//	glGenVertexArrays( 1, &vao );
//	glBindVertexArray( vao );
//	glEnableVertexAttribArray( 0 );
//	glBindBuffer( GL_ARRAY_BUFFER, vbo );
//	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
//
//
//	glGenBuffers(1, &vbo2);
//	glBindBuffer( GL_ARRAY_BUFFER, vbo2);
//	glBufferData( GL_ARRAY_BUFFER, 9 * sizeof(float), points2, GL_STATIC_DRAW );
//
//	glGenVertexArrays( 1, &vao2 );
//	glBindVertexArray( vao2 );
//	glEnableVertexAttribArray( 0 );
//	glBindBuffer( GL_ARRAY_BUFFER, vbo2 );
//	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	// second shape?
	//
	// shadering(colour the triangle) moved to imports
	const char* vertex_shader = get_shader_content("trig.vert");
	const char* fragment_shader = get_shader_content("trig.frag");
	const char* fragment_shader2 = get_shader_content("trig2.frag"); 

	GLuint vs = glCreateShader( GL_VERTEX_SHADER);
	glShaderSource ( vs, 1, &vertex_shader, NULL);
	glCompileShader( vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource( fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);


	GLuint fs2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource( fs2, 1, &fragment_shader2, NULL);
	glCompileShader(fs2);



	// create an empty gpu executable program
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);
	
	GLuint shader_program2 = glCreateProgram();
	glAttachShader(shader_program2, fs2);
	glAttachShader(shader_program2, vs);
	glLinkProgram(shader_program2);
	// draw in a loop
	double prev_s = glfwGetTime();
	double title_countdown_s = 0.1;
	while (!glfwWindowShouldClose(window)) {
		double curr_s = glfwGetTime(); //get current time
		double elapsed_s = curr_s - prev_s; // workout the time elapsed over the last frame
		prev_s = curr_s; // Set the "previous time" for the next frame to use.

		// print the FPS, but not every frame, to not cause flickering.
		title_countdown_s -= elapsed_s;
		
		if (title_countdown_s <= 0.0 && elapsed_s > 0.0 ) {
			double fps = 1.0 / elapsed_s;

			// create a string and put the FPS as the window title.
			//
			char tmp[256];
			sprintf(tmp, "FPS %.2lf", fps);
			glfwSetWindowTitle(window, tmp);
			title_countdown_s = 0.1;
		//update window events.
		}
		
		glfwPollEvents();
		
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, 1);
		}

		// Check if the window resized.
//		glfwGetWindowSize( window, &win_w, &win_h );
   		// Update the viewport (drawing area) to fill the window dimensions.
//		glViewport( 0, 0, win_w, win_h );

		// Wipe the drawing surface clear

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Put the shader program, and the VAO, in focus of OpenGL's state machine
		glUseProgram(shader_program);
		glBindVertexArray(vao);

		// Draw points 0-3 from the currently bound VAO with the current in-use shader.
//		glPointSize(10.0f);
//		glDrawArrays(GL_POINTS, 0, 6);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glUseProgram(shader_program2);
		glBindVertexArray(vao2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glfwSwapBuffers(window);
		


	}

	// Close OpenGL window, context, and any other GLFW resources.
	glfwTerminate();

//	free(vertex_shader);
//	free(fragment_shader);
//	free(fragment_shader2);
	
	return 0;
}

// note to self: turning things into functions really troubled me but after learning more about pointers and memory addresses I realized how I could rectify that.
