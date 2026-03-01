// source https://antongerdelan.net/opengl/hellotriangle.html
// source https://moderncprogramming.com/loading-a-glsl-shader-from-file-in-opengl-using-pure-c/
#include <stdio.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <string.h>


// loading shaders

char* get_shader_content(const char* fileName)
{
	FILE *fp;
	long size = 0;
	char* shaderContent;

	// Read file shize
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
	fread(shaderContent, 1, size-1, fp);
	fclose(fp);

	return shaderContent;
}

int main(void) {
	// INITIALIZATION PROCESS

	printf("hello world!\n");
	// start context with glfw lib
	if ( !glfwInit()) {
		fprintf( stderr, "ERROR: could not start GLFW3. \n");
		return 1;
	}
	
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	GLFWwindow* window = glfwCreateWindow( 800, 600, "Hello Triangle", NULL, NULL);

	if (!window) {
		fprintf( stderr, "ERROR: Could not open window with GLFW3.\n" );
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);


	int version_glad = gladLoadGL (glfwGetProcAddress);

	if (version_glad == 0) {
		fprintf( stderr, "ERROR: Failed to initialize OpenGL context.\n" );
		return 1;
	}

	glViewport(0, 0, 800, 600);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	printf("Loaded OPenGL %i. %i\n", GLAD_VERSION_MAJOR( version_glad ), GLAD_VERSION_MINOR( version_glad) );


	printf( "Renderer: %s.\n", glGetString( GL_RENDERER ) );
	printf( "OpenGL version supported %s.\n", glGetString( GL_VERSION ) );
	// We do our stuff here
	
	// Points of a triangle
	float points[] = {
		-0.5f,  0.5f,  0.0f, // x,y,z of first point.
		0.5f, -0.5f,  0.0f, // x,y,z of second point.
		-0.5f, -0.5f,  0.0f,  // x,y,z of third point.
		-0.5f, 0.5f, 0.0f, // forth
		0.5f, 0.5f, 0.0f, // fith
		0.5f, -0.5f, 0.0f // sixth
	};

	// Vertex Buffer Object (vbo)
	
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer( GL_ARRAY_BUFFER, vbo);
	glBufferData( GL_ARRAY_BUFFER, 36 * sizeof(float), points, GL_STATIC_DRAW );
	
	// vertext array object (vao)
	GLuint vao = 0;
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	glEnableVertexAttribArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );

	// shadering(colour the triangle) moved to imports
	const char* vertex_shader = get_shader_content("trig.vert");
	const char* fragment_shader = get_shader_content("trig.frag");

	GLuint vs = glCreateShader( GL_VERTEX_SHADER);
	glShaderSource ( vs, 1, &vertex_shader, NULL);
	glCompileShader( vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource( fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	// create an empty gpu executable program
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);

	// draw in a loop
	
	while ( !glfwWindowShouldClose( window ) ) {
		//update window events.
		
		glfwPollEvents();

		// Wipe the drawing surface clear

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Put the shader program, and the VAO, in focus of OpenGL's state machine
		glUseProgram(shader_program);
		glBindVertexArray(vao);

		// Draw points 0-3 from the currently bound VAO with the current in-use shader.
		
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
	}

	// Close OpenGL window, context, and any other GLFW resources.
	glfwTerminate();
	return 0;
}
