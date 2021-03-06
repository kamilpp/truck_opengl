#include "model.h"

Model::Model() {
}

bool Model::load(string model_file) {
	clear();
	
	Assimp::Importer Importer;
	const aiScene* pScene = Importer.ReadFile(model_file.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	
	if (!parse_scene(pScene, model_file)) {
		fprintf(stderr, "Error parsing '%s': '%s'\n", model_file.c_str(), Importer.GetErrorString());
		return false;
	} else {
		return true;
	}
}

bool Model::parse_scene(const aiScene *pScene, string &model_file) {
	if (pScene) {
		meshes.reserve(pScene->mNumMeshes);
		materials.reserve(pScene->mNumMaterials);
		
		if (!load_materials(pScene, model_file)) {
			return false;
		}
		
		for (unsigned i = 0, len = pScene->mNumMeshes; i < len; ++i) {
			const aiMesh* paiMesh = pScene->mMeshes[i];
			meshes.push_back(new Mesh(paiMesh, materials[paiMesh->mMaterialIndex]));
		}
		Vertex2D<float>local_min = meshes[0]->get_cords_min(), local_max = meshes[0]->get_cords_max();

		for (unsigned i = 1; i < meshes.size(); i++) {
			local_min.x = min(meshes[i]->get_cords_min().x, local_min.x);
			local_min.y = min(meshes[i]->get_cords_min().y, local_min.y);
			local_max.x = max(meshes[i]->get_cords_max().x, local_max.x);
			local_max.y = max(meshes[i]->get_cords_max().y, local_max.y);
		}
		overall_min_point = local_min;
		overall_max_point = local_max;
		length = local_max.x - local_min.x;
		width = local_max.y - local_min.y;
	} else {
		return false;
	}
	
	return true;
}


bool Model::load_materials(const aiScene* pScene, const string& model_file) {
	// Extract the directory part from the file name
	string::size_type SlashIndex = model_file.find_last_of("/");
	string dir;
	
	if (SlashIndex == string::npos) {
		dir = ".";
	} else if (SlashIndex == 0) {
		dir = "/";
	} else {
		dir = model_file.substr(0, SlashIndex);
	}
	
	// Initialize the materials
	for (unsigned i = 0; i < pScene->mNumMaterials; ++i) {
		materials.push_back(new Material());
		if (!materials[i]->load(pScene->mMaterials[i], dir)) {
			return false;
		}
	}
	
	return true;
}

void Model::clear() {
	for (unsigned i = 0, len = materials.size(); i < len; ++i) {
		delete materials[i];
	}
	
	materials.clear();
	
	for (unsigned i = 0, len = meshes.size(); i < len; ++i) {
		delete meshes[i];
	}
	
	meshes.clear();
}

Model::~Model() {
	clear();
}

void Model::set_mv_matrix(glm::mat4 MV) {
	for (unsigned i = 0, ilen = meshes.size(); i < ilen; ++i) {
		meshes[i]->set_mv_matrix(MV);
	}
}

void Model::draw(bool use_mv, bool apply_material) {
	for (unsigned i = 0, len = meshes.size(); i < len; ++i) {
		meshes[i]->draw(use_mv, apply_material);
	}
}

float Model::get_width() {
	return width;
}

float Model::get_length() {
	return length;
}

Vertex2D<float>Model::get_min_point() {
	return overall_min_point;
}

Vertex2D<float>Model::get_max_point() {
	return overall_max_point;
}
