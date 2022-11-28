#include "System_Physics.h"

namespace air {
	Physics_debugDraw::Physics_debugDraw() {
		render = new RendererDebug(1000000);
	};
	Physics_debugDraw::~Physics_debugDraw() {
		delete render;
		std::cout << "debug destroyed\n";
	};
	void Physics_debugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
		b2Vec2 p1 = vertices[vertexCount - 1];
		for (int32 i = 0; i < vertexCount; ++i) {
			b2Vec2 p2 = vertices[i];
			render->drawLine(glm::vec3(p1.x, p1.y, 0), glm::vec3(p2.x, p2.y, 0), glm::vec4(color.r, color.g, color.b, 1));
			p1 = p2;
		}
	};
	
	void Physics_debugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
	
	};
	
	void Physics_debugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color) {};
	
	void Physics_debugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) {};
	
	void Physics_debugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
	
	};

	void Physics_debugDraw::DrawTransform(const b2Transform& xf) {};
	
	void Physics_debugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color) {};
	
	void Physics_debugDraw::Flush(C_Camera2d* cam) {
		render->submit(cam->camera);
	}
}