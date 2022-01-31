#include <iostream>
#include <fstream>
#include <string>

#include <glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <SOIL2.h>
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, GLuint program);
bool loadShaders(GLuint& program);

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGH = 480;

// Vertices coordinates
GLfloat vertices[] = 
{
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

GLuint indices[] =
{
    0, 1, 3, // Lower left triangle
    1, 2, 3, // Lower right triangle

};

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGH, "Hello openGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glew init
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "glew error"<<"\n";
        glfwTerminate();
        return -1;
    }

    //Shader init
    GLuint core_program;
    if (!loadShaders(core_program))
    {
        glfwTerminate();
        return -1;
    }


    GLuint VAO, VBO, EBO;

    // Generate the VAO and VBO with only 1 object each
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Make the VAO the current Vertex Array Object by binding it
    glBindVertexArray(VAO);

    // Bind the VBO specifying it's a GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Introduce the vertices into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind the EBO specifying it's a GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Introduce the indices into the EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configure the Vertex Attribute so that OpenGL knows how to read the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // Enable the Vertex Attribute so that OpenGL knows to use it
    glEnableVertexAttribArray(0);
    /* Configure the Color Attribute

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // Enable the Vertex Attribute so that OpenGL knows to use it
    glEnableVertexAttribArray(1);
    */
    // uv attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // Enable the Vertex Attribute so that OpenGL knows to use it
    glEnableVertexAttribArray(2);


    // Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // gen and binde textures
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); 

    // mapping projection
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // filter textufes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //flip image before loading
    stbi_set_flip_vertically_on_load(true);
    // load image. create texture . generate mippam
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
    stbi_image_free(data);

    //camera prozection
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    //camera perspective
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(65.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    //backface fix
    glEnable(GL_DEPTH_TEST);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glClearColor(0.1f, 0.3f, 0.4f, 1.0f);

        //binde textures
        glBindTexture(GL_TEXTURE_2D, texture);

        //matrix transform
        glm::mat4 transform = glm::mat4(1.0f); // entity matrix
        //transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime()*2, glm::vec3(0.0f, 1.0f, 1.0f));


        // use program
        glUseProgram(core_program);

        //update uniform
        int viewLoc = glGetUniformLocation(core_program, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projectLoc = glGetUniformLocation(core_program, "projection");
        glUniformMatrix4fv(projectLoc, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int transLoc = glGetUniformLocation(core_program, "transform");
        glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(transform));

        //binde and draw mesh
        glBindVertexArray(VAO);
        // Draw primitives, number of indices, datatype of indices, index of indices
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        processInput(window, core_program);//escape and arrow
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwDestroyWindow(window);
    glfwTerminate();
    glDeleteProgram(core_program);
    return 0;
}


void processInput(GLFWwindow* window, GLuint program)
{
    int vertexColorLocation2 = glGetUniformLocation(program, "yOffset");
    int vertexColorLocation = glGetUniformLocation(program, "xOffset");

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        glClearColor(0.1f, 0.7f, 0.3f, 1.0f);
        //glUniform1f(vertexColorLocation2, -0.5f);

    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        glClearColor(0.7f, 0.2f, 0.8f, 1.0f);
        //glUniform1f(vertexColorLocation2, 0.5f);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        glClearColor(0.0f, 0.2f, 0.3f, 1.0f);
        //glUniform1f(vertexColorLocation, -0.5f);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        glClearColor(1.f, 0.0f, 0.0f, 1.0f);
        //glUniform1f(vertexColorLocation, 0.5f);
    }
}

// glfw: change windows size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool loadShaders(GLuint &program)
{
    bool loadSuccess = true;

    GLint success;
    char infoLog[512];

    std::string temp = "";
    std::string src  = "";

    std::ifstream in_file;

    //vertex
    in_file.open("VertexShader.glsl");

    if (in_file.is_open())
    {
        while (std::getline(in_file, temp))
            src += temp + "\n";
    }
    else
    {
        std::cout << "error to load vertex shader" << "\n";
        loadSuccess = false;
    }

    in_file.close();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vertSrc = src.c_str();
    glShaderSource(vertexShader, 1, &vertSrc, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "error to compile vertex shader" << "\n";
        std::cout << infoLog << "\n";
        loadSuccess = false;

    }

    temp = "";
    src  = "";

    //fragment
    in_file.open("FragmentShader.glsl");

    if (in_file.is_open())
    {
        while (std::getline(in_file, temp))
            src += temp + "\n";
    }
    else
    {
        std::cout << "error to load fragment shader" << "\n";
        loadSuccess = false;
    }

    in_file.close();

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fragSrc = src.c_str();
    glShaderSource(fragmentShader, 1, &fragSrc, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "error to compile fragment shader" << "\n";
        std::cout << infoLog << "\n";
        loadSuccess = false;

    }
    //program
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "error to link shader program" << "\n";
        std::cout << infoLog << "\n";
        loadSuccess = false;

    }


    //end
    glUseProgram(0);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return loadSuccess;

}


