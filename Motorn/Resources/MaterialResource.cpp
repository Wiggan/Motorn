#include "MaterialResource.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
MaterialResource::MaterialResource(const std::string pFileName) {
	mMaterial.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mMaterial.diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	mMaterial.specular = { 0.2f, 0.2f, 0.2f, 16.0f };

	using namespace DirectX;
	using namespace std;
	ifstream ifs;
	ifs.open(pFileName);
	if ( ifs ) {
		string line;
		while ( getline(ifs, line) ) {
			//cout << line << endl;
			if ( line.size() > 0 && line.compare(0, 1, "#") ) {

				istringstream iss(line);
				vector<string> tokens;
				copy(istream_iterator<string>(iss),
					istream_iterator<string>(),
					back_inserter<vector<string> >(tokens));
				if ( strcmp("Ka", tokens[0].c_str()) == 0 ) {
					mMaterial.ambient.x = atof(tokens[1].c_str());
					mMaterial.ambient.y = atof(tokens[2].c_str());
					mMaterial.ambient.z = atof(tokens[3].c_str());
				} else if ( strcmp("Kd", tokens[0].c_str()) == 0 ) {
					mMaterial.diffuse.x = atof(tokens[1].c_str());
					mMaterial.diffuse.y = atof(tokens[2].c_str());
					mMaterial.diffuse.z = atof(tokens[3].c_str());
				} else if ( strcmp("Ks", tokens[0].c_str()) == 0 ) {
					mMaterial.specular.x = atof(tokens[1].c_str());
					mMaterial.specular.y = atof(tokens[2].c_str());
					mMaterial.specular.z = atof(tokens[3].c_str());
				} else if ( strcmp("Ns", tokens[0].c_str()) == 0 ) {
					mMaterial.specular.w = atof(tokens[1].c_str());
				} else if ( strcmp("d", tokens[0].c_str()) == 0 ) {
					mMaterial.diffuse.w = atof(tokens[1].c_str());
				} 
			}
		}
	}
}


MaterialResource::~MaterialResource() {
}

const Material& MaterialResource::getMaterial() {
	return mMaterial;
}