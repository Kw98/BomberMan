#pragma once

#include <fstream>
#include <iostream>
#include <ctime>
#include "Components.hpp"

namespace Bomber {

	class BackgroundSys
	{
	public:
		BackgroundSys(gloop::GLoop &gloop) {
			std::srand(std::time(nullptr));
			gloop::StageManager	&stages = gloop.get_stage_manager();
			gloop::SystemHook hook = gloop::SystemHook{"BACKGROUND_SYS", 10, 0, false, [this](GEcm::Register &reg, gloop::GLoop &) {
				std::ifstream myfile ("assets/backgroundMap.txt");
				std::string line;
				if (myfile.is_open()) {
					int y = -10;
					 while (getline(myfile,line)) {
						int x = -25;
					 	for (auto &&block: line) {
					 		setBlock(reg, x, y, int(block) - '0');
					 		x += 1;
					 	}
					 	y += 5;
					 }
					myfile.close();
				}
				else {
					std::cout << "Unable to open file" << std::endl;;
					for (int y = -30; y < Bomber::MAX_Y + 30; y += 5) {
						for (int x = -30; x < Bomber::MAX_X + 30; x++) {
							int block = std::rand() % 4;
							setBlock(reg, x, y, block);
						}
					}
				}
				return gloop::HookStatus::OK; }};

			stages.get_stage(gloop::StageType::INIT).add_hook(gloop::HookType::EARLY, hook);
		};

		~BackgroundSys() {

		};

	private:
		void	setBlock(GEcm::Register &reg, int &x, int y, int block) {
			if ((x >= 0 && x <= Bomber::MAX_X) &&
				(y > 0 && y < Bomber::MAX_Y)) {
			} else {
				auto ground = reg.create();
				if (block == 1)
					reg.construct<Bomber::Graphics>(ground, true, Bomber::BACKGROUND_GRASS_T, Bomber::ItemType::CUBE, Bomber::State::NEW, 5.0);
				else
					reg.construct<Bomber::Graphics>(ground, true, Bomber::BACKGROUND_T, Bomber::ItemType::CUBE, Bomber::State::NEW, 5.0);
				reg.construct<Bomber::Pos>(ground, x, y, -6);
			}
			x += 4;
		}
	};
} // Bomber
