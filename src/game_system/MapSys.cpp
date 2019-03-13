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
		generate_map_without_boxes(reg);
	}

	void	MapSys::generate_map_without_boxes(GEcm::Register &reg) {
		for (int y = 0; y < 19; y++) {
			for (int x = 0; x < 17; x++) {
				entity = reg.create();
				reg.construct<Pos>(entity, x, y);
				if (x % 2 == 0 && y % 2 == 0) {
					reg.construct<Case>(entity, Bomber::Bomber_types::WALL);
					reg.construct<Graphic>(entity, "texture");
				} else {
					reg.construct<Case>(entity, Bomber::Bomber_types::NOTHING);
					reg.construct<Graphic>(entity, "texture");
				}
			}
		}
	}

} // Bomber