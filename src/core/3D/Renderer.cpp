#include "Renderer.h"
#include "components/3D/MeshRenderer.h"
#include "components/3D/LightComponent.h"
#include "core/Globals.h"
#include "core/Debug.h"
#include <sdl/SDL.h>
#include "custom/Rendering/SkyBox.h"
#include "core/ShaderManager.h"

using namespace MATH;

void Renderer::Init()
{
	skyBox = new SkyBox();

	//Set up renderer shaders
	gBufferShader = ShaderManager::GetShaders("gBufferShaderVert.glsl", "gBufferShaderFrag.glsl");
	resultShader = ShaderManager::GetShaders("gBufferResolveVert.glsl", "gBufferResolveFrag.glsl");
	
	//Create GBuffer
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);

	//Attach depthRenderBuffer
	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

	//Colour texture
	glGenTextures(1, &albedoTexture);
	glBindTexture(GL_TEXTURE_2D, albedoTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, albedoTexture, 0);

	//Normal texture
	glGenTextures(1, &normTexture);
	glBindTexture(GL_TEXTURE_2D, normTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normTexture, 0);

	//Positions texture
	glGenTextures(1, &posTexture);
	glBindTexture(GL_TEXTURE_2D, posTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, posTexture, 0);

	//Dpeth texture (not used for depth calculations, just used for a visualization)
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, depthTexture, 0);

	//Stencil texture not functional yet, going to be used to mark whether lighting/shadows are applied to objects
	glBindTexture(GL_TEXTURE_2D, stencilTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16UI, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, stencilTexture, 0);


	const GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4};
	glDrawBuffers(5, buffers);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		EngineLogger::Error("FrameBuffer not complete", "Renderer.cpp", __LINE__);
	}

	//Set frame buffer back to default
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);


	vao = 0;
	
	float quadVertices[] =
	{	//positions							//uvs
		-1.0f,	-1.0f,	0.0f,		0.0f, 0.0f,
		-1.0f,	1.0f,	0.0f,		0.0f, 1.0f,
		1.0f,	-1.0f,	0.0f,		1.0f, 0.0f,
		1.0f,	1.0f,	0.0f,		1.0f, 1.0f,
	};
	//Setup vertex array for the result of the gBuffer
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

}

void Renderer::AddMeshRenderer(MeshRenderer* mr)
{
	meshRenderers.emplace_back(mr);
}

void Renderer::AddLight(LightComponent* light)
{
	lights.emplace_back(light);
}

void Renderer::Render() const
{
	//Skybox here
	skyBox->Render();
	
	
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//loop through all meshrenderers
	for (size_t i = 0; i < meshRenderers.size(); i++)
	{

		//Check meshrenderers for specific flags and do certain functions based on those flags
		if (meshRenderers[i]->renderFlags & RenderProperties::OVERRIDE_RENDERER)
		{
			meshRenderers[i]->Render();
			continue;
		}

		gBufferShader.RunShader();

	/*	LIGHTING			= 0b00000001,
		CREATES_SHADOWS		= 0b00000010,
		RECIEVES_SHADOWS	= 0b00000100,
		BLOOM				= 0b00001000,
		PHYSICS_MOVEMENT	= 0b00010000,
		TRANSPARENT			= 0b00100000,
		WATER				= 0b01000000,
		OVERRIDE_SHADER		= 0b10000000*/

		const Uint16 stencilMarker = meshRenderers[i]->renderFlags;

		if (meshRenderers[i]->renderFlags & RenderProperties::LIGHTING)
		{
			
		}
		if (meshRenderers[i]->renderFlags & RenderProperties::CREATES_SHADOWS)
		{
			RenderShadowTexture(*meshRenderers[i]);
		}
		if (meshRenderers[i]->renderFlags & RenderProperties::RECIEVES_SHADOWS)
		{
			RenderShade(*meshRenderers[i]);
		}
		if (meshRenderers[i]->renderFlags & RenderProperties::BLOOM)
		{
			RenderBloom(*meshRenderers[i]);
		}
		if (meshRenderers[i]->renderFlags & RenderProperties::PHYSICS_MOVEMENT)
		{
			RenderPhysics(*meshRenderers[i]);
		}
		if (meshRenderers[i]->renderFlags & RenderProperties::WATER)
		{
			RenderWaterEffects(*meshRenderers[i]);
		}

		gBufferShader.TakeUniform("stencilMarker", stencilMarker);
		
		meshRenderers[i]->Render(gBufferShader);
		glUseProgram(0);
	}
	
	pos.DrawCube(Vec3(0), Vec3(15), true, MATH::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	
	
	//Rebind the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);

	pos.DrawTextureToScreen(posTexture, -1.0f, -0.5f, 0.0f, -0.5f);
	norm.DrawTextureToScreen(normTexture, -1.0f, -0.5f, 0.5f, 0.0f);
	albedo.DrawTextureToScreen(albedoTexture, -1.0f, -0.5f, -0.5f, -1.0f);
	depth.DrawTextureToScreen(depthTexture, -1.0f, -0.5f, 1.0f, 0.5f);
	//stencil.DrawTextureToScreen(stencilTexture, 0.0f, 1.0f, 1.0f, 0.0f);
	RenderGBufferResult();

	//Uses the gBufferResolve shader to render the result of the gBuffer


}

void Renderer::DestroyRenderer()
{
	delete(skyBox);
	skyBox = nullptr;
	glDeleteRenderbuffers(1, &depthRenderBuffer);
	glDeleteTextures(1, &depthTexture);
	glDeleteTextures(1, &stencilTexture);
	glDeleteTextures(1, &posTexture);
	glDeleteTextures(1, &normTexture);
	glDeleteTextures(1, &albedoTexture);
	glDeleteFramebuffers(1, &gBuffer);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

SkyBox* Renderer::GetSkyBox()
{
	 return skyBox; 
}


//Binds all the gBuffer textures
void Renderer::BindGBufferTextures() const
{
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(resultShader.GetID(), "albedoTexture"), 0);
	glBindTexture(GL_TEXTURE_2D, albedoTexture);

	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(resultShader.GetID(), "normTexture"), 1);
	glBindTexture(GL_TEXTURE_2D, normTexture);

	glActiveTexture(GL_TEXTURE2);
	glUniform1i(glGetUniformLocation(resultShader.GetID(), "posTexture"), 2);
	glBindTexture(GL_TEXTURE_2D, posTexture);

	glActiveTexture(GL_TEXTURE3);
	glUniform1i(glGetUniformLocation(resultShader.GetID(), "depthTexture"), 3);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	glActiveTexture(GL_TEXTURE4);
	glUniform1i(glGetUniformLocation(resultShader.GetID(), "stencilTexture"), 4);
	glBindTexture(GL_TEXTURE_2D, stencilTexture);
}

void Renderer::UnbindGBufferTextures() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::RenderGBufferResult() const
{
	resultShader.RunShader();

	BindGBufferTextures();
	//AttachLights();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	UnbindGBufferTextures();

	glUseProgram(0);
}

void Renderer::AttachLights() const
{
	for (size_t i = 0; i < lights.size(); i++)
	{
		std::string lightNum = "[" + std::to_string(i) + "]";
		
		resultShader.TakeUniform("lightsPos" + lightNum, lights[i]->gameobject->transform.GetPosition());
		resultShader.TakeUniform("lightsAmb" + lightNum, lights[i]->ambColor);
		resultShader.TakeUniform("lightsDiff" + lightNum, lights[i]->diffColor);
		resultShader.TakeUniform("lightsSpec" + lightNum, lights[i]->specColor);
		resultShader.TakeUniform("lightsIntens" + lightNum, lights[i]->intensity);
		//resultShader.TakeUniform("activeLights", static_cast<int>(lights.size()));
	}
}

void Renderer::RenderShadowTexture(const MeshRenderer& mr) const
{
}

void Renderer::RenderShade(const MeshRenderer& mr) const
{
}

void Renderer::RenderBloom(const MeshRenderer& mr) const
{
}

void Renderer::RenderPhysics(const MeshRenderer& mr) const
{
}

void Renderer::RenderWaterEffects(const MeshRenderer& mr) const
{
}
