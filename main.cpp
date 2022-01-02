#include "pch.h"

#include "Camera.h"
#include "Mesh.h"
#include "Model.h"

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
		// Position			  // Texture Coordinate
		// Back face
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right    
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right              
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left                
		// Front face
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right        
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left        
		// Left face
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left       
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		// Right face
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right      
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right          
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		// Bottom face          
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left        
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		// Top face
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right                 
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // bottom-left  
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f  // top-left 
	};

	float grassVertex[] =
	{
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};

	float quadVertices[] =
	{
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
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

	GLFWwindow* window = glfwCreateWindow(1024, 768, "TEST", nullptr, nullptr);

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Init GLAD
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		return -2;
	}


	// Enabling Depth test (z value)
	glEnable(GL_DEPTH_TEST);
	// Enabling Stencil test (which fragment is rendered to screen)
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	// Enabling Blending (Texture transparency or alpha)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Enabling Face Culling (Delete or Remove object face that not seen by the user)
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);		// Set which face to delete
	glFrontFace(GL_CW);			// Set the front face to Clockwise (default: counter clockwise)

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);




	glfwSwapInterval(1);
	// Callback
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<const void*>(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<const void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	unsigned grassVAO;
	glGenVertexArrays(1, &grassVAO);
	glBindVertexArray(grassVAO);
	// VBO
	unsigned grassVBO;
	glGenBuffers(1, &grassVBO);
	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertex), grassVertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<const void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	unsigned screenVAO;
	glGenVertexArrays(1, &screenVAO);
	glBindVertexArray(screenVAO);

	unsigned screenVBO;
	glGenBuffers(1, &screenVBO);
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<const void*>(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	glm::vec3 vegetation[] =
	{
		glm::vec3(-1.5f,  0.0f, -0.48f),
		glm::vec3(1.5f,  0.0f,  0.51f),
		glm::vec3(0.0f,  0.0f,  0.7f),
		glm::vec3(-0.3f,  0.0f, -2.3f),
		glm::vec3(0.5f,  0.0f, -0.6f)
	};

	glm::vec3 lightColor = glm::vec3{1.0f, 1.0f, 1.0f};
	glm::mat4 lightModel = glm::translate(glm::mat4{1.0f}, glm::vec3{0.0f, 0.0f, -5.0f});


	Shader shader{"Shader", "V:\\Code\\C++\\OpenGL-Modeling\\shader.vs", "V:\\Code\\C++\\OpenGL-Modeling\\shader.fs"};
	shader.Use();

	Model mods{R"(V:\Materials\3D\Backpack\backpack.obj)"};

	// Grass shader
	Shader grassShader{ "Grass", "grass.vs", "grass.fs" };
	grassShader.Use();
	grassShader.SetUniform("tex", 1);
	grassShader.DisUse();

	Shader screenShader{ "framebuffer", "framebuffer.vs", "framebuffer.fs" };
	screenShader.Use();
	screenShader.SetUniform("tex", 1);
	screenShader.DisUse();

	// load image
	int width, height, channel;
	unsigned char* grassData = LoadImg("V:\\Code\\C++\\2D-Game\\assets\\grass.png", &width, &height, &channel);
	unsigned grassTex = 0;
	
	if (grassData)
	{
		glGenTextures(1, &grassTex);
		glBindTexture(GL_TEXTURE_2D, grassTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, grassData);

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	}
	else
	{
		Logger::Log(LogType::FAILED, "GRASS-TEXTURE", "Failed to load grass texture");
	}

	unsigned char* windowData = LoadImg("V:\\Code\\C++\\2D-Game\\assets\\blending_transparent_window.png", &width, &height, &channel);
	unsigned windowTex = 0;
	if (windowData)
	{
		glGenTextures(1, &windowTex);
		glBindTexture(GL_TEXTURE_2D, windowTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, windowData);

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		Logger::Log(LogType::FAILED, "WINDOW-TEXTURE", "Failed to load grass texture");
	}


	// ===================================================================================================================================

	// Make our framebuffer
	unsigned FBO = 0;
	unsigned fboTex = 0;

	{
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);		// Bind to GL_FRAMEBUFFER make us able to write and read to framebuffer

		// Make texture and attach it into framebuffer
		// Make buffer as texture
		glGenTextures(1, &fboTex);
		glBindTexture(GL_TEXTURE_2D, fboTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Attach texture
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTex, 0);

		// Close texture binding
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	unsigned RBO = 0;
	{
		// Generate Object
		glGenRenderbuffers(1, &RBO);
		// Binding
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);

		// Create depth and stencil buffer
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 768);

		// Attach buffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

		// Close renderbuffer binding
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	// Checking frame buffer status
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Logger::Log(LogType::FAILED, "FB-INIT", "Failed to instantiate Framebuffer");
		return 0;
	}

	// Close Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ===================================================================================================================================


	glm::mat4 model = glm::translate(glm::mat4{1.0f}, glm::vec3{0.0f, 0.0f, -10.0f});
	glm::mat4 view = cam.GetView();
	glm::mat4 projection = glm::perspective(glm::radians(cam.GetZoom()), 1024.0f / 768.0f, 0.1f, 100.0f);

	float lastTime = 0.0f;
	float cumulativeTime = 0.0f;

	glfwShowWindow(window);
	// Sorting semi-transparent object
	std::map<float, glm::vec3> sortDistance;

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
		cam.Move();
		// ImGui
		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplGlfw_NewFrame();
		//ImGui::NewFrame();

		////ImGui::SetWindowPos(ImVec2{ 0.0f, 0.0f });
		//ImGui::Begin("Setting");

		//ImGui::Text("Position");
		//ImGui::DragFloat("X", &lightPosition.x, 0.1f, -100.0f, 100.0f);
		//ImGui::DragFloat("Y", &lightPosition.y, 0.1f, -100.0f, 100.0f);
		//ImGui::DragFloat("Z", &lightPosition.z, 0.1f, -100.0f, 100.0f);

		//ImGui::End();


		//ClearScreen();
		// Bind buffer to fbo
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);

		// Clear
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Stencil
		// Draw 1st object
		glStencilFunc(GL_ALWAYS, 1, 0xff);		// Always pass all stencil test
		glStencilMask(0xff);
		glEnable(GL_CULL_FACE);

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

		// Draw 2st object (scaled)
		glStencilFunc(GL_NOTEQUAL, 1, 0xff);	// Only draw stencil that is not 1 (because object before is already fill the buffer with 1 for drawing real object)
		glStencilMask(0x00);					// To set (enable/disable) if the stencil buffer is can be written. 0 -> disable, 0xff (1) -> enable
		glDisable(GL_DEPTH_TEST);				// Performance purpose, because we dont care about depth test when its not using texture

		lightShader.Use();

		lightShader.SetUniform("Color", lightColor);
		lightShader.SetUniform("model", glm::scale(model, glm::vec3{ 1.1f }));
		lightShader.SetUniform("projection", projection);
		lightShader.SetUniform("view", view);

		mods.Draw(lightShader);

		// Enable back
		glStencilMask(0xff);
		glStencilFunc(GL_ALWAYS, 0, 0xff);
		glEnable(GL_DEPTH_TEST);

		// Draw Light
		lightShader.Use();
		lightModel = glm::mat4{ 1.0f };
		
		lightModel = glm::translate(lightModel, lightPosition);

		lightShader.SetUniform("Color", lightColor);
		lightShader.SetUniform("projection", projection);
		lightShader.SetUniform("view", view);
		lightShader.SetUniform("model", glm::scale(lightModel, glm::vec3{ 0.2f }));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		lightShader.SetUniform("Color", glm::vec3{ 0.3f, 0.3f, 0.5f });
		lightShader.SetUniform("model", glm::translate(lightModel, { 0.0f, 5.0f, 2.0f }));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Draw Grass
		glDisable(GL_CULL_FACE);
		grassShader.Use();
		
		grassShader.SetUniform("projection", projection);
		grassShader.SetUniform("view", view);

		glBindVertexArray(grassVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, windowTex);
		// Sorting object
		for (unsigned i = 0; i < ARRAYSIZE(vegetation); i++)
		{
			float distance = glm::length(cam.GetPosition() - vegetation[i]);
			sortDistance.insert(std::make_pair(distance, vegetation[i]));
		}

		for (auto it = sortDistance.rbegin(); it != sortDistance.rend(); it++)
		{
			glm::mat4 grassModel = glm::translate(glm::mat4{ 1.0f }, it->second);
			grassShader.SetUniform("model", grassModel);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		// Clear map
		sortDistance.clear();

		// ImGui Render
		//ImGui::Render();
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (cumulativeTime >= 1.0f)
		{
			cumulativeTime = 0.0f;
			if (unsigned n = Shader::GetFalseUniform())
			{
				Logger::Log(LogType::FAILED, "FALSE-UNIFORM", "", n);
			}
		}

		// Bind back to default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_CULL_FACE);

		// clear
		glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader.Use();
		glBindVertexArray(screenVAO);
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fboTex);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
	}
	// Deleting all gl components
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &grassVBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &grassVAO);
	glDeleteTextures(1, &grassTex);
	glDeleteTextures(1, &windowTex);
	glDeleteFramebuffers(1, &FBO);

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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

	if ((key == GLFW_KEY_W || key == GLFW_KEY_A || key == GLFW_KEY_S || key == GLFW_KEY_D) && action == GLFW_RELEASE)
	{
		cam.KeyInput(CameraMovement::KEYUP, deltaTime);
	}
	else
	{
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
