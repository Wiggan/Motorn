#pragma once
#include "MeshResource.h"
#include "Sprite.h"
#include "Shader.h"
#include "Texture.h"
#include "MaterialResource.h"
#include <unordered_map>
#include <map>
#include <string>

struct TimeStamp {

    bool operator!=(const TimeStamp &other) {
        return year != other.year || month != other.month || day != other.day || 
            hour != other.hour || minute != other.minute || second != other.second;
    }

    short year;
    short month;
    short day;
    short hour;
    short minute;
    short second;
};

enum ResourceType {
    SHADER,
    MESH,
    TEXTURE,
    MATERIAL,
    LEVEL,
};

struct FileInfo {
    FileInfo(const std::string &pCompletePath, const std::string &pName, ResourceType pType) :
    completePath(pCompletePath), name(pName), type(pType) {}
    bool operator<(const FileInfo &other) {
        return completePath < other.completePath;
    }
    friend bool operator<(const FileInfo &left, const FileInfo &right) {
        return left.completePath < right.completePath;
    }
    std::string completePath;
    std::string name;
    ResourceType type;
};

typedef std::unordered_map<std::string, Drawable**> DrawableMap;
typedef std::unordered_map<std::string, MeshResource> MeshResourceMap;
typedef std::unordered_map<std::string, Texture*> TextureMap;
typedef std::unordered_map<std::string, MaterialResource> MaterialMap;
typedef std::unordered_map<std::string, Shader*> ShaderMap;
typedef std::map<FileInfo, TimeStamp> FilesMap;


class ResourceLoader {
    static MeshResourceMap      mMeshes;
    static TextureMap           mTextures;
    static MaterialMap          mMaterials;
    static ShaderMap            mShaders;
    static FilesMap             mFilesToCheck;
    static TimeStamp            getFileTimeStamp(const std::string &pPath);
    static D3dStuff             mStuff;
    static bool                 mLevelChanged;
public:
    static MeshResource*        getMeshResource(const std::string &pMeshName);
    static MaterialResource*    getMaterialResource(const std::string &pMaterialName);
    static Texture*             getTexture(const std::string &pTextureName);
    static Shader*              getShader(const std::string &pShaderName);
    static bool                 isLevelChanged();
    static void                 watchLevel(const std::string &pfileName);
    static void                 init(const D3dStuff &pStuff);
    static void                 checkForChangedResources();
                                ResourceLoader() {}
                                ~ResourceLoader() {}
    //static Drawable** getSprite(Texture* pTexture);

};

