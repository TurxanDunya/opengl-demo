#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "lmgui/imgui.h"
#include "lmgui/imgui_impl_glfw_gl3.h"

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1920, 1080, "Hello OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error while initializing GLEW" << std::endl;
        return 0;
    }

    {
        float positions[] = {
            -100.0f, -100.0f, 0.0f, 0.0f, // 0
             100.0f, -100.0f, 1.0f, 0.0f, // 1
             100.0f,  100.0f, 1.0f, 1.0f, // 2
            -100.0f,  100.0f, 0.0f, 1.0f, // 3

             200.0f, -100.0f, 0.0f, 0.0f, // 4
             400.0f, -100.0f, 1.0f, 0.0f, // 5
             400.0f,  100.0f, 1.0f, 1.0f, // 6
            -200.0f,  100.0f, 0.0f, 1.0f  // 7
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,

            4, 5, 6,
            6, 7, 4
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexBuffer vb(positions, 8 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);

        VertexArray va;
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 12);

        Shader shader("res/shader/basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.0, 1.0, 0.0, 1.0);

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        Texture texture("res/textures/crusader.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        glm::vec3 translation(200, 0, 0);

        while (!glfwWindowShouldClose(window))
        {
            GLCall(renderer.Clear());

            ImGui_ImplGlfwGL3_NewFrame();

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetuniformMat4f("u_MVP", mvp);

                GLCall(renderer.Draw(va, ib, shader));
            }

            {
                ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);              
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}