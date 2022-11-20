#pragma once

#include <entt.hpp>

namespace air {
	class Scene;

	class System {
		friend class Scene;
	public:
		virtual void init() = 0;
		virtual void update(float _deltaTime) = 0;
		virtual void terminate() = 0;

		virtual void updateLast(float _deltaTime) {};
	protected:
		entt::registry* reg = nullptr;
		Scene* scene = nullptr;
	};
}