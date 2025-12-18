
#define _USE_MATH_DEFINES 
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <fstream>
#include <chrono>

#include "cow.h"
#include "ppm_io.h"


void convertNDCtoImage(const glm::vec4 vertexNDC, glm::vec4 vertexView, const uint32_t& imageWidth, const uint32_t& imageHeight, glm::vec3& vertexRaster)
{
    //code here
    vertexRaster.x = (vertexNDC.x + 1.0f) / 2 * imageWidth;
    vertexRaster.y = (1.0f - vertexNDC.y) / 2 * imageHeight;
    vertexRaster.z = -vertexView.z;
}



float min3(const float& a, const float& b, const float& c)
{
    return std::min(a, std::min(b, c));
}

float max3(const float& a, const float& b, const float& c)
{
    return std::max(a, std::max(b, c));
}

//벡터의 포인트가 오른쪽인지 확인하는 함수
bool edge(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
    return ((c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x) >= 0);
}

// 세가지의 꼭짓점을 이루어진 삼각형의 면적을 구하기
float area(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c)
{
    return fabs((c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]));
}

glm::mat4 lookAt(glm::vec3 campos, glm::vec3 look, glm::vec3 up)
{

    //code here
    glm::vec3 z = glm::normalize(campos - look);
    glm::vec3 x = glm::normalize(glm::cross(up, z));
    glm::vec3 y = glm::normalize(glm::cross(z, x));
    glm::mat4 T = glm::translate(glm::mat4(1), -campos);
    glm::mat4 R{ glm::vec4(x.x, y.x, z.x, 0), glm::vec4(x.y, y.y, z.y, 0), glm::vec4(x.z, y.z, z.z, 0), glm::vec4(0, 0, 0, 1) };

    return R * T;

}

glm::mat4 perspective(float fovy, float aspect, float near, float far)
{
    fovy = glm::radians(fovy);

    glm::mat4 PS(0.0f);

    PS[0][0] = 1.0f / (aspect * glm::tan(fovy / 2.0f));

    PS[1][1] = 1.0f / (glm::tan(fovy / 2.0f));

    PS[2][2] = -(far + near) / (far - near);

    PS[2][3] = -1.0f;

    PS[3][2] = -(2.0f * far * near / (far - near));

    return PS;

}

const uint32_t imageWidth = 640;
const uint32_t imageHeight = 480;


const uint32_t ntris = 3156;
const float nearClippingPlane = 1;
const float farClippingPLane = 1000;



int main(int argc, char** argv)
{

    glm::mat4 worldToCamera = glm::lookAt(glm::vec3(20, 10, 20), glm::vec3(0, 5, 0), glm::vec3(0, 1, 0));

    float t, b, l, r;

    PPM ppmOut;
    ppmOut.setBinary(true);



    struct rgb
    {
        uint8_t r, g, b;
    };

    std::vector <rgb>  frameBuffer;
    frameBuffer.resize(imageWidth * imageHeight);


    for (uint32_t i = 0; i < imageWidth * imageHeight; ++i) {
        frameBuffer[i].r = 0;
        frameBuffer[i].g = 0;
        frameBuffer[i].b = 0;
    }

    float* depthBuffer = new float[imageWidth * imageHeight];

    for (uint32_t i = 0; i < imageWidth * imageHeight; ++i) {
        depthBuffer[i] = farClippingPLane;
    }

    auto t_start = std::chrono::high_resolution_clock::now();

    for (uint32_t i = 0; i < ntris; ++i) {

        const glm::vec3& v0 = vertices[nvertices[i * 3]];
        const glm::vec3& v1 = vertices[nvertices[i * 3 + 1]];
        const glm::vec3& v2 = vertices[nvertices[i * 3 + 2]];


        glm::mat4 modelMatrix(1.0f);

        glm::mat4 viewMatrix = lookAt(glm::vec3(20, 10, 20), glm::vec3(0, 5, 0), glm::vec3(0, 1, 0));

        glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

        //Camera(view) coordinates
        glm::vec4 v0e;
        glm::vec4 v1e;
        glm::vec4 v2e;

        //Code here (calculate v0e, v1e, v2e here)
        v0e = modelViewMatrix * glm::vec4(v0, 1.0);
        v1e = modelViewMatrix * glm::vec4(v1, 1.0);
        v2e = modelViewMatrix * glm::vec4(v2, 1.0);


        glm::mat4 projection = perspective((45.0f), imageWidth / (float)imageHeight, nearClippingPlane, farClippingPLane);



        //Clip coodinates
        glm::vec4 v0c, v1c, v2c;

        //Code here (calculate v0c, v1c, v2c here)
        v0c = projection * v0e;
        v1c = projection * v1e;
        v2c = projection * v2e;


        //Perspective division (divide v0c.x, v0c.y, v0c.z by v0c.w   (same to v1c, v2c)


        //Code here
        v0c.x /= v0c.w; v0c.y /= v0c.w; v0c.z /= v0c.w;
        v1c.x /= v1c.w; v1c.y /= v1c.w; v1c.z /= v1c.w;
        v2c.x /= v2c.w; v2c.y /= v2c.w; v2c.z /= v2c.w;



        glm::vec3 v0Raster, v1Raster, v2Raster;
        convertNDCtoImage(v0c, v0e, imageWidth, imageHeight, v0Raster);
        convertNDCtoImage(v1c, v1e, imageWidth, imageHeight, v1Raster);
        convertNDCtoImage(v2c, v2e, imageWidth, imageHeight, v2Raster);



        float xmin = min3(v0Raster.x, v1Raster.x, v2Raster.x);
        float ymin = min3(v0Raster.y, v1Raster.y, v2Raster.y);
        float xmax = max3(v0Raster.x, v1Raster.x, v2Raster.x);
        float ymax = max3(v0Raster.y, v1Raster.y, v2Raster.y);

        if (xmin > imageWidth - 1 || xmax < 0 || ymin > imageHeight - 1 || ymax < 0) continue;

        uint32_t x0 = std::max(int32_t(0), (int32_t)(std::floor(xmin)));
        uint32_t x1 = std::min(int32_t(imageWidth) - 1, (int32_t)(std::floor(xmax)));
        uint32_t y0 = std::max(int32_t(0), (int32_t)(std::floor(ymin)));
        uint32_t y1 = std::min(int32_t(imageHeight) - 1, (int32_t)(std::floor(ymax)));


        //calculat the area of triangle  (area)
        float total = area(v0Raster, v1Raster, v2Raster);
        v0Raster.z = 1.0f / v0Raster.z,
        v1Raster.z = 1.0f / v1Raster.z,
        v2Raster.z = 1.0f / v2Raster.z;



        for (uint32_t y = y0; y <= y1; ++y) {
            for (uint32_t x = x0; x <= x1; ++x) {

                glm::vec3 pixelSample(x + 0.5, y + 0.5, 0);
                // 픽셀의 정가운데를 테스트하기위하여 0.5를 더한다

                //calculate the areas of  three suvdivided triangles


                bool w0 = edge(v1Raster, v2Raster, pixelSample); //w0
                bool w1 = edge(v2Raster, v0Raster, pixelSample); //w1
                bool w2 = edge(v0Raster, v1Raster, pixelSample); //w2


                if (w0 == true && w1 == true && w2 == true) { //inside

                    float a2 = area(v0Raster, v1Raster, pixelSample);
                    float a0 = area(v1Raster, v2Raster, pixelSample);
                    float a1 = area(v2Raster, v0Raster, pixelSample);

                    //calculate the ratio here
                    a0 /= total;
                    a1 /= total;
                    a2 /= total;

                    //calculate the z of pixelSample
                    //픽셀 샘플에 z값을 파악해야한다 = 픽셀샘플의 카메라까지의 거리
                    


                    //code here
                    float oneOverZ = v0Raster.z * a0 + v1Raster.z * a1 + v2Raster.z * a2;
                    float z = 1 / oneOverZ;


                    // 덱스버퍼에는 해당 픽셀의 가장 가까운 (카메라 까지의) 이 들어있음

                   

                    if (z < depthBuffer[y * imageWidth + x]) {

                        depthBuffer[y * imageWidth + x] = z;

                        
                        //calculate normal vector from v0e, v1e, v2e
                        //nomal >1

                        glm::vec3 edge1 = glm::vec3(v1e - v0e);
                        glm::vec3 edge2 = glm::vec3(v2e - v0e);

                        glm::vec3 n = glm::normalize(glm::cross(edge1, edge2));

                        n = (n + glm::vec3(1.0f, 1.0f, 1.0f)) / 2.0f;
                        // n -> (-1 , 1) x, y, z 가 모두 사이 -> [0,1] 노말라이즈 하라는거 같음
                        // [-1,1] -> [0,1]

                        frameBuffer[y * imageWidth + x].r = (uint8_t)(n.x * 255.0f);
                        frameBuffer[y * imageWidth + x].g = (uint8_t)(n.y * 255.0f);
                        frameBuffer[y * imageWidth + x].b = (uint8_t)(n.z * 255.0f);
                        
                    }
                }
            } // 안
        } //바깥
    }

    auto t_end = std::chrono::high_resolution_clock::now();
    auto passedTime = std::chrono::duration<double, std::milli>(t_end - t_start).count();
    std::cerr << "Wall passed time: " << passedTime << "ms" << std::endl;


    rgb* p = frameBuffer.data();

    ppmOut.load(&p[0].r, imageHeight, imageWidth, 255, "P6");
    ppmOut.write("../../output.ppm");

    delete[] depthBuffer;

    return 0;
}