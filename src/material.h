#ifndef _INCLUDE_MATERIAL_H
#define _INCLUDE_MATERIAL_H

#include <string>
#include <cstring>
#include <vector>
#include "shared.h"
#include "texture.h"

class mesh_t;
#include "mesh.h"

using namespace std;

class material_t {
private:
	texture_t *texture;
	float ambient[4]
		,	emission[4]
		,	diffuse[4]
		,	specular[4]
		,	shininess;
	
	double texture_size;
public:
	material_t();
	~material_t();
	
	vector <mesh_t*> meshes;
	void clear();
	bool load (aiMaterial *material, string &dir);
	void apply();
	
	void draw_associated_meshes(glm::mat4 V = glm::mat4(1.0f));
	
	double get_texture_size() {
		return texture_size;
	}
	
	string get_texture_file_name();
	int compare_textures(const material_t &o) const;
	bool operator<(const material_t &o) const;
	bool operator==(const material_t &o) const;

	void substitute(material_t *o);
};

#endif
