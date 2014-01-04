#include "ResourceLoader.h"

std::map<std::string, Mesh*> ResourceLoader::meshes;
D3dSpriteStuff ResourceLoader::stuff;

ResourceLoader::ResourceLoader() {
}


ResourceLoader::~ResourceLoader() {
}

Mesh* ResourceLoader::getMesh(const std::string &meshName) {
	using namespace std;
	map<string, Mesh*>::iterator it = meshes.find(meshName);
	Mesh* mesh;
	if ( it == meshes.end() ) {
		mesh = new Mesh(stuff, "..\\assets\\models\\" + meshName + ".obj");
		meshes.insert(pair<string, Mesh*>(meshName, mesh));
	} else {
		mesh = it->second;
	}
	return mesh;
}

void ResourceLoader::init(const D3dSpriteStuff &s) {
	stuff = D3dSpriteStuff(s);
}

struct timestamp {
	short year;
	short month;
	short day;
	short hour;
	short minute;
	short second;
};


timestamp GetFileTimeStamp(const std::string & _Path) {
	timestamp timestamp;

	// set default values
	timestamp.year = -1;
	timestamp.month = -1;
	timestamp.day = -1;
	timestamp.hour = -1;
	timestamp.minute = -1;
	timestamp.second = -1;

#ifdef __WINDOWS__
	HANDLE hFile = 0;
	FILETIME create, access, time;

	hFile = CreateFile(_Path.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0);

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
#else
#endif

	return timestamp;
}