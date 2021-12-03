#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void checkCompileErrors(unsigned int shader, std::string type);

// Константы
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// shaders
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 acolor;\n"
"layout (location = 2) in vec3 aTexCoord;\n"

"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"

"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   ourColor = acolor;\n"
"   TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform float time;\n"

"void main()\n"
"{\n"
"   FragColor = vec4(vec3(sin(time), 0.5f, 0.5f ), 1.0f);\n"
"}\n\0";

//add fragment shader for second triangle
const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"

"uniform sampler2D ourTexture;\n"

"void main()\n"
"{\n"
"   FragColor = texture(ourTexture, TexCoord);\n"
"}\n\0";


int main()
{
    // glfw: инициализация и конфигурирование
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw: создание окна
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "hello to OpenGL", NULL, NULL);
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

    // Компилирование нашей шейдерной программы
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader2);
    checkCompileErrors(fragmentShader2, "FRAGMENT2");

    // Связывание шейдеров
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Связывание шейдеров2
    int shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader2);


    //get image
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Устанавливаем параметры наложения и фильтрации текстур (для текущего связанного объекта текстуры)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Загрузка и генерация текстуры
    int width, height, nrChannels;
    unsigned char* data = stbi_load("textures/wood25_512.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data); // освободить память, занимаемую изображением
    // Указывание вершин (и буферов) и настройка вершинных атрибутов
    float triangle1[] = {
        -0.9f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,    0.0f, 0.0f,// слева
        -0.0f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,    1.0f, 0.0f,// справа
        -0.45f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,    0.5f, 1.0f// верхняя вершина   
    };

    // add 2 triangle
    float triangle2[] = {
        // Второй треугольник
         0.0f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,    0.0f, 0.0f,// слева
         0.9f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,    1.0f, 0.0f,// справа
         0.45f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,    0.5f, 1.0f// вверху
    };


    unsigned int VBO1, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

    // Сначала связываем объект вершинного массива, затем связываем и устанавливаем вершинный буфер(ы), и затем конфигурируем вершинный атрибут(ы)
    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    int timeuniform = glGetUniformLocation(shaderProgram, "time");

    
 
    //add atributes for second triangle
    unsigned int VBO2, VAO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);

    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Обратите внимание, что данное действие разрешено, вызов glVertexAttribPointer() зарегистрировал VBO как привязанный вершинный буферный объект для вершинного атрибута, так что после этого мы можем спокойно выполнить отвязку
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Вы можете отменить привязку VAO после этого, чтобы другие вызовы VAO случайно не изменили этот VAO (но подобное довольно редко случается).
    // Модификация других VAO требует вызова glBindVertexArray(), поэтому мы обычно не снимаем привязку VAO (или VBO), когда это не требуется напрямую
    glBindVertexArray(0);

    // Раскомментируйте следующую строку для отрисовки полигонов в режиме каркаса
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Цикл рендеринга
    while (!glfwWindowShouldClose(window))
    {
        // Обработка ввода
        processInput(window);

        // Рендеринг
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // textures
        glBindTexture(GL_TEXTURE_2D, texture);
        // Рисуем наш первый треугольник
        glUseProgram(shaderProgram);

        float timeValue = glfwGetTime();
        glUniform1f(timeuniform, timeValue);


        glBindVertexArray(VAO1); // поскольку у нас есть только один VAO, то нет необходимости связывать его каждый раз (но мы сделаем это, чтобы всё было немного организованнее)
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // не нужно каждый раз его отвязывать
        // add and draw second triangle
        glUseProgram(shaderProgram2);

        glBindVertexArray(VAO2); // поскольку у нас есть только один VAO, то нет необходимости связывать его каждый раз (но мы сделаем это, чтобы всё было немного организованнее)
        glDrawArrays(GL_TRIANGLES, 0, 3);


        // glfw: обмен содержимым front- и back-буферов. Отслеживание событий ввода/вывода (была ли нажата/отпущена кнопка, перемещен курсор мыши и т.п.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Опционально: освобождаем все ресурсы, как только они выполнили свое предназначение
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);

    // glfw: завершение, освобождение всех ранее задействованных GLFW-ресурсов
    glfwTerminate();
    return 0;
}

// Обработка всех событий ввода: запрос GLFW о нажатии/отпускании кнопки мыши в данном кадре и соответствующая обработка данных событий
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: всякий раз, когда изменяются размеры окна (пользователем или операционной системой), вызывается данная callback-функция
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Убеждаемся, что окно просмотра соответствует новым размерам окна.
    // Обратите внимание, высота окна на Retina-дисплеях будет значительно больше, чем указано в программе
    glViewport(0, 0, width, height);
}

// проверка на ошибки
void checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}