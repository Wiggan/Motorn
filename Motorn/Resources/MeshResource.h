#pragma once
#include <string>
#include "Drawable.h"
#include "Structs.h"
#include "Texture.h"
#include "MaterialResource.h"
#include <vector>

class MeshResource; 
class MeshResourceListener {
public:
	virtual void onMeshLoaded(MeshResource* pUpdatedMeshResource) = 0;
};

class MeshResource
{
	ID3D11Device						*dev;
	ID3D11DeviceContext					*devcon;
	ID3D11Buffer						*mVertexBuffer;
	ID3D11Buffer						*mIndexBuffer;
	int									mIndexCount;
	std::vector<MeshResourceListener*>	mListeners;
	std::string							mFileName;
public:
					MeshResource(D3dStuff &pStuff, const std::string &pFileName);
					~MeshResource();
	bool			load();
	bool			reload();
	void			addListener(MeshResourceListener* pListener);
	ID3D11Buffer*	getVertexBuffer();
	ID3D11Buffer*	getIndexBuffer();
	int				getIndexCount();
};

