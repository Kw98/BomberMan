#include <time.h>
#include "MapSys.hpp"

static constexpr char	MAP_SYS_NAME[] = "MAP_SYS";

namespace Bomber {
	
	MapSys::MapSys(gloop::GLoop &gloop) {
		_rand.seed(time(NULL));
		gloop::StageManager	&stages = gloop.get_stage_manager();

		gloop::SystemHook hook = gloop::SystemHook{MAP_SYS_NAME, 10, 0, false, [this](GEcm::Register &reg, gloop::GLoop &){
			map_generator(reg);
			return gloop::HookStatus::OK; }};

		stages.get_stage(gloop::StageType::INIT).add_hook(gloop::HookType::EARLY, hook);
	}

	void	MapSys::map_generator(GEcm::Register &reg) {
		generate_wall_ground(reg);
		generate_box(reg);
	}

	void	MapSys::generate_wall_ground(GEcm::Register &reg) {
		for (int y = 0; y < Bomber::MAX_Y; y++) {
			for (int x = 0; x < Bomber::MAX_X; x++) {
				if ((x % 2 == 0 && y % 2 == 0) || y == 0 || x == 0 || y == Bomber::MAX_Y - 1 || x == Bomber::MAX_X - 1) {
					auto entity = reg.create();
					reg.construct<Bomber::Bomber_types>(entity, Bomber::Bomber_types::WALL);
					reg.construct<Bomber::Collision>(entity, Bomber::Collision::BLOCK_ALL);
					reg.construct<Bomber::Graphics>(entity, true, Bomber::WALL_T, Bomber::ItemType::CUBE, Bomber::State::NEW);
					reg.construct<Bomber::Pos>(entity, x, y, 0);
				}
				auto ground = reg.create();
				reg.construct<Bomber::Graphics>(ground, true, Bomber::GROUND_T, Bomber::ItemType::CUBE, Bomber::State::NEW);
				reg.construct<Bomber::Pos>(ground, x, y, -1);
			}
		}
	}

	void	MapSys::generate_box(GEcm::Register &reg) {
		std::uniform_int_distribution<>	distrib;

		for (int y = 1; y < Bomber::MAX_Y - 1; y++) {
			for (int x = 1; x < Bomber::MAX_X - 1; x++) {
				if (distrib(_rand) % 100 > Bomber::POURCENT_MAP_REMPL && !(x % 2 == 0 && y % 2 ==0)) {
					if (!(y == 1 && x == 1) && !(y == 2 && x == 1) && !(y == 1 && x == 2) 
						&& !(y == Bomber::MAX_Y - 2 && x == Bomber::MAX_X - 2) && !(y == Bomber::MAX_Y - 3 && x == Bomber::MAX_X - 2) && !(y == Bomber::MAX_Y - 2 && x == Bomber::MAX_X - 3)) {
						auto entity = reg.create();
						reg.construct<Bomber::Bomber_types>(entity, Bomber::Bomber_types::BOX);
						reg.construct<Bomber::Collision>(entity, Bomber::Collision::BLOCK_PLAYER);
						reg.construct<Bomber::Graphics>(entity, true, Bomber::BOX_T, Bomber::ItemType::CUBE, Bomber::State::NEW);
						reg.construct<Bomber::Pos>(entity, x, y, 0);
					}
				}
			}
		}
	}

} // Bomber