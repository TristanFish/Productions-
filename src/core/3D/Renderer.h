#pragma once
#include <vector>
#include <unordered_map>
#include "graphics/Shader.h"
#include "core/Debug.h"

class MeshRenderer;
class LightComponent;

class Renderer
{
public:
	std::vector<MeshRenderer*> meshRenderers;
	std::vector<LightComponent*> lights;

	void Init();
	void AddMeshRenderer(MeshRenderer* mr);
	void AddLight(LightComponent* light);
	void Render() const;
	void DestroyRenderer();
private:
	Shader gBufferShader;
	Shader resultShader;

	GLuint gBuffer;
	GLuint depthRenderBuffer;

	GLuint depthTexture;
	GLuint stencilTexture;
	GLuint posTexture;
	GLuint normTexture;
	GLuint albedoTexture;

	GLuint vao;
	GLuint vbo;

	Debug pos;
	Debug norm;
	Debug albedo;
	Debug depth;
	Debug stencil;

	//GLuint currentGTexture;

	void BindGBufferTextures() const;
	void UnbindGBufferTextures() const;

	void RenderGBufferResult() const;


	//Attaches the most important lights to this object's shader
	void AttachLights(const MeshRenderer& mr) const;
	//Draws this object into the shadow texture
	void RenderShadowTexture(const MeshRenderer& mr) const;
	//Draws shadows onto this object
	void RenderShade(const MeshRenderer& mr) const;
	//Does bloom
	void RenderBloom(const MeshRenderer& mr) const;
	//Does physics movement
	void RenderPhysics(const MeshRenderer& mr) const;
		/*NONE			
		LIGHTING		
		CREATES_SHADOWS	
		RECIEVES_SHADOWS
		BLOOM			
		PHYSICS_MOVEMENT */
};

