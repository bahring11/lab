/*
#version 450

in vec3 fColor; // 받은데이터

//프레그먼트 쉐이더는 반드시 하나 이상의 out vec4가 있어야함

out vec4 fregColor; //픽셀의 색상을 의미 왜 vec4? R, G, B, A 까지 

void main()
{
	fregColor = vec4(fColor, 1.0);
}




#version 450 core

// 버텍스 쉐이더에서 넘어온 데이터 (사용하지 않더라도 선언은 유지해야 에러가 안 납니다)
in vec3 vColor;

// 최종 출력 색상
out vec4 FragColor;

void main(void)
{
    // vColor(보간된 값)를 무시하고, 강제로 빨간색 출력
    // RGBA: (1.0, 0.0, 0.0, 1.0) -> 빨강
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}

*/


#version 450 core

// [수정] 버텍스 쉐이더에서 보낸 이름과 똑같아야 함
in vec3 vColor;

out vec4 FragColor;

void main(void)
{
    FragColor = vec4(vColor, 1.0);
}