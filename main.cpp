#include <glad/glad.h>
#include <iostream>

#include "Core/App.h"

#include "OpenGL/Shader.h"
#include "OpenGL/Program.h"
#include "OpenGL/BufferObject.h"
#include "OpenGL/Vao.h"
#include "OpenGL/Texture2D.h"

#include "ImGui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

int main()
{



    App app;
    try
    {
        app.initialize();

        char* buf;
        float f;

        ImGui::Text("Hello, world %d", 123);
        if (ImGui::Button("Save"))
            std::cout<<"lol!\n";
        ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

        // PROGRAM
        
        Shader vertex(GL_VERTEX_SHADER), fragment(GL_FRAGMENT_SHADER);
        vertex.source(app.fileManager.readTextFile("resources/shaders/vertex.shader"));
        fragment.source(app.fileManager.readTextFile("resources/shaders/fragment.shader"));

        Program program(vertex.getId(), fragment.getId());
        program.link();

        // TEXTURE

        Texture2D tex;
        TextureData tdata = app.fileManager.loadTextureData("resources/textures/container.jpg");
        tex.bind();

        tex.setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
        tex.setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
        tex.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        tex.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        tex.data(tdata.data, tdata.width, tdata.height);

        tex.unbind();
        tdata.destroy();

        // TEXTURE 2
        Texture2D tex2;
        TextureData tdata2 = app.fileManager.loadTextureData("resources/textures/awesomeface.png");
        tex2.bind();

        tex2.setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
        tex2.setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
        tex2.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        tex2.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        tex2.data(tdata2.data, tdata2.width, tdata2.height, GL_RGBA);

        tex2.unbind();
        tdata2.destroy();

        // VAO

        Vbo vbo;
        Vao vao;

        vao.bind();
        vbo.bind();

        vbo.data(sizeof(vertices), vertices, GL_STATIC_DRAW);

        vao.queueAttrib(GL_FLOAT, 3);
        vao.queueAttrib(GL_FLOAT, 2);
        vao.setAttribs();

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        vbo.unbind();

        // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
        // ebo.unbind();

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        vao.unbind();


        //  TRANSFORM
        glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 
        glm::mat4 proj = glm::perspective(glm::radians(50.0f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 100.0f);

        // RENDERING LOOP
        program.use();
        program.setUniform1i("texture1", 0);
        program.setUniform1i("texture2", 1);
        program.setUniformMat4f("model", model);
        program.setUniformMat4f("view", view);
        program.setUniformMat4f("projection", proj);

        while (!app.shouldClose())
        {
            app.startFrame();

            model = glm::rotate(model, glm::radians(0.1f), glm::vec3(0.0f,0.0f,1.0f));
            program.setUniformMat4f("model", model);

            Texture2D::activateUnit(0);
            tex.bind();
            Texture2D::activateUnit(1);
            tex2.bind();

            program.use();
            vao.bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);

            app.endFrame();
        }
        app.terminate();    // for some reason won't work if taken out of try-catch
    }
    catch(const std::exception& e)
    {
        app.logger.log(e.what());
        app.terminate();
    }

    return 0;
}