#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

glm::quat startQuat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // 初始姿态，无旋转
glm::quat endQuat = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // 终止姿态，绕Y轴旋转90度

glm::quat slerp(const glm::quat& start, const glm::quat& end, float t) {
    return glm::mix(start, end, t);
}

void updateModelRotation(glm::quat& modelQuat, float deltaTime) {
    static float elapsedTime = 0.0f;
    elapsedTime += deltaTime;

    if (elapsedTime > 2.0f) {
        elapsedTime = 2.0f;
    }

    float t = elapsedTime / 2.0f;

    modelQuat = slerp(startQuat, endQuat, t);
}

void renderLoop() {
    float deltaTime = 1.0f / 60.0f; // 假设帧率为60FPS

    glm::quat modelQuat;
    updateModelRotation(modelQuat, deltaTime);

}