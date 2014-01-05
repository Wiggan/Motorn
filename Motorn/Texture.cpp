#include "Texture.h"
#include "WICTextureLoader.h"

Texture::Texture(D3dStuff &pStuff, const std::string &pFileName) {
	using namespace std;
	wstring wideFileName;
	wideFileName.assign(pFileName.begin(), pFileName.end());
	DirectX::CreateWICTextureFromFile(pStuff.dev, pStuff.devcon, wideFileName.c_str(), &mTexture, &mTextureView, 0 );

}

ID3D11ShaderResourceView* Texture::getTextureView() {
	return mTextureView;
}

Texture::~Texture() {
}
