#pragma once
#include <glm/glm.hpp>

class Viewer {
public:
    Viewer(
        const glm::vec3& viewPoint, const glm::vec3& viewCenter, const glm::vec3& upVector,
        float fieldOfView, float aspectRatio
    );

    glm::vec3 getViewPoint() const;
    glm::vec3 getViewCenter() const;
    glm::vec3 getUpVector() const;
    float getFieldOfView() const;
    float getAspectRatio() const;

    glm::vec3 getViewDir() const;
    glm::vec3 getImagePlaneHorizDir() const;
    glm::vec3 getImagePlaneVertDir() const;

    void translate(float changeHoriz, float changeVert, bool parallelToViewPlane);
    void zoom(float changeVert);
    void rotate(float changeHoriz, float changeVert);
    void centerAt(const glm::vec3& pos);
    void lookFrom(const glm::vec3& pos);

    void setFieldOfView(float fieldOfView);
    void setAspectRatio(float aspectRatio);
    void setTranslateSpeed(float translateSpeed);
    void setZoomFraction(float zoomFraction);
    void setRotateSpeed(float rotateSpeed);

private:
    void getFrustrumInfo();

    glm::vec3 m_viewPoint;
    glm::vec3 m_viewCenter;
    glm::vec3 m_upVector;
    float m_fieldOfView;
    float m_aspectRatio;

    float m_translateSpeed;
    float m_zoomFraction;
    float m_rotateSpeed;

    glm::vec3 m_viewDir;
    glm::vec3 m_imagePlaneHorizDir;
    glm::vec3 m_imagePlaneVertDir;

    float m_displayWidth;
    float m_displayHeight;
};