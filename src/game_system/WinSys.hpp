#pragma once

#include <iostream>
#include <unordered_map>
#include "Components.hpp"
#include "GLoop.hpp"

static constexpr char	WIN_SYS_NAME[] = "WIN_SYS";

namespace Bomber {

	class WinSys {
		public:
			WinSys(gloop::GLoop &gloop) {
				gloop::SystemHook win = gloop::SystemHook{WIN_SYS_NAME, 0, 0, false, [this](GEcm::Register &reg, gloop::GLoop &){
					if (reg.getPool<Bomber::Stat>()->size() == 1)
						return gloop::HookStatus::STOP_LOOP;
					return gloop::HookStatus::OK; }};

				gloop.get_stage_manager().get_stage(gloop::StageType::LOOP).add_hook(gloop::HookType::MIDDLE, win);
			}

		private:

			/* Data */
	}; // WinSys

} // Bomber