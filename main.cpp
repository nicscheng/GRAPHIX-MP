#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // Window width and height
    float width = 600;
    float height = 600;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "NICOLE CHENG", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    //SPOON
    std::string path = "3D/plane.obj";
    
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning, error;
    tinyobj::attrib_t attributes;
    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &materials,
        &warning,
        &error,
        path.c_str()
    );

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    // texture 1
    int img_width0, img_height0, color_channels0;
    unsigned char* tex_bytes0 = stbi_load(
        "3D/brickwall.jpg",
        &img_width0,
        &img_height0,
        &color_channels0,
        0
    );

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        img_width0,
        img_height0,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        tex_bytes0
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes0);


    // normal
    unsigned char* norm_bytes0 = stbi_load(
        "3D/brickwall_normal.jpg",
        &img_width0,
        &img_height0,
        &color_channels0,
        0
    );

    GLuint norm_tex;
    glGenTextures(1, &norm_tex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, norm_tex);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        img_width0,
        img_height0,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        norm_bytes0
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(norm_bytes0);

    // texture 2
    unsigned char* tex_bytes2 = stbi_load(
        "3D/yae.png",
        &img_width0,
        &img_height0,
        &color_channels0,
        0
    );

    GLuint texture2;
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        img_width0,
        img_height0,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        tex_bytes2
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes2);

    //glEnable(GL_DEPTH_TEST);

    // Vertex and Fragment Shader
    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertString = vertBuff.str();
    const char* v = vertString.c_str();

    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragString = fragBuff.str();
    const char* f = fragString.c_str();

   

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &f, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // second
    std::fstream vertSrc1("Shaders/skybox.vert");
    std::stringstream vertBuff1;
    vertBuff1 << vertSrc1.rdbuf();
    std::string vertString1 = vertBuff1.str();
    const char* v1 = vertString1.c_str();

    std::fstream fragSrc1("Shaders/skybox.frag");
    std::stringstream fragBuff1;
    fragBuff1 << fragSrc1.rdbuf();
    std::string fragString1 = fragBuff1.str();
    const char* f1 = fragString1.c_str();

    // second

    GLuint vertexShader1 = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader1, 1, &v1, NULL);
    glCompileShader(vertexShader1);

    GLuint fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader1, 1, &f1, NULL);
    glCompileShader(fragmentShader1);

    GLuint shaderProgram1 = glCreateProgram();
    glAttachShader(shaderProgram1, vertexShader1);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);

    /*
  7--------6
 /|       /|
4--------5 |
| |      | |
| 3------|-2
|/       |/
0--------1
*/
//Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
    };

    //Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    std::string facesSkybox[]{
        "Skybox/rainbow_rt.png",
        "Skybox/rainbow_lf.png",
        "Skybox/rainbow_up.png",
        "Skybox/rainbow_dn.png",
        "Skybox/rainbow_ft.png",
        "Skybox/rainbow_bk.png"

    };

    unsigned int skyboxTex;

    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    //prevents pixelating
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //prevents tiling
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++)
    {
        int w, h, skyCChannel;
        stbi_set_flip_vertically_on_load(false);
        unsigned char* data = stbi_load(facesSkybox[i].c_str(), &w, &h, &skyCChannel, 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            stbi_image_free(data);
        }
        
    }
    stbi_set_flip_vertically_on_load(true);
    //stbi_set_flip_vertically_on_load(true);
    //spoon texture
    
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    for (int i = 0; i < shapes[0].mesh.indices.size(); i += 3) {
        //V1
        tinyobj::index_t vData1 = shapes[0].mesh.indices[i];
        //V2
        tinyobj::index_t vData2 = shapes[0].mesh.indices[i + 1];
        //V3
        tinyobj::index_t vData3 = shapes[0].mesh.indices[i + 2];

        //v1
        glm::vec3 v1 = glm::vec3(
            attributes.vertices[vData1.vertex_index * 3],
            attributes.vertices[vData1.vertex_index * 3 + 1],
            attributes.vertices[vData1.vertex_index * 3 + 2]
        );

        //v2
        glm::vec3 v2 = glm::vec3(
            attributes.vertices[vData2.vertex_index * 3],
            attributes.vertices[vData2.vertex_index * 3 + 1],
            attributes.vertices[vData2.vertex_index * 3 + 2]
        );

        //v3
        glm::vec3 v3 = glm::vec3(
            attributes.vertices[vData3.vertex_index * 3],
            attributes.vertices[vData3.vertex_index * 3 + 1],
            attributes.vertices[vData3.vertex_index * 3 + 2]
        );

        //UV
        glm::vec2 uv1 = glm::vec2(
            attributes.texcoords[vData1.texcoord_index * 2],
            attributes.texcoords[vData1.texcoord_index * 2 + 1]
        );

        glm::vec2 uv2 = glm::vec2(
            attributes.texcoords[vData2.texcoord_index * 2],
            attributes.texcoords[vData2.texcoord_index * 2 + 1]
        );

        glm::vec2 uv3 = glm::vec2(
            attributes.texcoords[vData3.texcoord_index * 2],
            attributes.texcoords[vData3.texcoord_index * 2 + 1]
        );

        glm::vec3 deltaPos1 = v2 - v1;
        glm::vec3 deltaPos2 = v3 - v1;

        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float r = 1.f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));

        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);

    }

    std::vector<GLuint> mesh_indices;
    std::vector<GLfloat> full_vertex_data;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {

        tinyobj::index_t vertex_data = shapes[0].mesh.indices[i];

        int vertex_index = vertex_data.vertex_index * 3;
        int normal_index = vertex_data.normal_index * 3;
        int texcoord_index = vertex_data.texcoord_index * 2;


        // Position
        full_vertex_data.push_back(attributes.vertices[vertex_index]);
        full_vertex_data.push_back(attributes.vertices[vertex_index + 1]);
        full_vertex_data.push_back(attributes.vertices[vertex_index + 2]);

        // Normals
        full_vertex_data.push_back(attributes.normals[normal_index]);
        full_vertex_data.push_back(attributes.normals[normal_index + 1]);
        full_vertex_data.push_back(attributes.normals[normal_index + 2]);

        // UVs
        full_vertex_data.push_back(attributes.texcoords[texcoord_index]);
        full_vertex_data.push_back(attributes.texcoords[texcoord_index + 1]);


        // tangents
        full_vertex_data.push_back(tangents[i].x);
        full_vertex_data.push_back(tangents[i].y);
        full_vertex_data.push_back(tangents[i].z);


        // bitangents
        full_vertex_data.push_back(bitangents[i].x);
        full_vertex_data.push_back(bitangents[i].y);
        full_vertex_data.push_back(bitangents[i].z);
    }

    GLfloat vertices[]{
        0.f, 0.f, 0.f,
        -0.5f, 0.5f, 0.f,
        0.5, -0.5f, 0.f
    };

    GLuint indices[]{
        0,1,2
    };


    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * full_vertex_data.size(),
        full_vertex_data.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)0
    );

    GLintptr norm_ptr = 3 * sizeof(float);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_TRUE,
        14 * sizeof(GL_FLOAT),
        (void*)norm_ptr
    );

    GLintptr uv_ptr = 6 * sizeof(GLfloat);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)uv_ptr
    );

    GLintptr tangentPtr = 8 * sizeof(float);
    GLintptr bitangentPtr = 11 * sizeof(float);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)tangentPtr);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)bitangentPtr);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

   
    // Perspective projection
    glm::mat4 projection_matrix = glm::perspective(glm::radians(60.0f), height / width, 0.1f, 1000.0f);

    // Light
    glm::vec3 lightColor = glm::vec3(1, 1, 1);
    glm::vec3 lightPos = glm::vec3(0, 0, 0);

    glm::mat4 identity_matrix = glm::mat4(1.0f);
    float ambientStr = 0.8f;
    glm::vec3 ambientColor = lightColor;

    float x, y, z;
    x = y = z = 0.0f;
    z = -5.f; // go down

    float scale_x, scale_y, scale_z;
    scale_x = scale_y = scale_z = 4.0f;

    float rot_x, rot_y, rot_z, theta;
    rot_x = rot_y = rot_z = theta = 0;
    rot_x = 1.0f;
    //float theta = -90.0f;

    float specStr = 3.f;
    float specPhong = 25.0f;

    //glEnable(GL_BLEND);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //theta -= 0.1f;

        //theta = 3.0f;
        //theta2 = mod_x;

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // BLEND FUNCTION
        glEnable(GL_BLEND);
        //glBlendFunc(GL_ONE, GL_ONE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glColor4f(1.0, 1.0, 1.0, 0.5);

        lightPos = glm::vec3(-5, 5, 0);
        //rot_x = rotmodx;
        //rot_y = rotmody;

        //glfwSetKeyCallback(window, Key_Callback);

        /* View matrix initialization */
        glm::vec3 cameraPos = glm::vec3(0, 0, 10.f);
        glm::mat4 cameraPosMatrix = glm::translate(glm::mat4(1.0f), cameraPos * -1.f);

        glm::vec3 WorldUp = glm::normalize(glm::vec3(0, 1.f, 0));
        glm::vec3 CameraCenter = glm::vec3(0, 0.0f, 0);

        glm::vec3 F = CameraCenter - cameraPos;
        F = glm::normalize(F);

        glm::vec3 R = glm::cross(F, WorldUp);
        glm::vec3 U = glm::cross(R, F);

        glm::mat4 cameraOrientationMatrix = glm::mat4(1.0f);


        //manually assign the Matrix
        //Matrix[Column][Row]
        //1st col
        cameraOrientationMatrix[0][0] = R.x;
        cameraOrientationMatrix[1][0] = R.y;
        cameraOrientationMatrix[2][0] = R.z;
        //2nd row
        cameraOrientationMatrix[0][1] = U.x;
        cameraOrientationMatrix[1][1] = U.y;
        cameraOrientationMatrix[2][1] = U.z;
        //3rd row
        cameraOrientationMatrix[0][2] = -F.x;
        cameraOrientationMatrix[1][2] = -F.y;
        cameraOrientationMatrix[2][2] = -F.z;

        glm::mat4 view_matrix = glm::lookAt(cameraPos, CameraCenter, WorldUp);

        theta += 0.02f;

        // for the brickwall
        glm::mat4 transformation_matrix = glm::translate(identity_matrix, glm::vec3(x, y, z));
        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(scale_x, scale_y, scale_z));
        transformation_matrix = glm::rotate(transformation_matrix,
            glm::radians(theta),
            glm::normalize(glm::vec3(rot_x, rot_y, rot_z)));

        transformation_matrix = glm::rotate(transformation_matrix,
            glm::radians(-90.0f),
            glm::normalize(glm::vec3(0, 0, 1)));



        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        glUseProgram(shaderProgram1);

        glm::mat4 sky_view = glm::mat4(1.f);
         
        //remove position of camera
        sky_view - glm::mat4(glm::mat3(view_matrix));

        unsigned int sky_projectionLoc = glGetUniformLocation(shaderProgram1, "projection");
        glUniformMatrix4fv(sky_projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        unsigned int sky_viewLoc = glGetUniformLocation(shaderProgram1, "view");
        glUniformMatrix4fv(sky_viewLoc, 1, GL_FALSE, glm::value_ptr(sky_view));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        glUseProgram(shaderProgram);

        unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

        unsigned int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

        unsigned int ambientStrLoc = glGetUniformLocation(shaderProgram, "ambientStr");
        glUniform1f(ambientStrLoc, ambientStr);

        unsigned int ambientColorLoc = glGetUniformLocation(shaderProgram, "ambientColor");
        glUniform3fv(ambientColorLoc, 1, glm::value_ptr(ambientColor));

        unsigned int cameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");
        glUniform3fv(cameraPosLoc, 1, glm::value_ptr(cameraPos));

        unsigned int specStrLoc = glGetUniformLocation(shaderProgram, "specStr");
        glUniform1f(specStrLoc, specStr);

        unsigned int specPhongLoc = glGetUniformLocation(shaderProgram, "specPhong");
        glUniform1f(specPhongLoc, specPhong);

        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));

        unsigned int transformationLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);

        glActiveTexture(GL_TEXTURE1);
        GLuint tex1Address = glGetUniformLocation(shaderProgram, "norm_tex");
        glBindTexture(GL_TEXTURE_2D, norm_tex);
        glUniform1i(tex1Address, 1);
        
        glBlendEquation(GL_SUBTRACT);
        glActiveTexture(GL_TEXTURE2);
        GLuint tex2Address = glGetUniformLocation(shaderProgram, "tex1");
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(tex2Address, 2);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, full_vertex_data.size() / 14);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderProgram1);

    glfwTerminate();
    return 0;
}