#pragma once

#include <vector>
#include "Register.hpp"

struct vec3df {
	double	x;
	double	y;
	double	z;
};

namespace Bomber {

	template<typename T>
	struct Vec2 {
		T	x;
		T	y;
	};

	enum Bomber_types {
		PATH,
		WALL,
		BOX,
	};

	enum Bomber_bonus {
		NOTHING,
		BIG_EXPLOSION,
		LIFE,
		MORE_BOMB,
	};

	enum Collision {
		KILL,
		BLOCK_ALL,
		BLOCK_PLAYER,
		PASS,
	};

	struct Pos : public Vec2<int> {};

	struct Stat {
		int	max_bomb;
		int	explosion_radius;
		int	life;
	};

	struct Bomb {
		GEcm::entity_type	parent;
		int			explosion_radius;
	};

	struct Graphics {
		bool		isVisible;
		std::string	texture;
		vec3df pos;
	};
	

} // Bomber
