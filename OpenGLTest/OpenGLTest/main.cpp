#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include "shader.h"
#include "camera.h"
#include "ImportedModel.h"
#include <fstream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

#define numVAOs 2
#define numVBOs 6

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

ImportedModel myModel("./obj/spot.obj");
ImportedModel myModel2("./obj/diablo3.obj");

void setupVertices(ImportedModel myModel, int mid) {
	std::vector<glm::vec3> vert = myModel.getVertices();
	std::vector<glm::vec3> norm = myModel.getNormals();
	std::vector<glm::vec2> tex = myModel.getTexures();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	for (int i = 0; i < myModel.getNumVertices(); i++) {
		pvalues.push_back((vert[i]).x);
		pvalues.push_back((vert[i]).y);
		pvalues.push_back((vert[i]).z);
		tvalues.push_back((tex[i]).x);
		tvalues.push_back((tex[i]).y);
		nvalues.push_back((norm[i]).x);
		nvalues.push_back((norm[i]).y);
		nvalues.push_back((norm[i]).z);
	}
	glBindVertexArray(vao[mid]);

	// position
	glBindBuffer(GL_ARRAY_BUFFER, vbo[mid * 3 + 0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// texture
	glBindBuffer(GL_ARRAY_BUFFER, vbo[mid * 3 + 1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// normal
	glBindBuffer(GL_ARRAY_BUFFER, vbo[mid * 3 + 2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 900;

//���Ų���
float big = 0.6f;//�Ŵ�
float small = 0.6f;//��С

//ƽ�Ʋ���
float front_back = 0;
float left_right = 0;
float up_down = 0;

//��ת����
float rotate_x = 0.0;
float rotate_y = 0.0;
float rotate_z = 0.0;

//�߿�ͼת����־
int line_flag = 0;

// camera�ӽǱ仯�������
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

int main()
{
	// glfw��ʼ��
	glfwInit();
	// glfw�õİ汾��majorΪ3 minorΪ3�����԰汾Ϊ3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//����GLFW����ʹ�õ��Ǻ���ģʽ(Core-profile)

	// ����glfw���ڴ���
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Test Window", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Glfw Window Failed" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return EXIT_FAILURE;
	}
	Shader lightingShader("./shader/1.colors.vert", "./shader/1.colors.frag");
	Shader lightingShader2("./shader/2.colors.vert", "./shader/2.colors.frag");
	glGenVertexArrays(numVAOs, vao);
	glGenBuffers(numVBOs, vbo);
	setupVertices(myModel, 0);
	setupVertices(myModel2, 1);

	////��һ������
	//unsigned int texture;
	//glGenTextures(1, &texture);
	//glBindTexture(GL_TEXTURE_2D, texture);
	////���û���
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	////���˷�ʽ
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	////���ز���������
	//int texWidth, texHeight, nrChannels;	//numberChannel:ͨ����
	//stbi_set_flip_vertically_on_load(true);
	//unsigned char* texData = stbi_load("./obj/spot_diffuse.png", &texWidth, &texHeight, &nrChannels, 0);
	//if (texData)
	//{
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else cout << "Failed to load texture" << endl;
	//stbi_image_free(texData);
	//lightingShader.setInt("ourTexture", 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//�߿�ͼת�������ж�
		if (line_flag == 0) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ��1�������shaderʹ�úͲ������ã���ʹ�������ã�˳���ܷ�
		lightingShader.use();
		float timeControl = (float)glfwGetTime();
		lightingShader.setFloat("iTime", timeControl);
		deltaTime = timeControl - lastFrame;
		lastFrame = timeControl;
		timeControl = (float)glfwGetTime();//����timeControlϵͳʱ�亯��
		float radius = 10.0f;
		float camX = sin(timeControl) * radius;
		float camZ = cos(timeControl) * radius;//����X��Z����ʱ������Ǻ�����ʵ�ֹ�Դ����ת
		glm::vec3 lightPos(camX, 0.0f, camZ);//���ù�Դ��ά��������ת
		glm::vec3 lightPos_pro(-camX, 0.0f, -camZ);//���ù�Դ��ά��������ת
		lightingShader.setVec3("objectColor", 0.6f, 0.84f, 0.85f);
		lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec2("iResolution", glm::vec2((float)SCR_WIDTH, (float)SCR_HEIGHT));
		lightingShader.setVec3("lightPos", lightPos);
		lightingShader.setVec3("lightPos_pro", lightPos_pro);
		lightingShader.setVec3("viewPos", camera.Position);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		//����
		model = glm::scale(model, glm::vec3(big + small, big + small, big + small));
		//ƽ��
		model = glm::translate(model, glm::vec3(left_right, up_down, front_back));
		//��ת
		model = glm::rotate(model, glm::radians(rotate_z), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotate_y), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotate_x), glm::vec3(0.0f, 0.0f, 1.0f));
		lightingShader.setMat4("model", model);
		glBindVertexArray(vao[0]);
		glDrawArrays(GL_TRIANGLES, 0, myModel.getNumVertices());
		//glDrawArraysInstanced(GL_TRIANGLES, 0, myModel.getNumVertices(), 1);


		// ��2�������shaderʹ�úͲ������ã���ʹ�������ã�˳���ܷ�
		lightingShader2.use();
		timeControl = (float)glfwGetTime();
		lightingShader2.setFloat("iTime", timeControl);
		lightingShader2.setVec3("objectColor", 0.6f, 0.84f, 0.85f);
		lightingShader2.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightingShader2.setVec2("iResolution", glm::vec2((float)SCR_WIDTH, (float)SCR_HEIGHT));
		lightingShader2.setVec3("lightPos", lightPos);
		lightingShader2.setVec3("lightPos_pro", lightPos_pro);
		lightingShader2.setVec3("viewPos", camera.Position);
		lightingShader2.setMat4("projection", projection);
		lightingShader2.setMat4("view", view);
		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(0.5, 0.5, 0.5));
		lightingShader2.setMat4("model2", model2);
		glBindVertexArray(vao[1]);
		glDrawArrays(GL_TRIANGLES, 0, myModel2.getNumVertices());
		//glDrawArraysInstanced(GL_TRIANGLES, 0, myModel2.getNumVertices(), 2);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_Z) big += 0.03;//ģ�ͷŴ�

	if (key == GLFW_KEY_X) small -= 0.03;//ģ����С

	if (key == GLFW_KEY_G) front_back += 0.3;//ģ��ǰ��

	if (key == GLFW_KEY_T) front_back -= 0.3;//ģ�ͺ���

	if (key == GLFW_KEY_H) left_right += 0.3;//ģ������

	if (key == GLFW_KEY_F) left_right -= 0.3;//ģ������

	if (key == GLFW_KEY_R) up_down += 0.3;//ģ������

	if (key == GLFW_KEY_Y) up_down -= 0.3;//ģ������

	if (key == GLFW_KEY_I) rotate_y += 10.0;//ģ����ǰ��ת

	if (key == GLFW_KEY_K) rotate_y -= 10.0;//ģ�������ת

	if (key == GLFW_KEY_J) rotate_z += 10.0;//ģ��������ת

	if (key == GLFW_KEY_L) rotate_z -= 10.0;//ģ��������ת

	if (key == GLFW_KEY_U) rotate_x += 10.0;//ģ��˳ʱ����ת

	if (key == GLFW_KEY_O) rotate_x -= 10.0;//ģ����ʱ����ת

	if (key == GLFW_KEY_V) line_flag = 0;//ת��Ϊ��ͼ

	if (key == GLFW_KEY_B) line_flag = 1;//ת��Ϊ�߿�ͼ
}