#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <chrono>

#include <shaderLoader.h>

using std::cout, std::endl;
using std::chrono::high_resolution_clock, std::chrono::duration_cast;
using std::chrono::milliseconds;

static const int width = 1024;
static const int height = width / 4 * 3;

/**
 * Initialize glfw and glew and make a glfw window.
 * Returns nullptr if initialization fails.
 */
GLFWwindow* createWindow()
{
    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return NULL;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int width = 1024;
    int height = width / 4 * 3;  // 4/3 aspect ratio

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Playground", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }
    glViewport(0, 0, width, height);

    return window;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

std::vector<glm::vec3> getStapleVertices(std::vector<float>& values)
{
    float width = 2.0f / values.size();
    float space = width * 0.1;

    std::vector<glm::vec3> res{};
    for (int i = 0; i < values.size(); i++) {
        float y = -0.9 + values[i] * 1.8;
        float x1 = -1 + width * i;
        float x2 = -1 + width * (i + 1) - space;

        glm::vec3 arr[] = {
            {x1, -1, 0},  // BL
            {x1, y, 0},  // TL
            {x2, -1, 0},  // BR
            {x2, -1, 0},  // BR
            {x1, y, 0},  // TL
            {x2, y, 0}  // TR
        };
        res.insert(res.end(), std::begin(arr), std::end(arr));
    }
    return res;
}

std::vector<glm::vec3> genColors(int size, int index)
{
    static const glm::vec3 greenTriangles[] = {
        {0.2f, 0.8f, 0.2f},
        {0.2f, 0.8f, 0.2f},
        {0.2f, 0.8f, 0.2f},
        {0.2f, 0.8f, 0.2f},
        {0.2f, 0.8f, 0.2f},
        {0.2f, 0.8f, 0.2f}
    };

    static const glm::vec3 whiteTriangles[] = {
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f}
    };


    std::vector<glm::vec3> res{};
    for (int i = 0; i < size; i++) {
        if (i == index || i == index + 1) {
            res.insert(res.end(), std::begin(greenTriangles), std::end(greenTriangles));
        }
        else {
            res.insert(res.end(), std::begin(whiteTriangles), std::end(whiteTriangles));
        }
    }
    return res;
}

// Returns -1 if sorting is done
int bubbelOneIt(std::vector<float>& values)
{
    static int index = 0;
    static bool changed = false;

    // Change places of value[index] and value[index + 1]
    if (values[index] > values[index + 1]) {
        changed = true;
        std::swap(values[index], values[index + 1]);
    }

    index++;
    if (index == values.size() - 1) {
        if (!changed) return -1;
        index = 0;
    }
    return index;
}

bool sortIt(std::vector<float>& values, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& colors)
{
    static bool v = true;
    static int index;

    if (v) {
        index = bubbelOneIt(values);
        if (index == -1) return false;
        vertices = getStapleVertices(values);
    }
    else {
        colors = genColors(values.size(), index);
    }

    v = !v;
    return true;
}

int main(void)
{
    GLFWwindow* window = createWindow();

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(0.5f, 0.7f, 0.8f, 0.0f);

    // Create VAO
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // 0.1, 0.2, 0.4, 0.6, 0.7
    std::vector<float> values{ 1, 0, 0.2, 0.6, 0.4, 0.7, 0.15, 0.72, 0.55 };
    auto vertices = getStapleVertices(values);

    GLuint vertexBufferID;
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_DYNAMIC_DRAW);

    auto colors = genColors(values.size(), 0);

    GLuint colorBufferID;
    glGenBuffers(1, &colorBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(colors[0]), &colors[0], GL_DYNAMIC_DRAW);

    Shader shader = Shader("vertexShader.glsl", "fragmentShader.glsl");

    // cout << "asd " << vertices.size() / values.size() << endl;
    // cout << "Vertices size" << vertices.size() << endl;
    // cout << "Colors size" << colors.size() << endl;

    auto lastTime = high_resolution_clock::now();

    while (!glfwWindowShouldClose(window)) {
        if (duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - lastTime).count() >= 100) {
            lastTime = high_resolution_clock::now();
            if (sortIt(values, vertices, colors)) {
                glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
                glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_DYNAMIC_DRAW);
                glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
                glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(colors[0]), &colors[0], GL_DYNAMIC_DRAW);
            }
        }

        // TODO: use element buffer (indices of vertex to avoid duplicate)
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader.ID);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
        processInput(window);
    }

    glfwTerminate();
    return 0;
}
