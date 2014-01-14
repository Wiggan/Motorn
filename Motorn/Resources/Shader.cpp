#include "Shader.h"
#include <iostream>
#include <fstream>
#include "D3Dcompiler.h"
#include "../Structs.h"
#include "../Vertex.h"

Shader::Shader(D3dStuff &pStuff, const std::string &pFileName) {
    using namespace std;

    dev = pStuff.dev;
    devcon = pStuff.devcon;

    // Load shader file
    ifstream ifs(pFileName, std::ios::binary);
    ifs.seekg(0, std::ios_base::end);
    size = (unsigned int)(1 + ifs.tellg());
    ifs.seekg(0, std::ios_base::beg);
    shader = new char[size];
    for ( int i = 0; i < size; i++ ) {
        shader[i] = 0;
    }
    shader[size - 1] = '\0';
    ifs.read(shader, size);

    if ( size == 0 ) {
        cout << "File is empty!" << endl;
    }

    
}

bool Shader::load() {
    using namespace std;
    ID3D10Blob *VS, *PS, *error = NULL;
    D3DCompile(shader, size, NULL, NULL, NULL, "VShader", "vs_5_0", 0, 0, &VS, &error);
    if ( error != NULL && error->GetBufferSize() > 0 ) {
        char* err = (char*)malloc(error->GetBufferSize());
        memcpy(err, error->GetBufferPointer(), error->GetBufferSize());
        cout << err << endl;
        free(err);
        cout << "Shader loading failed, falling back to standard" << endl;
        return false;
    }
    error = NULL;
    D3DCompile(shader, size, NULL, NULL, NULL, "PShader", "ps_5_0", 0, 0, &PS, &error);
    if ( error != NULL &&  error->GetBufferSize() > 0 ) {
        char* err = (char*)malloc(error->GetBufferSize());
        memcpy(err, error->GetBufferPointer(), error->GetBufferSize());
        cout << err << endl;
        free(err);
        cout << "Shader loading failed, falling back to standard" << endl;
        return false;
    }
    dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &mVertexShader);
    devcon->VSSetShader(mVertexShader, 0, 0);
    dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &mPixelShader);
    devcon->PSSetShader(mPixelShader, 0, 0);
    ID3D11InputLayout* layout;
    dev->CreateInputLayout(ied, sizeof(ied) / sizeof(*ied), VS->GetBufferPointer(), VS->GetBufferSize(), &layout);
    devcon->IASetInputLayout(layout);
    return true;
}

Shader::~Shader() {
    if ( mVertexShader != nullptr )
        mVertexShader->Release();
    if ( mPixelShader != nullptr )
        mPixelShader->Release();
    delete shader;
}
