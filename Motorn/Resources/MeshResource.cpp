#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "MeshResource.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <stdlib.h> 
#include "../Camera.h"

MeshResource::MeshResource(D3dStuff &pStuff, const std::string &pFileName)
{
	using namespace DirectX;
	using namespace std;
	dev = pStuff.dev;
	devcon = pStuff.devcon;
	mFileName = pFileName;
}

MeshResource::~MeshResource()
{
}

void MeshResource::addListener(MeshResourceListener* pListener) {
	mListeners.push_back(pListener);
}
ID3D11Buffer* MeshResource::getVertexBuffer() {
	return mVertexBuffer;
}
ID3D11Buffer* MeshResource::getIndexBuffer() {
	return mIndexBuffer;
}
int	MeshResource::getIndexCount() {
	return mIndexCount;
}
bool MeshResource::reload() {
	if ( load() ) {
		for ( auto it = mListeners.begin(); it != mListeners.end(); it++ ) {
			(*it)->onMeshLoaded(this);
		}
		return true;
	} else {
		std::cout << "Failed reloading mesh resource!" << std::endl;
		return false;
	}
}
bool MeshResource::load() {
	using namespace DirectX;
	using namespace std;
	ifstream ifs;
	ifs.open(mFileName);
	vector<VERTEX> vertices(0);
	vector<uint16_t> vertexIndices(0);
	vector<XMFLOAT3> positions(0);
	vector<XMFLOAT3> normals(0);
	vector<XMFLOAT2> uvs(0);
	if ( ifs ) {
		string line;
		while ( getline(ifs, line) ) {
			if ( line.size() > 0 && line.compare(0, 1, "#") ) {
				istringstream iss(line);
				vector<string> tokens;
				copy(istream_iterator<string>(iss),
					istream_iterator<string>(),
					back_inserter<vector<string> >(tokens));
				if ( strcmp("v", tokens[0].c_str()) == 0 ) {
					XMFLOAT3 v;
					v.x = (float)atof(tokens[1].c_str());
					v.y = (float)atof(tokens[2].c_str());
					v.z = (float)atof(tokens[3].c_str());
					//v.Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
					//cout << "Adding a vertex: " << tokens[0] << " " << v.X << " " << v.Y << " " << v.Z << endl;
					positions.push_back(v);
				} else if ( strcmp("vt", tokens[0].c_str()) == 0 ) {
					XMFLOAT2 vt;
					vt.x = (float)atof(tokens[1].c_str());
					vt.y = (float)atof(tokens[2].c_str());
					//cout << "Adding a vertex: " << tokens[0] << " " << v.X << " " << v.Y << " " << v.Z << endl;
					uvs.push_back(vt);
				} else if ( strcmp("vn", tokens[0].c_str()) == 0 ) {
					XMFLOAT3 vn;
					vn.x = (float)atof(tokens[1].c_str());
					vn.y = (float)atof(tokens[2].c_str());
					vn.z = (float)atof(tokens[3].c_str());
					//cout << "Adding a vertex: " << tokens[0] << " " << v.X << " " << v.Y << " " << v.Z << endl;
					normals.push_back(vn);
				} else if ( strcmp("f", tokens[0].c_str()) == 0 ) {

					for ( int i = 1; i < tokens.size(); i++ ) {
						//Check for / indexes
						string face = tokens[i];
						int firstSlash = face.find_first_of("/");
						int lastSlash = face.find_last_of("/");

						VERTEX vertex;
						vertex.Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
						vertex.Position = positions[atoi(face.substr(0, firstSlash).c_str()) - 1];

						if ( firstSlash != -1 && lastSlash != firstSlash + 1 ) {
							string uvIndex = face.substr(firstSlash + 1, lastSlash);
							if ( uvIndex.size() > 0 ) {
								vertex.UV = uvs[atoi(uvIndex.c_str()) - 1];
							}
						}

						if ( firstSlash != -1 && lastSlash != -1 && firstSlash != lastSlash ) {
							string normalIndex = face.substr(lastSlash + 1);
							if ( normalIndex.size() > 0 ) {
								vertex.Normal = normals[atoi(normalIndex.c_str()) - 1];
							}
						}

						vertices.push_back(vertex);
						vertexIndices.push_back(vertexIndices.size());
					}
					//cout << "Adding a face: " << tokens[0] << " " << tokens[1] << " " << tokens[2] << " " << tokens[3] << endl;
				}
			}
		}



		D3D11_BUFFER_DESC vbd;
		ZeroMemory(&vbd, sizeof(vbd));

		vbd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		vbd.ByteWidth = sizeof(VERTEX)* vertices.size();
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
		vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

		dev->CreateBuffer(&vbd, NULL, &mVertexBuffer);       // create the buffer

		D3D11_MAPPED_SUBRESOURCE vms;
		HRESULT hr = devcon->Map(mVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &vms);   // map the buffer
		if ( FAILED(hr) ) {
			std::cout << "Failed to map buffer! " << hr << std::endl;
			return false;
		}
		memcpy(vms.pData, &vertices[0], sizeof(VERTEX)* vertices.size());                // copy the data
		devcon->Unmap(mVertexBuffer, NULL);                                     // unmap the buffer


		D3D11_BUFFER_DESC ibd;
		ZeroMemory(&ibd, sizeof(ibd));

		mIndexCount = vertexIndices.size();

		ibd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		ibd.ByteWidth = sizeof(uint16_t)* vertexIndices.size();
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		hr = dev->CreateBuffer(&ibd, NULL, &mIndexBuffer);
		if ( FAILED(hr) ) {
			std::cout << "Failed to map buffer! " << hr << std::endl;
			return false;
		}


		D3D11_MAPPED_SUBRESOURCE ims;
		hr = devcon->Map(mIndexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ims);
		if ( FAILED(hr) ) {
			std::cout << "Failed to map buffer! " << hr << std::endl;
			return false;
		}
		memcpy(ims.pData, &vertexIndices[0], sizeof(uint16_t)* vertexIndices.size());
		devcon->Unmap(mIndexBuffer, NULL);
	} else {
		cout << "File " << mFileName << " not found!" << endl;
		return false;
	}
	return true;
}