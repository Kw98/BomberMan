#pragma once

#include <iostream>
#include "Components.hpp"
#include "GLoop.hpp"

static constexpr char	PLAYER_SYS_NAME[] = "PLAYER_SYS";

namespace Bomber {

	class PlayerSys {
		public:
			PlayerSys(gloop::GLoop &gloop) {
				gloop::StageManager	&stages = gloop.get_stage_manager();

				gloop::SystemHook init = gloop::SystemHook{PLAYER_SYS_NAME, 9, 0, false, [this](GEcm::Register &reg, gloop::GLoop &){
					_playerO = createPlayer(reg, {1, 1, 0}, Bomber::PLAYER_ONE_T);
					_playerT = createPlayer(reg, {Bomber::MAX_X - 2, Bomber::MAX_Y - 2, 0}, Bomber::PLAYER_TWO_T);
					return gloop::HookStatus::OK; }};

				stages.get_stage(gloop::StageType::INIT).add_hook(gloop::HookType::EARLY, init);

				gloop::SystemHook actions = gloop::SystemHook{PLAYER_SYS_NAME, 5, 60, false, [this](GEcm::Register &reg, gloop::GLoop &){
					playerAction(reg, _playerO);
					playerAction(reg, _playerT);
					return gloop::HookStatus::OK; }};
				stages.get_stage(gloop::StageType::LOOP).add_hook(gloop::HookType::MIDDLE, actions);
			}

		private:
			GEcm::entity_type	createPlayer(GEcm::Register &reg, const Bomber::Pos pos, const std::string &texture) {
				GEcm::entity_type id = reg.create();
				reg.construct<Bomber::Inputs>(id);
				reg.construct<Bomber::Pos>(id, pos);
				reg.construct<Bomber::Stat>(id, 1, 1, 1);
				reg.construct<Bomber::Graphics>(id, true, texture, Bomber::ItemType::SPHERE, Bomber::State::NEW, 0.5);
				reg.construct<Bomber::Collision>(id, Bomber::Collision::PASS);
				return id;
			}

			void	playerAction(GEcm::Register &reg, const GEcm::entity_type ent) {
				auto	view = reg.safe_view<Bomber::Inputs, Bomber::Stat, Bomber::Pos>(ent);
				auto	in = std::get<Bomber::Inputs *>(view);
				if (in->keys.size() > 0) {
					std::cout << "ENT: " << ent << "  keys num: " << in->keys.size() << std::endl;
					while (!in->keys.empty())
						in->keys.pop();
				}
				// auto	otherEnt = reg.global_view<Bomber::Pos, Bomber::Collision>();


			}

			/* Data */
			GEcm::entity_type	_playerO;
			GEcm::entity_type	_playerT;

	}; // MapSys

} // Bomber