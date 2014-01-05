#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
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
};

struct FileInfo {
	FileInfo(const std::string &pCompletePath, const std::string &pName, ResourceType pType) :
	completePath(pCompletePath), name(pName), type(pType) {

	}
	bool operator<(const FileInfo &other) {
		return name < other.name;
	}
	friend bool operator<(const FileInfo &left, const FileInfo &right) {
		return left.name < right.name;
	}
	std::string completePath;
	std::string name;
	ResourceType type;
};

typedef std::unordered_map<std::string, Drawable**> DrawableMap;
typedef std::unordered_map<std::string, Texture*> TextureMap;
typedef std::unordered_map<std::string, Shader*> ShaderMap;
typedef std::map<FileInfo, TimeStamp> FilesMap;


class ResourceLoader {
	static DrawableMap mMeshes;
	static TextureMap mTextures;
	static ShaderMap mShaders;
	static FilesMap mFilesToCheck;
	static TimeStamp getFileTimeStamp(const std::string &pPath);
	static D3dStuff mStuff;
public:
	static Drawable** getMesh(const std::string &pMeshName, std::vector<Texture*> pTextures);
	static Texture* getTexture(const std::string &pTextureName);
	static Shader* getShader(const std::string &pShaderName);
	static void init(const D3dStuff &pStuff);
	static void checkForChangedResources();
	ResourceLoader();
	~ResourceLoader();

};

