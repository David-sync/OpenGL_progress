#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

// thêm thư viện matrix
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "loadingBMP.h"
#include "controls/controls.hpp"

int main() {

    // Khởi tạo GLFW
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4); // 4 sample / 1 pixel
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // đổi từ 3 thành 4
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // đổi từ 3 thành 6
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Tạo cửa sổ
    GLFWwindow* window = glfwCreateWindow(800, 600, "Gay cube here we come aaaaaaaaaa", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Khởi tạo GLAD để load các hàm OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // khởi tạo VAO
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // mảng 3 vector đỉnh
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };

    static const GLfloat g_uv_buffer_data[] = {
        // Tam giác 1-6 (18 đỉnh): Co rúm về 1 pixel (0,0)
            0.0f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, // T1
            0.0f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, // T2
            0.0f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, // T3
            0.0f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, // T4
            0.0f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, // T5
            0.0f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, // T6

            // ==================================================
            // TAM GIÁC 7 (Mặt +Z phần 1): Trải nửa tấm ảnh
            // ==================================================
            0.0f, 1.0f,  // Đỉnh Top-Left (-1, 1, 1)
            0.0f, 0.0f,  // Đỉnh Bottom-Left (-1,-1, 1)
            1.0f, 0.0f,  // Đỉnh Bottom-Right (1,-1, 1)

            // Tam giác 8-11 (12 đỉnh): Co rúm về 1 pixel (0,0)
            0.0f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, // T8
            0.0f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, // T9
            0.0f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, // T10
            0.0f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, // T11

            // ==================================================
            // TAM GIÁC 12 (Mặt +Z phần 2): Trải nửa tấm ảnh còn lại
            // ==================================================
            1.0f, 1.0f,  // Đỉnh Top-Right (1, 1, 1)
            0.0f, 1.0f,  // Đỉnh Top-Left (-1, 1, 1)
            1.0f, 0.0f   // Đỉnh Bottom-Right (1,-1, 1)
    };

    static const GLfloat g_triangle_buffer_data[] = {
        -2.0f, -2.0f, 2.0f, 1.0f, 0.0f, 0.0f,
        2.0f, -2.0f, 2.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 2.0f, 2.0f, 0.0f, 0.0f, 1.0f
    };

    GLuint Texture = loadDDS("dio_face.DDS");

    GLuint triangle_vertexbuffer;
    glGenBuffers(1, &triangle_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, triangle_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_triangle_buffer_data), g_triangle_buffer_data, GL_STATIC_DRAW);

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);




    GLuint programID = LoadShaders("vertex.themalaihay", "fragment.themalaihay");
    if (programID == 0)
    {
        std::cout << "Shader program failed!\n";
        return -1;
    }




    // trả về 1 số nguyên, MVP kiểu giống như hỏi hòm thư đang ở đâu và được trả về hòm thư số mấy 
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");


    // ẩn con trỏ chuột và khóa vào trong cửa sổ game
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(window, scroll_callback);
    double lastTime = glfwGetTime();
    do {
        glEnable(GL_CULL_FACE);
    
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        // Xóa màn hình
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        // giải thích cho gpu nên đọc dữ liệu trong kho vertexbuffer như thế nào
        // glVertexAttribPointer(index, size, type, normalized, stride, pointer)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        
        glUseProgram(programID);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glUniform1i(glGetUniformLocation(programID, "myTextureSample"), 0);


        // move
        computeMatricesFromInputs(window, deltaTime);
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // đưa mvp vào hòm thư số MatrixID hồi nãy
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        glDrawArrays(GL_TRIANGLES, 0, 36);


        // Hiện tại bên vertex đang chỉ nhận vec2 là vertexUV thôi nên tam giác bị đọc sai
        //glEnableVertexAttribArray(0);
        //glBindBuffer(GL_ARRAY_BUFFER, triangle_vertexbuffer);
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // stride là bước nhảy 6 tại data trộn lẫn với màu rồi, 1 lần nó đọc 3 block thì offset 0 + 6

        //glEnableVertexAttribArray(1);
        //glBindBuffer(GL_ARRAY_BUFFER, triangle_vertexbuffer);
        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        glDisableVertexAttribArray(0);
        


        // Tráo đổi bộ đệm và nhận sự kiện chuột/phím
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Kiểm tra xem nút ESC có được bấm không, hoặc cửa sổ bị bấm X không
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);

    // Dọn dẹp bộ nhớ trước khi thoát
    glfwTerminate();
    return 0;
}