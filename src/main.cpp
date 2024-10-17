#include <glad/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/ext.hpp>

#include <iostream>
#include <vector>

int main()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "Error initlaizing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("OpenGL Template",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          1280,
                                          720,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (window == nullptr)
    {
        std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext)
    {
        std::cerr << "Error creating OpenGL context: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_GL_MakeCurrent(window, glContext);

    int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    if(version == 0)
    {
        std::cerr << "Failed to load OpenGL: " << glGetError() << std::endl;
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    std::vector<GLfloat> vertices = {
        // positions          // normals
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

    const char *vertexShaderSource = R"(
    #version 460 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;

    out vec3 FragPos;
    out vec3 Normal;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        FragPos = vec3(model * vec4(aPos, 1.0));
        Normal = mat3(transpose(inverse(model))) * aNormal;  
        gl_Position = projection * view * vec4(FragPos, 1.0);
    }
    )";

    const char *fragmentShaderSource = R"(
    #version 460 core
    out vec4 FragColor;

    in vec3 Normal;  
    in vec3 FragPos;  
  
    uniform vec3 lightPos; 
    uniform vec3 viewPos; 
    uniform vec3 lightColor;
    uniform vec3 objectColor;

    void main()
    {
        // ambient
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lightColor;
  	
        // diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
    
        // specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor;  
        
        vec3 result = (ambient + diffuse + specular) * objectColor;
        FragColor = vec4(result, 1.0);
    }
    )";

    std::vector<GLfloat> gridVertices = {
        -50.0f, 0.0f, -50.0f,
        50.0f, 0.0f, -50.0f,
        50.0f, 0.0f, 50.0f,
        -50.0f, 0.0f, 50.0f};

    const char *gridVertexShaderSource = R"(
    #version 460 core

    layout (location = 0) in vec3 aPos;

    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;

    out vec3 worldPos;

    void main() {
        vec4 worldPosition = model * vec4(aPos, 1.0);
        worldPos = worldPosition.xyz;
        gl_Position = projection * view * worldPosition;
    }
    )";

    const char *gridFragmentShaderSource = R"(
    #version 460 core

    in vec3 worldPos;

    out vec4 FragColor;

    uniform vec3 gridColor = vec3(0.8, 0.8, 0.8);
    uniform float gridSize = 1.0; // 1 meter grid size
    uniform float lineWidth = 0.02; // Width of the grid lines

    void main() {
        vec2 coord = worldPos.xz; // Use xz plane for the grid
        vec2 grid = abs(fract(coord / gridSize - 0.5) - 0.5) / fwidth(coord);
        float line = min(grid.x, grid.y);

        float alpha = 1.0 - min(line, 1.0);

        // Make the lines more pronounced
        alpha = smoothstep(0.0, lineWidth, alpha);

        FragColor = vec4(gridColor, alpha);
    }
    )";

    GLuint cubeVao, cubeVbo;
    glGenVertexArrays(1, &cubeVao);
    glGenBuffers(1, &cubeVbo);

    glBindVertexArray(cubeVao);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint vertexShader, fragmentShader, shaderProgram;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint gridVao, gridVbo;
    glGenVertexArrays(1, &gridVao);
    glGenBuffers(1, &gridVbo);

    glBindVertexArray(gridVao);
    glBindBuffer(GL_ARRAY_BUFFER, gridVbo);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(GLfloat), gridVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);

    // Compile and link shaders for the grid
    GLuint gridVertexShader, gridFragmentShader, gridShaderProgram;
    gridVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(gridVertexShader, 1, &gridVertexShaderSource, NULL);
    glCompileShader(gridVertexShader);

    gridFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(gridFragmentShader, 1, &gridFragmentShaderSource, NULL);
    glCompileShader(gridFragmentShader);

    gridShaderProgram = glCreateProgram();
    glAttachShader(gridShaderProgram, gridVertexShader);
    glAttachShader(gridShaderProgram, gridFragmentShader);
    glLinkProgram(gridShaderProgram);

    glDeleteShader(gridVertexShader);
    glDeleteShader(gridFragmentShader);

    glUseProgram(0);

    glViewport(0, 0, 1280, 720);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.2f, 0.4f, 0.8f, 1.0f);

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), 1280.0f / 720.0f, 0.001f, 1000.0f);
    glm::mat4 cubeModel = glm::mat4(1.0f);
    glm::mat4 gridModel = glm::mat4(1.0f);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    float radius = 5.0f;
    float camX, camZ, camY;

    bool running = true;
    float dt = 0.0f, angle = 0.0f;
    Uint32 NOW = SDL_GetPerformanceCounter(), LAST = 0;

    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
            }
        }

        NOW = SDL_GetPerformanceCounter();
        dt = (float)(NOW - LAST) / SDL_GetPerformanceFrequency();
        LAST = NOW;

        const Uint8 *keystate = SDL_GetKeyboardState(NULL);

        if (keystate[SDL_SCANCODE_A])
        {
            angle -= 0.5f * dt;
        }
        if (keystate[SDL_SCANCODE_D])
        {
            angle += 0.5f * dt;
        }
        if(keystate[SDL_SCANCODE_W]) 
        {
            camY += 0.5f * dt;
        }
        if(keystate[SDL_SCANCODE_S]) 
        {
            camY -= 0.5f * dt;
        }

        if (keystate[SDL_SCANCODE_LEFT])
        {
            cubeModel = glm::rotate(cubeModel, glm::radians(10.0f) * dt, glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (keystate[SDL_SCANCODE_RIGHT])
        {
            cubeModel = glm::rotate(cubeModel, glm::radians(10.0f) * dt, glm::vec3(0.0f, -1.0f, 0.0f));
        }
        if(keystate[SDL_SCANCODE_UP]) 
        {
            cubeModel = glm::rotate(cubeModel, glm::radians(10.0f) * dt, glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if(keystate[SDL_SCANCODE_DOWN]) 
        {
            cubeModel = glm::rotate(cubeModel, glm::radians(10.0f) * dt, glm::vec3(-1.0f, 0.0f, 0.0f));
        }

        camX = sin(angle) * radius;
        camZ = cos(angle) * radius;

        glm::mat4 view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (shaderProgram != -1)
        {
            glUseProgram(shaderProgram);

            GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
            GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
            GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");

            if (projectionLoc != -1)
                glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

            if (viewLoc != -1)
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

            if (modelLoc != -1)
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cubeModel));

            GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
            GLuint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
            GLuint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
            GLuint objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");

            glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
            glUniform3f(viewPosLoc, camX, 1.0f, camZ);
            glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
            glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
        }

        glBindVertexArray(cubeVao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glUseProgram(gridShaderProgram);

        GLuint projectionLoc = glGetUniformLocation(gridShaderProgram, "projection");
        GLuint viewLoc = glGetUniformLocation(gridShaderProgram, "view");
        GLuint modelLoc = glGetUniformLocation(gridShaderProgram, "model");
        GLuint gridColorLoc = glGetUniformLocation(gridShaderProgram, "gridColor");
        GLuint gridSizeLoc = glGetUniformLocation(gridShaderProgram, "gridSize");
        GLuint lineWidthLoc = glGetUniformLocation(gridShaderProgram, "lineWidth");

        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m)); // Identity matrix for the grid
        glUniform3f(gridColorLoc, 0.8f, 0.8f, 0.8f);
        glUniform1f(gridSizeLoc, 1.0f);
        glUniform1f(lineWidthLoc, 0.02f);

        glBindVertexArray(gridVao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        SDL_GL_SwapWindow(window);
    }

    glDeleteVertexArrays(1, &cubeVao);
    glDeleteBuffers(1, &cubeVbo);
    glDeleteProgram(shaderProgram);

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}