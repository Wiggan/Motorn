#include "Camera.h"
#include <iostream>

Camera* Camera::instance = 0;

using namespace DirectX;

Camera::Camera()
{
    std::cout << "Creating the camera..." << std::endl;
    position = XMFLOAT3(0.0f, 0.0f, -3.0f);
    target = XMFLOAT3(0.0f, 0.0f, 0.0f);
    XMStoreFloat3(&up, XMLoadFloat3(&position) + XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f)));
    std::cout << "Up is: " << up.x << " " << up.y << " " << up.z << std::endl;

    XMStoreFloat4x4(&view, XMMatrixIdentity());
    XMStoreFloat4x4(&projection, XMMatrixIdentity());

    projectionChanged = true;
    viewChanged = true;
}


Camera::~Camera()
{
}

Camera& Camera::getInstance()
{
    if (instance == NULL) {
        instance = new Camera();
    }
    return *instance;
}

void Camera::setProjectionSize(const DirectX::XMINT2 &size)
{
    std::cout << "New screen size: " << size.x << "x" << size.y << std::endl;
}
void Camera::setProjectionAngle(float angle)
{
    projectionChanged = true;
    this->angle = angle;
}
void Camera::setNearest(float distance)
{
    projectionChanged = true;
    nearest = distance;
}
void Camera::setFarthest(float distance)
{
    projectionChanged = true;
    farthest = distance;
}
DirectX::XMFLOAT3 Camera::getPosition() {
    return position;
}
void Camera::move( const DIRECTIONS direction, const float distance )
{
    viewChanged = true;
    DirectX::XMFLOAT3 dir; 
    switch ( direction ) {
    case LEFT:
        XMStoreFloat3( &dir, XMVector3Normalize( XMVector3Cross(XMLoadFloat3( &target ) - XMLoadFloat3( &position ),
            XMLoadFloat3( &up ) - XMLoadFloat3( &position ) ) ) * distance );
        break;
    case RIGHT:
        XMStoreFloat3( &dir, XMVector3Normalize( XMVector3Cross( XMLoadFloat3( &target ) - XMLoadFloat3( &position ),
            XMLoadFloat3( &up ) - XMLoadFloat3( &position ) ) ) * -distance );
        break;
    case FORWARD:
        XMStoreFloat3( &dir, XMVector3Normalize( XMLoadFloat3( &target ) - XMLoadFloat3( &position ) ) * distance );
        break;
    case BACKWARD:
        XMStoreFloat3( &dir, XMVector3Normalize( XMLoadFloat3( &target ) - XMLoadFloat3( &position ) ) * -distance );
        break;
    }
    //std::cout << "Moving, up is direction " << up.x - position.x << " " << up.y - position.y << " " << up.z - position.z << std::endl;
    XMMATRIX transformation = XMMatrixTranslation( dir.x, dir.y, dir.z );
    XMStoreFloat3(&position, XMVector3Transform(XMLoadFloat3(&position), transformation));
    XMStoreFloat3(&target, XMVector3Transform(XMLoadFloat3(&target), transformation));
    XMStoreFloat3(&up, XMVector3Transform(XMLoadFloat3(&up), transformation));
}

void Camera::rotate(const DIRECTIONS direction, float degrees)
{
    if (degrees == 0.0f)
        return;
    viewChanged = true;

    // rotate vectors
    XMFLOAT3 look_at_target; 
    XMStoreFloat3(&look_at_target, XMLoadFloat3(&target) - XMLoadFloat3(&position));
    //std::cout << "Looking at BEFORE transform: " << target.x << " " << target.y << " " << target.z << std::endl;
    XMFLOAT3 look_at_up; 
    XMStoreFloat3(&look_at_up, XMLoadFloat3(&up) - XMLoadFloat3(&position));


    XMMATRIX transformation; 
    switch ( direction ) {
    case PITCH:
        transformation = XMMatrixRotationAxis(XMVector3Cross(XMLoadFloat3(&look_at_target),
            XMLoadFloat3(&look_at_up)), XMConvertToRadians(degrees)*0.1f);
        //std::cout << "Up before PITCH transform: " << look_at_up.x << " " << look_at_up.y << " " << look_at_up.z << std::endl;
        break;
    case YAW:
        transformation = XMMatrixRotationAxis(XMLoadFloat3(&look_at_up), XMConvertToRadians(degrees)*0.1f);
        //std::cout << "Up before YAW transform: " << look_at_up.x << " " << look_at_up.y << " " << look_at_up.z << std::endl;
        break;
    case ROLL:
        break;
    }

    XMStoreFloat3(&look_at_target, XMVector3Transform(XMLoadFloat3(&look_at_target), transformation));
    XMStoreFloat3(&look_at_up, XMVector3Transform(XMLoadFloat3(&look_at_up), transformation));


    // restore vectors's end points mTarget and mUp from new rotated vectors
    XMStoreFloat3(&target, XMLoadFloat3(&position) + XMLoadFloat3(&look_at_target));
    XMStoreFloat3(&up, XMLoadFloat3(&position) + XMLoadFloat3(&look_at_up));

    //std::cout << "Looking at AFTER transform: " << target.x << " " << target.y << " " << target.z << std::endl;
}

void Camera::setTarget(const DirectX::XMFLOAT3 &newTarget)
{
    viewChanged = true;
    XMFLOAT3 old_look_at_target;
    XMStoreFloat3(&old_look_at_target, XMLoadFloat3(&target) - XMLoadFloat3(&position));
    XMFLOAT3 new_look_at_target;
    XMStoreFloat3(&new_look_at_target, XMLoadFloat3(&newTarget) - XMLoadFloat3(&position));

    float angle = XMConvertToDegrees(XMVectorGetX(
        XMVector3AngleBetweenNormals(XMVector3Normalize(XMLoadFloat3(&old_look_at_target)),
        XMVector3Normalize(XMLoadFloat3(&new_look_at_target)))));
    if (angle != 0.0f && angle != 360.0f && angle != 180.0f)
    {
        XMFLOAT3 axis;
        XMStoreFloat3(&axis, XMVector3Cross(XMLoadFloat3(&old_look_at_target), XMLoadFloat3(&new_look_at_target)));
        //rotate(axis, angle);
    }
    target.x = newTarget.x;
    target.y = newTarget.y;
    target.z = newTarget.z;

}

DirectX::XMFLOAT4X4 Camera::getView() // TODO by reference?
{
    if (viewChanged) {
        //XMStoreFloat3(&up, XMLoadFloat3(&position) + XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f)));
        XMStoreFloat4x4(&view, XMMatrixLookAtLH(XMLoadFloat3(&position), XMLoadFloat3(&target),
            XMLoadFloat3(&up) - XMLoadFloat3(&position)));
        view._31 *= -1;
        view._32 *= -1;
        view._33 *= -1;
        view._34 *= -1;
        XMStoreFloat4x4(&view, XMMatrixTranspose(XMLoadFloat4x4(&view)));
        viewChanged = false;
    }
    return view;
}

DirectX::XMFLOAT4X4 Camera::getProjection()
{
    if (projectionChanged) {

        XMStoreFloat4x4(&projection, XMMatrixPerspectiveFovLH(angle, width / height,
            nearest, farthest));
        XMStoreFloat4x4(&projection, XMMatrixTranspose(XMLoadFloat4x4(&projection)));
        projectionChanged = false;
    }
    return projection;
}