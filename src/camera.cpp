#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

NAMESPACE(sage3d)

Camera::Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& front)
    : _position(position)
    , _front(front)
{
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    _direction = glm::normalize(position - target);
    _right = glm::normalize(glm::cross(up, _direction));
    _up = glm::cross(_direction, _right);
    _front = glm::normalize(_direction);
    update();
}

void Camera::move(CameraMovementDirection direction, float value)
{
    switch (direction)
    {
    case Forward:
        _position += value * _front;
        break;
    case Backward:
        _position -= value * _front;
        break;
    case StrafeRight:
        _position += value * _right;
        break;
    case StrafeLeft:
        _position -= value * _right;
        break;
    }
    update();
}

void Camera::rotate(CameraRotationType type, float angle)
{
    switch (type)
    {
    case Pitch:
        _pitch += angle;
        _pitch = std::fmod(_pitch, 360.0f);
        break;
    case Yaw:
        _yaw += angle;
        _yaw = std::fmod(_yaw, 360.0f);
        break;
    case Roll:
        _roll += angle;
        _roll = std::fmod(_roll, 360.0f);
        break;
    }
    glm::vec3 dir;
    dir.x = std::cos(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));
    dir.y = std::sin(glm::radians(_pitch));
    dir.z = std::sin(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));
    _front = glm::normalize(dir);
    _right = glm::normalize(glm::cross(_front, glm::vec3(0.0f, 1.0f, 0.0f)));
    _up = glm::normalize(glm::cross(_right, _front));
    update();
}

void Camera::update()
{
    _view = glm::lookAt(_position, _position + _front, _up);
    if (_roll > 0)
    {
        _view = glm::rotate(_view, glm::radians(_roll), glm::vec3(0.0f, 0.0f, 1.0f));
    }
}

NAMESPACE_END
