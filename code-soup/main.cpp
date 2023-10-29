#include <glad\glad.h>
#include <glfw\glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings for window size
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// vertex shader (c code to be compiled during runtime)
const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
// fragment shader (c code to be compiled during runtime)
const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";

// the main function that runs when the program starts
int main() {

    // initializes glfw
    glfwInit();
    // sets the glfw version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // sets glfw profile to "core"
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // sets glfw profile to "compatability" if on apple os
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // creates the glfw window with the width, height and title
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "soup", NULL, NULL);
    // if window fails to create;
    if (window == NULL)
    {
        // send an error message
        std::cout << "failed to create glfw window" << std::endl;
        // end the program and glfw
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    // sets up a callback to the "framebuffer_size_callback" function
    // when the window is resized it calls the function
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // loads glad and checks if it loads
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        // if it fails send message and end program
        std::cout << "failed to initialize glad" << std::endl;
        return -1;
    }

    // 
    glViewport(0, 0, WIDTH, HEIGHT);

    // creates a vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // uses the previously defined vertex shader c code
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // and compiles it
    glCompileShader(vertexShader);

    // creates an int to store shader state
    int success;
    // char array to store error information
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "vertex shader failed to compile\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "fragment shader failed to compile\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "shader program failed to link\n" << infoLog << std::endl;
    }
    // deletes the shaders after linking them to opengl
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // the main rendering loop
    // as long as glfw isnt pending to close
    // the loop will run
    while (!glfwWindowShouldClose(window))
    {
        // input section
        processInput(window);

        // rendering section
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // events and buffers section
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // if the rendering loop ends
    // glfw is terminated and the program ends
    glfwTerminate();
    return 0;
}

// this function processes user input
void processInput(GLFWwindow* window)
{
    // if the user presses escape
    // tell glfw to close the window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// this function is called when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // updates the viewport size to the new window size
    glViewport(0, 0, width, height);
}