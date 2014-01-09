#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "Sprite.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <stdlib.h> 
#include "../Camera.h"

Mesh::Mesh(D3dStuff &stuff, const std::string &filename, std::vector<Texture*> pTextures, MaterialResource** pMaterialResource)
{
	using namespace DirectX;
	using namespace std;
	dev = stuff.dev;
	devcon = stuff.devcon;
	mTextures = pTextures;
	for ( auto it = pTextures.begin(); it != pTextures.end(); it++ ) {
		mTextureViews.push_back((*it)->getTextureView());
	}
	mMaterialResource = pMaterialResource;
	ifstream ifs;
	ifs.open(filename);
	vector<VERTEX> vertices(0);
	vector<uint16_t> vertexIndices(0);
	vector<XMFLOAT3> positions(0);
	vector<XMFLOAT3> normals(0);
	vector<XMFLOAT2> uvs(0);
	//vector<uint16_t> normalIndices(0);
	if (ifs) {
		string line;
		while (getline(ifs, line)) {
			//cout << line << endl;
			if ( line.size() > 0  && line.compare(0, 1, "#")) {

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
				} else if ( strcmp( "f", tokens[0].c_str() ) == 0 ) {

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
		
		dev->CreateBuffer(&vbd, NULL, &pVBuffer);       // create the buffer

		D3D11_MAPPED_SUBRESOURCE vms;
		HRESULT hr = devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &vms);   // map the buffer
		if ( FAILED(hr) ) {
			std::cout << "Failed to map buffer! " << hr << std::endl;
			return;
		}
		memcpy( vms.pData, &vertices[0], sizeof(VERTEX)* vertices.size() );                // copy the data
		devcon->Unmap(pVBuffer, NULL);                                     // unmap the buffer


		D3D11_BUFFER_DESC ibd;
		ZeroMemory(&ibd, sizeof(ibd));

		n = vertexIndices.size();

		ibd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		ibd.ByteWidth = sizeof(uint16_t)* vertexIndices.size();
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; 
		
		hr = dev->CreateBuffer(&ibd, NULL, &iBuffer);
		if ( FAILED(hr) ) {
			std::cout << "Failed to map buffer! " << hr << std::endl;
			return;
		}


		D3D11_MAPPED_SUBRESOURCE ims;
		hr = devcon->Map(iBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ims);
		if ( FAILED(hr) ) {
			std::cout << "Failed to map buffer! " << hr << std::endl;
			return;
		}
		memcpy(ims.pData, &vertexIndices[0], sizeof(uint16_t)* vertexIndices.size());
		devcon->Unmap(iBuffer, NULL);
	}
	else 
	{
		cout << "File " << filename << " not found!" << endl;
	}
}

std::vector<Texture*> Mesh::getTextures() {
	return mTextures;
}
MaterialResource** Mesh::getMaterial() {
	return mMaterialResource;
}

Mesh::~Mesh()
{
}

void Mesh::draw()
{
	using namespace DirectX;
	constants.worldMatrix = mTransform;
	XMMATRIX A = XMLoadFloat4x4(&mTransform);
	A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR det = XMMatrixDeterminant(A);
	XMStoreFloat4x4(&constants.worldMatrixInverseTranspose, XMMatrixInverse(&det, A));
	constants.material = (*mMaterialResource)->getMaterial();
	setShaderConstants(constants);
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	devcon->IASetIndexBuffer( iBuffer, DXGI_FORMAT_R16_UINT, 0 );
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->PSSetShaderResources(0, 1, &mTextureViews[0]);
	devcon->DrawIndexed(n, 0, 0);
}

