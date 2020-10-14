#include "Model.h"
#include "core/TextureManager.h"

Model::Model()
{
	isLoaded = false;
}

Model::Model(const std::string& path)
{
	modelPath = path;
	isLoaded = false;
}

void Model::LoadModel()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ASSIMP error: " << importer.GetErrorString() << std::endl;
		return;
	}

	ProcessNode(scene->mRootNode, scene);

	importer.FreeScene();
	isLoaded = true;
	
	for (auto& mesh : meshes)
	{
		mesh.InitMesh();
	}
}

void Model::DestroyModel()
{
	for (auto& mesh : meshes)
	{
		mesh.DeleteMesh();
	}
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	//Load vertices
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		MATH::Vec3 vec;
		vec.x = mesh->mVertices[i].x;
		vec.y = mesh->mVertices[i].y;
		vec.z = mesh->mVertices[i].z;
		vertex.position = vec;

		vec.x = mesh->mNormals[i].x;
		vec.y = mesh->mNormals[i].y;
		vec.z = mesh->mNormals[i].z;
		vertex.normal = vec;

		if (mesh->mTextureCoords[0])
		{
			MATH::Vec2 uv;
			uv.x = mesh->mTextureCoords[0][i].x;
			uv.y = mesh->mTextureCoords[0][i].y;
			vertex.uv = uv;
		}
		else
		{
			vertex.uv = MATH::Vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	//Load indices
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	//Load materials
	aiColor4D col(0.0f, 0.0f, 0.0f, 0.0f);
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		//Get material color
		material->Get(AI_MATKEY_COLOR_DIFFUSE, col);

		//Get diffuse textures
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, "diffuseTex");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		//Get Specular textures
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType::aiTextureType_SPECULAR, "specularTex");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	MATH::Vec4 color = Vec4(col.r, col.g, col.b, col.a);

	p_min.x = vertices[0].position.x;
	p_min.y = vertices[0].position.y;
	p_min.z = vertices[0].position.z;

	p_max.x = vertices[0].position.x;
	p_max.y = vertices[0].position.y;
	p_max.z = vertices[0].position.z;

	// Loops Through verticies and gives us the min and max verticies and put's them into a vector
	for (size_t i = 1; i < vertices.size(); i++)
	{
		p_min.x = std::min(p_min.x, vertices[i].position.x);
		p_min.y = std::min(p_min.y, vertices[i].position.y);
		p_min.z = std::min(p_min.z, vertices[i].position.z);

		p_max.x = std::max(p_max.x, vertices[i].position.x);
		p_max.y = std::max(p_max.y, vertices[i].position.y);
		p_max.z = std::max(p_max.z, vertices[i].position.z);
	}


	return Mesh(vertices, indices, textures, color);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
	std::vector<Texture> textures;

	//load all textures of a TextureType (specular, diffuse, etc.) from the selected material
	for (size_t i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString path;

		//Get the path to the texture associated with the model
		mat->GetTexture(type, i, &path);

		std::string texturePath(path.C_Str());

		texturePath = texturePath.substr(texturePath.find_last_of('\\') + 1, texturePath.length());
		//Load texture
		TextureManager::GetTexture(texturePath).SetType(typeName);
		textures.push_back(TextureManager::GetTexture(texturePath));
	}
	return textures;
}