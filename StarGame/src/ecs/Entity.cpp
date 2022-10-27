#include "EntityComponent.h"

namespace air {
	Entity::Entity(entt::entity _id, Scene* _scene)
		: scene(_scene), entity_handle(_id) {
		
	}

	Entity::~Entity() {

	}
}