#pragma once

namespace air {
	class Entity;

	class Component {
		friend class Entity;
	public:
		Entity entity() {
			return _gameObject;
		}
		Entity _gameObject;
	};
}