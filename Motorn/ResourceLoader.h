#pragma once
#include "Mesh.h"
#include <map>


class ResourceLoader {
	static std::map<std::string, Mesh*> meshes;
public:
	static D3dSpriteStuff stuff;
	static Mesh* getMesh(const std::string &mesh);
	static void init(const D3dSpriteStuff &stuff);
	ResourceLoader();
	~ResourceLoader();

};

