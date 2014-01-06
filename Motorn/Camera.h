#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

enum DIRECTIONS {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	YAW,
	PITCH,
	ROLL
};

class Camera
{
private:
	Camera();
	~Camera();
	Camera(Camera const&);
	Camera& operator=(Camera const&);
	static Camera* instance;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 up;
	DirectX::XMFLOAT3 target;

	float angle = 120.0f; // ~120 degrees
	float nearest = 0.1f;
	float farthest = 10000;
	float height = 600;
	float width = 800;

	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;

	// Keep track of whether matrices need to be updated
	bool viewChanged;
	bool projectionChanged;

public:
	static Camera& getInstance();
	DirectX::XMFLOAT4X4 getView();
	DirectX::XMFLOAT4X4 getProjection();
	DirectX::XMFLOAT3 getPosition();

	void setProjectionSize(const DirectX::XMINT2 &size);
	void setProjectionAngle(float angle);
	void setNearest(float distance);
	void setFarthest(float distance);
	void move(const DIRECTIONS direction, const float distance);
	void rotate(const DIRECTIONS direction, const float degrees);
	void setTarget(const DirectX::XMFLOAT3 &newTarget);
};

