#include "Map.hpp"

static constexpr char	MAP_SYS_NAME[] = "MAP_SYS";

namespace Bomber {
	
	MapSys::MapSys(gloop::GLoop &gloop) {
		gloop::StageManager	&stages = gloop.get_stage_manager();

		gloop::SystemHook hook = gloop::SystemHook{MAP_SYS_NAME, 10, 0, false, [this](GEcm::Register &reg, gloop::GLoop &){
			map_generator(reg);
			return gloop::HookStatus::OK; }};

		stages.get_stage(gloop::StageType::INIT).add_hook(gloop::HookType::EARLY, hook);
	}

	void	MapSys::map_generator(GEcm::Register &reg) {
		generate_wall_ground(reg);
	}

	void	MapSys::generate_wall_ground(GEcm::Register &reg) {
		for (int y = 0; y < Bomber::MAX_Y; y++) {
			for (int x = 0; x < Bomber::MAX_X; x++) {
				if (x % 2 == 0 && y % 2 == 0) {
					auto entity = reg.create();
					reg.construct<Bomber::Bomber_types>(entity, Bomber::Bomber_types::WALL);
					reg.construct<Bomber::Collision>(entity, Bomber::Collision::BLOCK_ALL);
					reg.construct<Bomber::Graphics>(entity, true, Bomber::WALL_T);
					reg.construct<Bomber::Pos>(entity, x, y, 0);
				}
				auto ground = reg.create();
				reg.construct<Bomber::Graphics>(ground, true, Bomber::GROUND_T);
				reg.construct<Bomber::Pos>(ground, x, y, -1);
			}
		}
	}

} // Bomber