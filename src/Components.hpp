#pragma once

#include <vector>
#include "Register.hpp"

namespace Bomber {

	constexpr char	GROUND_T[] = "./sprites/BrickGround.jpg";
	constexpr char	WALL_T[] = "./sprites/BrickWall.jpg";
	constexpr char	BOX_T[] = "./sprites/wood_box.png";

	constexpr int	MAX_X = 17;
	constexpr int	MAX_Y = 19;

	template<typename T>
	struct Vec3 {
		T	x;
		T	y;
		T	z;
	};

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
		GET,
	};

	struct Pos : public Vec3<int> {};

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
	};

	struct Input {
		
	};
} // Bomber