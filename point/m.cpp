#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <ctime>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_button_callback(GLFWwindow* window, std::vector<glm::vec3> &vec,  std::vector<float> &vX, std::vector<float> &vY);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

   
    glewExperimental = true;

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Невозможно инициализировать GLEW\n");
        return -1;
    }

    Shader ourShader("part_sh.vs", "part_sh.fs");

    float vertices[] = {
         0.0f,   0.0f,    0.0f,
         0.0f,  -0.05f,   0.0f,
        -0.05f, -0.05f,   0.0f, 
        -0.05f, -0.05f,   0.0f,
        -0.05f,  0.0f,    0.0f,
         0.0f,   0.0f,    0.0f
    };

    std::vector<glm::vec3> particles_locations = {
        glm::vec3(0.5f, 0.5f, 0.0f),
        glm::vec3(0.0f, 0.5f, 0.0f),
        glm::vec3(-0.5f,-0.5f, 0.0f)
    };

    // float dist = glm::distance(glm::vec3(2.0f, 4.0f, 5.0f), glm::vec3(1.0f, 3.0f, 4.0f));
    // glm::vec3 norm_v = glm::vec3(1,1,1);
    // int value = 3;
    // norm_v = glm::vec3(norm_v.x * value, norm_v.y * value, norm_v.z * value);
    // norm_v += -1;
    // std::cout << (norm_v * 3.0f).x << "\n";

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO);

    srand(time(NULL));

    std::vector<float> velocityY;
    std::vector<float> velocityX;

    for (int i = 0; i < 3; i++) {
        float xr = (rand()%2 + 1);
        float yr = (rand()%2 + 1);
        velocityY.push_back(yr/100);
        velocityX.push_back(xr/100);

    }

    // std::vector<float> velocityX = {  , 0.007f, 0.02f};
    // std::vector<float> velocityY = { 0.01f, 0.04f, -0.07f};
    int flag = 0;
    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        if (flag%3 == 0)
            mouse_button_callback(window, particles_locations, velocityX, velocityY);
        flag ++;
        // for (size_t i = 0; i < sizeof(particles_locations)/sizeof(glm::vec3) +1; i++)
        // {
        //     std::cout << "[" << i << "]" << particles_locations[i].x  << "\n";
        // }
        

        glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();

        // std::cout << particles_locations.size() << "\n";

        for (int i = 0; i < particles_locations.size(); i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, particles_locations[i]);
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        // glm::vec3 dm = {0.1, 0.1, 0.1};
        
        bool check = false;
        for (unsigned int j = 0; j < particles_locations.size(); j++) {
            particles_locations[j].x += velocityX[j];
            particles_locations[j].y += velocityY[j];
            particles_locations[j].z += 0; 

            if (particles_locations[j].x >= 0.985f || particles_locations[j].x <= -0.90f) {
                velocityX[j] *= -1;
            }

            if (particles_locations[j].y >= 0.985f || particles_locations[j].y <= -0.90f) {
                velocityY[j] *= -1;
            }

        }

        // std::cout << particles_locations.size() << "\n";
        for (int k = 0; k < particles_locations.size(); k++) {
            for (int l = k+1; l < particles_locations.size(); l++) {
                if (abs(particles_locations[l].x - particles_locations[k].x) < 0.05f  
                && abs(particles_locations[l].y - particles_locations[k].y) < 0.05f) {
                    velocityX[l] *= -1;
                    velocityX[k] *= -1;
                    velocityY[l] *= -1;
                    velocityY[k] *= -1;
                    // std::cout << "FLAG k = " << k << " l = " << l << "\n";
                }
                // std::cout << "k = " << k << " l = " << l << "\n";
                //     std::cout << " x = " <<  abs(particles_locations[l].x - particles_locations[k].x) << " y =" << abs(particles_locations[l].y - particles_locations[k].y) << "\n";
                
            }
            
        }

        // std::cout << "END\n";

        

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void mouse_button_callback(GLFWwindow* window, std::vector<glm::vec3> &vec, std::vector<float> &vX, std::vector<float> &vY) {
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) 
    {
        double xpos, ypos;
       //getting cursor position
        glfwGetCursorPos(window, &xpos, &ypos);

        glm::vec3 v ((xpos-400)/400, (ypos-300)/300 * -1, 0.0);
        vec.push_back(v);

        // std::cout << vec[vec.size()-1].x << " " << vec[vec.size()-1].y << " " << vec[vec.size()-1].z << "\n";

        float xr = (rand()%3 + 1);
        float yr = (rand()%3 + 1);
        vY.push_back(yr/100);
        vX.push_back(xr/100);

        // std::cout << "Cursor Position at (" << (xpos-400)/400 << " : " << (ypos-300)/300 * -1 << ")" << std::endl;
    }
}