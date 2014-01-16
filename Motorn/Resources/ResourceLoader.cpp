#include "ResourceLoader.h"

MeshResourceMap ResourceLoader::mMeshes;
TextureMap ResourceLoader::mTextures;
MaterialMap ResourceLoader::mMaterials;
ShaderMap ResourceLoader::mShaders;
FilesMap ResourceLoader::mFilesToCheck;
D3dStuff ResourceLoader::mStuff;
bool ResourceLoader::mLevelChanged = false;

Texture* ResourceLoader::getTexture(const std::string &pTextureName) {
    using namespace std;
    string fileName = "..\\assets\\textures\\" + pTextureName + ".bmp";
    TextureMap::iterator it = mTextures.find(pTextureName);
    Texture* texture;
    if ( it == mTextures.end() ) {
        texture = new Texture(mStuff, fileName);
        mTextures.insert(pair<string, Texture*>(pTextureName, texture));
        //FileInfo info(fileName, pTextureName, TEXTURE);
        //mFilesToCheck.insert(pair<FileInfo, TimeStamp>(info, getFileTimeStamp(fileName)));
    } else {
        texture = it->second;
    }
    return texture;
}
MeshResource* ResourceLoader::getMeshResource(const std::string &pMeshName) {
    using namespace std;
    string fileName = "..\\assets\\models\\" + pMeshName + ".obj";
    MeshResourceMap::iterator it = mMeshes.find(pMeshName);
    MeshResource* mesh;
    if ( it == mMeshes.end() ) {
        mMeshes.insert(pair<string, MeshResource>(pMeshName, MeshResource(mStuff, fileName)));
        mFilesToCheck.insert(pair<FileInfo, TimeStamp>(FileInfo(fileName, pMeshName, MESH), getFileTimeStamp(fileName)));
        mesh = &mMeshes.find(pMeshName)->second;
        mesh->load();
    } else {
        mesh = &it->second;
    }
    return mesh;
}
MaterialResource* ResourceLoader::getMaterialResource(const std::string &pMaterialName) {
    using namespace std;
    string fileName = "..\\assets\\materials\\" + pMaterialName + ".mtl";
    MaterialMap::iterator it = mMaterials.find(pMaterialName);
    MaterialResource* material;
    if ( it == mMaterials.end() ) {
        mMaterials.insert(pair<string, MaterialResource>(pMaterialName, MaterialResource(fileName)));
        mFilesToCheck.insert(pair<FileInfo, TimeStamp>(FileInfo(fileName, pMaterialName, MATERIAL), getFileTimeStamp(fileName)));
        material = &mMaterials.find(pMaterialName)->second;
        material->load();
    } else {
        material = &it->second;
    }
    return material;
}
Shader* ResourceLoader::getShader(const std::string &pShaderName) {
    using namespace std;
    string fileName = "..\\assets\\shaders\\" + pShaderName + ".hlsl";
    ShaderMap::iterator it = mShaders.find(pShaderName);
    Shader* shader;
    if ( it == mShaders.end() ) {
        shader = new Shader(mStuff, fileName);
        mShaders.insert(pair<string, Shader*>(pShaderName, shader));
        FileInfo info(fileName, pShaderName, SHADER);
        mFilesToCheck.insert(pair<FileInfo, TimeStamp>(info, getFileTimeStamp(fileName)));
    } else {
        shader = it->second;
    }
    return shader;
}
bool ResourceLoader::isLevelChanged() {
    if ( mLevelChanged ) {
        mLevelChanged = false;
        return true;
    } 
    return false;
}
void ResourceLoader::watchLevel(const std::string &pfileName) {
    mFilesToCheck.insert(std::pair<FileInfo, TimeStamp>(FileInfo(pfileName, pfileName, LEVEL), getFileTimeStamp(pfileName)));
}
void ResourceLoader::init(const D3dStuff &s) {
    mStuff = D3dStuff(s);
}
void ResourceLoader::reset() {
    //mMaterials.clear();
    mMeshes.clear();
}
void ResourceLoader::checkForChangedResources() {
    for ( auto it = mFilesToCheck.begin(); it != mFilesToCheck.end(); it++ ) {
        TimeStamp now = getFileTimeStamp(it->first.completePath);
        if ( it->second != now ) {
            it->second = now;
            switch ( it->first.type ) {
            case SHADER: {
                std::cout << "Shader changed! Reloading " << it->first.completePath << std::endl;
                mShaders.erase(it->first.name);
                getShader(it->first.name)->load();
                break;
            }
            case MATERIAL: {
                std::cout << "Material changed! Reloading " << it->first.completePath << std::endl;
                MaterialResource* material = &mMaterials.find(it->first.name)->second;
                material->reload();
                break;
            }
            case MESH: {
                std::cout << "Mesh changed! Reloading " << it->first.completePath << std::endl;
                MeshResource* mesh = &mMeshes.find(it->first.name)->second;
                mesh->reload();
                break;
            }
            case LEVEL: {
                std::cout << "Mesh changed! Informing listeners " << it->first.completePath << std::endl;
                mLevelChanged = true;
                break;
            }
            default:
                std::cout << "File changed! Doing nothing for " << it->first.completePath << std::endl;
                break;
            }
        }
    }
}



TimeStamp ResourceLoader::getFileTimeStamp(const std::string &pPath) {
    TimeStamp timestamp;

    // set default values
    timestamp.year = -1;
    timestamp.month = -1;
    timestamp.day = -1;
    timestamp.hour = -1;
    timestamp.minute = -1;
    timestamp.second = -1;


    HANDLE hFile = 0;
    FILETIME create, access, time;

    hFile = CreateFile(pPath.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0);

    if ( hFile != INVALID_HANDLE_VALUE ) {
        GetFileTime(hFile, &create, &access, &time);
        CloseHandle(hFile);

        // TODO: get local time
        SYSTEMTIME systemTime;
        FileTimeToSystemTime(&time, &systemTime);

        timestamp.year = (short)systemTime.wYear;
        timestamp.month = (short)systemTime.wMonth;
        timestamp.day = (short)systemTime.wDay;
        timestamp.hour = (short)systemTime.wHour;
        timestamp.minute = (short)systemTime.wMinute;
        timestamp.second = (short)systemTime.wSecond;
    }

    return timestamp;
}