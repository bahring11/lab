#version 450 core

layout (location = 0) in vec3 coord3d; // 위치 (Position)
layout (location = 1) in vec3 normal;  // 법선 (Normal)

uniform mat4 mvp; 

// 프래그먼트 쉐이더로 보낼 변수 이름 (vColor)
out vec3 vColor; 

void main(void)
{
    gl_Position = mvp * vec4(coord3d, 1.0);
    
    // 법선(Normal)을 색상으로 변환: (-1 ~ 1) 범위를 (0 ~ 1) 범위로 매핑
    vColor = (normal + vec3(1.0, 1.0, 1.0)) * 0.5;
}