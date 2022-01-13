#ifndef CAMERA_H
#define CAMERA_H
#include <macros.h>
#include <glm/mat4x4.hpp>

NAMESPACE(sage3d)

enum CameraMovementDirection
{
    Forward, Backward,
    StrafeRight, StrafeLeft
};
enum CameraRotationType
{
    Pitch, Yaw, Roll
};

class Camera
{
    READ_PROP(position, glm::vec3)
    READ_PROP(right, glm::vec3)
    READ_PROP(up, glm::vec3)
    READ_PROP(direction, glm::vec3)
    READ_PROP(front, glm::vec3)
    READ_PROP(view, glm::mat4)
    READ_PROP(pitch, float)
    READ_PROP(yaw, float)
    READ_PROP(roll, float)
public:
    Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& front);
    void move(CameraMovementDirection direction, float value);
    void rotate(CameraRotationType type, float angle);
private:
    void update();
};

NAMESPACE_END

#endif // CAMERA_H
