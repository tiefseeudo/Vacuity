#pragma once

#include <unordered_map>
#include "Adonis/Core.h"
#include "Adonis/Rendering/Renderer.h"


namespace Adonis::rendersystem {

	class MeshSpecification {
	public:

		MeshSpecification() = delete;

		MeshSpecification(render::DrawMode mode = render::DrawMode::Triangles, render::DrawMethod method = render::DrawMethod::Classic);

		auto operator==(const MeshSpecification& rhs)const->bool;

		inline auto mode()const->render::DrawMode { return m_draw_mode; };

		inline auto method()const->render::DrawMethod { return m_draw_method; };

	private:
		render::DrawMode m_draw_mode;
		render::DrawMethod m_draw_method;
	};

	template<typename T> class Hasher;

	template<>
	class Hasher<MeshSpecification> {
	public:

		size_t operator()(const MeshSpecification& mg) const {
			auto hash1 = std::hash<uint32_t>()(static_cast<uint32_t>(mg.mode()));
			auto hash2 = std::hash<uint32_t>()(static_cast<uint32_t>(mg.method()));

			return hash1 ^ (hash2 << 1);
		}

	};


	class Mesh {
	public:

	private:
		uint32_t m_ibo;
		uint32_t m_vbo;
		uint32_t m_vao;
		uint32_t m_n_vertices;
		uint32_t m_n_indices;
		
	};


}
