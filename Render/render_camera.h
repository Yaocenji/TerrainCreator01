#ifndef RENDER_CAMERA_H
#define RENDER_CAMERA_H

#include <QMatrix4x4>
#include <QObject>
#include <QVector>
#include <QtMath>
namespace Render {

enum CameraControlMode {
    /// 无
    None,
    /// 旋转
    Rotate,
    /// 平移
    Translate
};

class Camera : public QObject {
    Q_OBJECT
protected:
    /// 摄像机看向的位置（是海平面/xz平面上的一点）
    QVector3D camlookPos;
    /// 距离系数
    float distanceCoef;
    /// 距离指数
    float distanceExpont;
    /// 方位角
    float azimuth;
    /// 高度角
    float altitude;
    /// 旋转灵敏度
    float rotateSensitive;
    /// 缩放灵敏度
    float zoomSensitive;
    /// 宽高比
    float aspectRatio;
    /// 垂直方向视野（角度）
    int fov;
    /// 近平面
    float nearPlane;
    /// 远平面
    float farPlane;

public:
    /// 获得当前距离
    float distance();
    /// 获得摄像机位置
    QVector3D cameraPos();
    /// 获取当前的view矩阵
    QMatrix4x4 matrixView();
    /// 获取当前的proj矩阵
    QMatrix4x4 matrixProjection();
    /// 设置距离系数
    void setDistanceCoef(float coef);
    /// 设置宽高比
    void setAspectRatio(float value);
    /// 设置fov
    void setFov(int angle);
    /// 设置近远平面
    void setClipPlanes(float near, float far);

    /// 旋转摄像机
    void rotate(float delta_x, float delta_y);
    /// 移动摄像机
    void translate(float delta_x, float delta_y);
    /// 拉近
    void zoomin();
    /// 拉远
    void zoomout();

public:
    explicit Camera(QObject *parent = nullptr, float Asp = 16.0 / 9.0,
                    int FOV = 25, float Near = 0.01, float Far = 50.0,
                    float Azimuth = 0.0, float Altitude = M_PI / 4.0,
                    float rSense = 0.005, float zSense = 0.1,
                    float DistanceCoef = 1.5,
                    QVector3D LookPos = QVector3D(0, 0, 0));

signals:
};

} // namespace Render

#endif // RENDER_CAMERA_H
