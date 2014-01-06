#pragma once
#include "../Structs.h"
#include <iostream>
class MaterialResource {
	Material mMaterial;
public:
	const Material &getMaterial();
	MaterialResource(const std::string pFileName);
	~MaterialResource();
};

