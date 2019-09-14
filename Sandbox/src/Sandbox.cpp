#include "Adonis.h"
#include "glm/ext.hpp"
#include "imgui.h"
#include <iostream>
#include <memory>

#ifdef ADONIS_PLATFORM_WINDOWS
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif



class Sandbox : public Adonis::Application {
public:
	Sandbox() {

	};

	~Sandbox() {};

	inline AD_ON_EVENT_DECL_OVERRIDE(RenderEvent) {

		/*using namespace Adonis::math::literals;
		using namespace Adonis::render;
		using namespace Adonis::rendersystem;

		float width = 10, height = 10, depth = 10;

		float cube_vertices[] = {
			width / 2.0f, -height / 2.0f, depth / 2.0f,	.0f, 1.0f, 1.0f,
			width / 2.0f,  height / 2.0f, depth / 2.0f,	.0f, 1.0f, 1.0f,
			-width / 2.0f, height / 2.0f,  depth / 2.0f,	.0f, 1.0f, 1.0f,
			-width / 2.0f, -height / 2.0f, depth / 2.0f,	.0f, 1.0f, 1.0f,
			width / 2.0f,  -height / 2.0f, -depth / 2.0f,	.0f, 1.0f, 1.0f,
			width / 2.0f,  height / 2.0f, -depth / 2.0f,	.0f, 1.0f, 1.0f,
			-width / 2.0f, -height / 2.0f, -depth / 2.0f,	.0f, 1.0f, 1.0f,
			-width / 2.0f, height / 2.0f, -depth / 2.0f,	.0f, 1.0f, 1.0f
		};

		uint32_t cube_indices[] = {
			0u,1u,2u,
			0u,2u,3u,
			1u,4u,5u,
			1u,5u,2u,
			6u,0u,3u,
			6u,3u,7u,
			6u,4u,5u,
			6u,5u,7u,
			3u,2u,5u,
			3u,5u,7u,
			0u,1u,4u,
			0u,4u,6u
		};

		static auto scene3D = std::make_shared<Scene>(SceneType::Scene3D);
		static auto cube_mesh = std::make_unique<Mesh>(cube_vertices, sizeof(cube_vertices), VertexArrayDesc::standard_pos_color_desc(), cube_indices, sizeof(cube_indices));
		static auto cube_model = std::make_unique<Model>(std::move(cube_mesh));
		static auto cam = std::make_unique<Camera>();
		static auto initialized = false;

		if (!initialized) {
			cam->enable_zoom();
			scene3D->set_pipe(RenderPipeline::test_pipeline_3D());
			scene3D->add_cam(std::move(cam), true);
			scene3D->add_model(std::move(cube_model));
			scene3D->enable_cam();
			renderer()->toggle_wireframe();
			initialized = true;
		}*/


	}

	inline AD_ON_EVENT_DECL_OVERRIDE(ViewportResizeEvent) {

		renderer()->set_viewport(0, 0, event->width(), event->height());

	}

	static float z_translation;
private:
	std::unique_ptr<Adonis::render::RenderPipeline> m_pipe;
	std::unique_ptr<Adonis::render::VertexArray> m_vao;
	std::unique_ptr<Adonis::render::VertexBuffer> m_vbo;


};

float Sandbox::z_translation = -1.0f;


std::unique_ptr<Adonis::Application> Adonis::create_application() {
	return std::make_unique<Sandbox>();
}
	