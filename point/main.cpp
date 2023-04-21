#include "include/glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"

#include <iostream>
#include <stdlib.h>
#include <utility>
#include <vector>
#include <cstdlib> // для функций rand() и srand()
#include <ctime>
#include <chrono>
#include <random>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// Константы
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

const int amount = 40;



/*std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(-0.8,0.8);
auto dice = std::bind ( distribution, generator );

std::default_random_engine generator2;
std::uniform_real_distribution<float> distribution2(-0.014,0.014);
auto dice2 = std::bind ( distribution2, generator2 );*/


/*glm::vec3 create_vector() {
    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator1 (seed1);
    std::uniform_real_distribution<float> distribution1(-0.8,0.8);

    unsigned seed2 = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator2 (seed2);
    std::uniform_real_distribution<float> distribution2(-0.8,0.8);

    //glm::vec3 vec(dice(), dice(), 0);
    glm::vec3 vec(distribution1(generator1), distribution2(generator2), 0);
    return vec; 
}

glm::vec3 create_speed() {
    //glm::vec3 vec(dice2(), dice2(), 0);
    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator1 (seed1);
    std::uniform_real_distribution<float> distribution1(-0.015,0.015);

    unsigned seed2 = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator2 (seed2);
    std::uniform_real_distribution<float> distribution2(-0.015,0.015);

    //glm::vec3 vec(dice(), dice(), 0);
    glm::vec3 vec(distribution1(generator1), distribution2(generator2), 0);
    return vec;
}
*/
//glm::vec3 vec(static_cast <float> (rand()) /( static_cast <float> (RAND_MAX)),
    //static_cast <float> (rand()) /( static_cast <float> (RAND_MAX)),
    //0);
int main()
{
    srand(time(NULL));

    // glfw: инициализация и конфигурирование
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // раскомментируйте эту строку, если используете macOS
#endif

    

    // glfw создание окна
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Marat", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: загрузка всех указателей на OpenGL-функции
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    Shader ourShader("shader.vs", "shader.fs");  // путь к файлам шейдеров
    

    /*glm::vec3 vectors[] = {//pair для цветов
         // координаты
         glm::vec3(0.0f, 0.0f, 0.0f),  // нижняя правая точка
         glm::vec3(-0.5f, -0.5f, 0.0f),  // нижняя левая точка
         glm::vec3(0.0f,  0.5f, 0.0f)   // верхняя точка
    };*/

    
    glm::vec3 vectors[amount];
    for(int i = 0; i < amount; i++) {
    	float x = (rand()%9 + 1);
        vectors[i].x = x/10;
        float y = (rand()%9 + 1);
        vectors[i].y = y/10;
        vectors[i].z = 0.0f;
    }
    std::cout  << "; x = " << vectors[0].x << "; y = " << vectors[0].y << std::endl;

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // Сначала связываем объект вершинного массива, затем связываем и устанавливаем вершинный буфер(ы), и затем конфигурируем вершинный атрибут(ы)
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vectors), vectors, GL_STATIC_DRAW);

    // Координатные артибуты
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, amount * sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Цветовые атрибуты

    /*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(1);*/

    // После этого вы можете отменить привязку VАО, чтобы другие вызовы VАО случайно не изменили этот VAO, но это редко происходит.
    // Изменение других значений VAO требует вызова функции glBindVertexArray() в любом случае, поэтому мы обычно не снимаем привязку VAO (или VBO), когда это непосредственно не требуется
    // glBindVertexArray(0);

    std::vector <float> directionX;
    for(int i = 0; i < amount; i++) {
        if(i % 3 == 1){
            directionX.push_back(1.0f);
            continue;
        }
        else directionX.push_back(-1.0f);;
    }

    std::vector <float> directionY;
    for(int i = 0; i < amount; i++) {
        if(i % 5 == 1 || i % 5 == 3){
            directionY.push_back(1.0f);
            continue;
        }
        else directionY.push_back(-1.0f);;
    }

    std::vector <glm::vec3> spd;
    for(int i = 0; i < amount; i++) {
        spd.push_back( glm::vec3((float) (rand()%9 + 1)/100 , (float)(rand()%9 + 1)/100, 0.0f));
    }

    //std::cout << "speed =  " <<spd[0].x << "___" << spd[0].y << std::endl;

    // Цикл рендеринга
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3, 1.0f);
        glClear( GL_COLOR_BUFFER_BIT );
        ourShader.use();


        for(int i = 0; i < amount; i++) {
            glm::mat4 view = glm::mat4(1.0f);
            view  = glm::translate(view, vectors[i]);
            unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
            glPointSize(50);
            glBindVertexArray(VAO);
            glDrawArrays(GL_POINTS, i + 1, i + 1);
        }

        for(int i = 0; i < amount; i++) {
            vectors[i].x += spd[i].x;
            vectors[i].y += spd[i].y;

            if ((vectors[i].x <= -0.95f ) || (vectors[i].x >= 0.95f)) {
                directionX[i] *= -1.0;
                spd[i].x *= directionX[i];
            }

            if ((vectors[i].y <= -0.95f) || (vectors[i].y >= 0.95f)) {
                directionY[i] *= -1.0;
                spd[i].y *= directionY[i];
            }
        }

        for(int i = 0; i < amount; i++ ){
            std::cout << i << "; x = " << vectors[i].x << "; y = " << vectors[i].y << std::endl;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // освобождение всех ранее задействованных GLFW-ресурсов
    glfwTerminate();
    return 0;
}

// Обработка всех событий ввода: запрос GLFW о нажатии/отпускании клавиш на клавиатуре в данном кадре и соответствующая обработка данных событий
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: всякий раз, когда изменяются размеры окна (пользователем или оперионной системой), вызывается данная callback-функция
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Убеждаемся, что окно просмотра соответствует новым размерам окна.
    // Обратите внимание, ширина и высота будут значительно больше, чем указано, на Retina-дисплеях
    glViewport(0, 0, width, height);
}