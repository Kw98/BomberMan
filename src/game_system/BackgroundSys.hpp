#pragma once

#include "Components.hpp"

namespace Bomber {

	class BackgroundSys
	{
	public:
		BackgroundSys(gloop::GLoop &gloop) {
			gloop::StageManager	&stages = gloop.get_stage_manager();

			gloop::SystemHook hook = gloop::SystemHook{"BACKGROUND_SYS", 10, 0, false, [this](GEcm::Register &reg, gloop::GLoop &) {
				for (int y = -30; y < Bomber::MAX_Y + 30; y += 5) {
					for (int x = -30; x < Bomber::MAX_X + 30; x++) {
						if ((x >= 0 && x <= Bomber::MAX_X) &&
							(y > 0 && y < Bomber::MAX_Y)) {
						} else {
							auto ground = reg.create();
							reg.construct<Bomber::Graphics>(ground, true, Bomber::BACKGROUND_T, Bomber::ItemType::CUBE, Bomber::State::NEW, 5.0);
							reg.construct<Bomber::Pos>(ground, x, y, -6);
							x += 4;
						}
					}
				}
				auto titleImg = reg.create();
				reg.construct<Bomber::Graphics>(titleImg, true, Bomber::TITLE_T, Bomber::ItemType::MESH, Bomber::State::NEW, 1.0);
				reg.construct<Bomber::Pos>(titleImg, 25, 30, 6);
				// ITexture* images = _driver->getTexture(Bomber::TITLE_T);

				// _driver->draw2DImage(images, core::position2d<s32>(25, 30),
				// 	core::rect<s32>(0,0,256,72), 0,
				// 	video::SColor(255,255,255,255), true);

				return gloop::HookStatus::OK; }};

			stages.get_stage(gloop::StageType::INIT).add_hook(gloop::HookType::EARLY, hook);
		};

		~BackgroundSys() {

		};	
	};
} // Bomber
