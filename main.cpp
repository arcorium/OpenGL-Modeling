#include "pch.h"

#include "Camera.h"
#include "Mesh.h"
#include "Model.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 1024

void FrameBufferCallback(GLFWwindow* win, int width, int height);
void KeyCallback(GLFWwindow* win, int key, int scancode, int action, int mods);
void MouseCallback(GLFWwindow* win, double x, double y);
void ScrollCallback(GLFWwindow* win, double xOffset, double yOffset);
void ClearScreen();

bool isFirst = true, isLine = false;
float lastX, lastY;
Camera cam{glm::vec3{0.0f, 0.0f, 0.0f}};
float deltaTime = 0.0f;
glm::vec3 lightPosition = glm::vec3{ 0.0f, 0.0f, -9.0f };

int main()
{
	float cubeVertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	// Init GLFW
	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, 0);

	GLFWwindow* window = glfwCreateWindow(1024, 768, "TEST", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Init GLAD
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		return -2;
	}

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(window, FrameBufferCallback);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	// Init IMGUI
	IMGUI_CHECKVERSION();
	// Make context
	auto* imContext = ImGui::CreateContext();
	// Init for OpenGL and GLFW
	ImGui_ImplGlfw_InitForOpenGL(window, false);
	ImGui_ImplOpenGL3_Init("#version 460 core");
	// Set dark style
	ImGui::StyleColorsDark();


	Shader lightShader("Light", "V:\\Code\\C++\\OpenGL-Modeling\\light.vs", "V:\\Code\\C++\\OpenGL-Modeling\\light.fs");

	uint32_t VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	uint32_t VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<const void*>(0));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	glm::vec3 lightColor = glm::vec3{1.0f, 1.0f, 1.0f};
	glm::mat4 lightModel = glm::translate(glm::mat4{1.0f}, glm::vec3{0.0f, 0.0f, -5.0f});


	Shader shader{"Shader", "V:\\Code\\C++\\OpenGL-Modeling\\shader.vs", "V:\\Code\\C++\\OpenGL-Modeling\\shader.fs"};
	shader.Use();

	Model mods{"V:\\Materials\\3D\\Backpack\\backpack.obj"};

	glm::mat4 model = glm::translate(glm::mat4{1.0f}, glm::vec3{0.0f, 0.0f, -10.0f});
	glm::mat4 view = cam.GetView();
	glm::mat4 projection = glm::perspective(glm::radians(cam.GetZoom()), 1024.0f / 768.0f, 0.1f, 100.0f);

	float lastTime = 0.0f;
	float cumulativeTime = 0.0f;

	glfwShowWindow(window);
	while (!glfwWindowShouldClose(window))
	{
		// Timing
		const float currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		cumulativeTime += deltaTime;
		// Updating
		view = cam.GetView();
		projection = glm::perspective(glm::radians(cam.GetZoom()), 1024.0f / 768.0f, 0.1f, 100.0f);

		glfwPollEvents();

		// ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//ImGui::SetWindowPos(ImVec2{ 0.0f, 0.0f });
		ImGui::Begin("Setting");

		ImGui::Text("Position");
		ImGui::DragFloat("X", &lightPosition.x, 0.1f, -100.0f, 100.0f);
		ImGui::DragFloat("Y", &lightPosition.y, 0.1f, -100.0f, 100.0f);
		ImGui::DragFloat("Z", &lightPosition.z, 0.1f, -100.0f, 100.0f);

		ImGui::End();


		ClearScreen();

		shader.Use();

		//shader.SetUniform("camera", cam.GetMatrix());
		shader.SetUniform("projection", projection);
		shader.SetUniform("view", view);
		shader.SetUniform("model", model);

		shader.SetUniform("material.shininess", 32.0f);

		// Light
		shader.SetUniform("light.position", lightPosition);
		shader.SetUniform("light.ambient", glm::vec3{0.1f, 0.1f, 0.1f});
		shader.SetUniform("light.diffuse", glm::vec3{1.0f, 1.0f, 1.0f});
		shader.SetUniform("light.specular", glm::vec3{1.0f, 1.0f, 1.0f});

		// Attenuation
		shader.SetUniform("light.constant", 1.0f);
		shader.SetUniform("light.linear", 0.09f);
		shader.SetUniform("light.quadratic", 0.032f);


		shader.SetUniform("viewPosition", cam.GetPosition());

		mods.Draw(shader);

		// Draw Light
		lightShader.Use();
		lightModel = glm::mat4{ 1.0f };
		
		lightModel = glm::translate(lightModel, lightPosition);
		lightModel = glm::scale(lightModel, glm::vec3{ 0.2f });

		lightShader.SetUniform("Color", lightColor);
		lightShader.SetUniform("projection", projection);
		lightShader.SetUniform("view", view);
		lightShader.SetUniform("model", lightModel);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// ImGui Render
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (cumulativeTime >= 1.0f)
		{
			cumulativeTime = 0.0f;
			if (unsigned n = Shader::GetFalseUniform())
				Logger::Log(LogType::FAILED, "UNIFORM", "There is " + std::to_string(n) + " false uniform");
		}
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void FrameBufferCallback(GLFWwindow* win, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ClearScreen()
{
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void KeyCallback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	if (scancode == glfwGetKeyScancode(GLFW_KEY_F1))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (scancode == glfwGetKeyScancode(GLFW_KEY_F2))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (scancode == glfwGetKeyScancode(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(win, 1);

	if (scancode == glfwGetKeyScancode(GLFW_KEY_F3))
		glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


	// Object Movement
	switch (key)
	{
	case GLFW_KEY_UP:
		if (mods == GLFW_MOD_SHIFT)
			lightPosition.y += 0.1f;
		else
			lightPosition.z -= 0.1f;
		break;
	case GLFW_KEY_DOWN:
		if (mods == GLFW_MOD_SHIFT)
			lightPosition.y -= 0.1f;
		else
			lightPosition.z += 0.1f;
		break;
	case GLFW_KEY_LEFT:
		lightPosition.x -= 0.1f;
		break;
	case GLFW_KEY_RIGHT:
		lightPosition.x += 0.1f;
		break;
	}

	// Camera Movement
	switch (key)
	{
	case GLFW_KEY_W:
		cam.KeyInput(CameraMovement::FORWARD, deltaTime);
		break;
	case GLFW_KEY_A:
		cam.KeyInput(CameraMovement::LEFT, deltaTime);
		break;
	case GLFW_KEY_S:
		cam.KeyInput(CameraMovement::BACKWARD, deltaTime);
		break;
	case GLFW_KEY_D:
		cam.KeyInput(CameraMovement::RIGHT, deltaTime);
		break;
	}
}


void MouseCallback(GLFWwindow* win, double x, double y)
{
	if (isFirst)
	{
		lastX = static_cast<float>(x);
		lastY = static_cast<float>(y);
		isFirst = false;
	}

	float xOffset = static_cast<float>(x) - lastX;
	float yOffset = static_cast<float>(y) - lastY;

	lastX = x;
	lastY = y;

	cam.CursorInput(xOffset, yOffset);
}

void ScrollCallback(GLFWwindow* win, double xOffset, double yOffset)
{
	cam.WheelInput(yOffset);
}
