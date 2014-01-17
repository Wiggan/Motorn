#include "Texture.h"
#include "WICTextureLoader.h"
#include <iostream>
Texture::Texture(D3dStuff &pStuff, const std::string &pFileName) {
    using namespace std;
    wstring wideFileName;
    wideFileName.assign(pFileName.begin(), pFileName.end());
    HRESULT hr = DirectX::CreateWICTextureFromFile(pStuff.dev, pStuff.devcon, wideFileName.c_str(), &mTexture, &mTextureView, 0 );
    if ( FAILED(hr) ) {
        cout << "Failed loading texture " << pFileName << " because: " << hr << endl;
    }
}

ID3D11ShaderResourceView* Texture::getTextureView() {
    return mTextureView;
}

Texture::~Texture() {
}
