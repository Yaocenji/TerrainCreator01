#include "render_camera.h"
namespace Render {

Camera::Camera(QObject *parent, float Asp, int FOV, float Near, float Far,
               float Azimuth, float Altitude, float rSense, float zSense,
               float DistanceCoef, QVector3D LookPos)
    : QObject{parent},
      camlookPos(LookPos),
      distanceCoef(DistanceCoef),
      distanceExpont(0),
      azimuth(Azimuth),
      altitude(Altitude),
      rotateSensitive(rSense),
      zoomSensitive(zSense),
      aspectRatio(Asp),
      fov(FOV),
      nearPlane(Near),
      farPlane(Far) {
}

float Camera::distance() {
    return exp(distanceExpont) * distanceCoef;
}

QVector3D Camera::cameraPos() {
    return camlookPos + QVector3D(distance() * sinf(altitude) * cosf(azimuth),
                                  distance() * cosf(altitude),
                                  distance() * sinf(altitude) * sinf(azimuth));
}

QMatrix4x4 Camera::matrixView() {
    QMatrix4x4 view;
    view.setToIdentity();
    view.lookAt(cameraPos(), camlookPos, QVector3D(0, 1, 0));
    return view;
}

QMatrix4x4 Camera::matrixProjection() {
    QMatrix4x4 proj;
    proj.setToIdentity();
    proj.perspective(fov, aspectRatio, nearPlane, farPlane);
    return proj;
}

float Camera::Near() {
    return nearPlane;
}

float Camera::Far() {
    return farPlane;
}

void Camera::setDistanceCoef(float coef) {
    distanceCoef = coef;
}

void Camera::setAspectRatio(float value) {
    aspectRatio = value;
}

void Camera::setFov(int angle) {
    fov = angle;
}

void Camera::setClipPlanes(float near, float far) {
    nearPlane = near;
    farPlane = far;
}

void Camera::rotate(float delta_x, float delta_y) {
    azimuth += delta_x * rotateSensitive;
    altitude -= delta_y * rotateSensitive;
    if (altitude >= M_PI / 2)
        altitude = M_PI / 2;
    else if (altitude <= 0.1)
        altitude = 0.1;
    return;
}

void Camera::translate(float delta_x, float delta_y) {
    float x_move = cos(azimuth) * delta_x - sin(azimuth) * delta_y;
    float y_move = sin(azimuth) * delta_x + cos(azimuth) * delta_y;
    x_move *= 0.001f;
    y_move *= 0.001f;
    camlookPos += QVector3D(-y_move, 0, x_move);
    //    camlookPos += QVector3D(-y_move / sin(altitude), 0, x_move);
    //    distanceExpont = log((distance() + y_move / tan(altitude)) /
    //    distanceCoef);
}

void Camera::zoomin() {
    distanceExpont -= zoomSensitive;
}
void Camera::zoomout() {
    distanceExpont += zoomSensitive;
}

} // namespace Render
