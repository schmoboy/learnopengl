#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// Vertex Shader source
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
// Fragment Shader source
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main()
{
	// Initialize GLFW and set options
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Triangle coordinates
	GLfloat vertices[] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,  // Lower left
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,   // Lower right
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // top
	};

	// Create window and set as current context
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// setup the resize callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// inititalize GLAD
	gladLoadGL();

	// create and compile shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Create a shader program from the shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

	// can delete shaders after creting a program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Create Vertex Array Pbject and Vertex Buffer Object
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind the VAO
	glBindVertexArray(VAO);

	// Bind the VBO and set its data to the vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configure the VAO so OGL knows how to read the data and enable it
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// handle any input events
		processInput(window);

		// Set background color 
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clear background
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader program and bind the VAO data
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		// Draw the object
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// swap the back and front buffer
		glfwSwapBuffers(window);

		// poll for events
		glfwPollEvents();
	}

	// cleanup our buffers and program
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// cleanup GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}