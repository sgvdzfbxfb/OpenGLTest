#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

using namespace std;

int main()
{
	// glfw初始化
	glfwInit();
	// glfw用的版本，major为3 minor为3，所以版本为3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 开启glfw窗口代码
	GLFWwindow* window = glfwCreateWindow(800, 600, "Test Window", NULL, NULL);

	if (window == NULL)
	{
		cout << "Glfw Window Failed" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	// 配置glew代码
	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		cout << "Glew Window Failed" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glViewport(0, 0, 800, 600);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0, 0, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	cout << "ddd" << endl;
	return 0;
}