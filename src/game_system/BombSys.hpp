#pragma once

#include <iostream>
#include <unordered_map>
#include "Components.hpp"
#include "GLoop.hpp"

static constexpr char	BOMB_SYS_NAME[] = "BOMB_SYS";
static constexpr int	BOMB_TIMER = 2000;

namespace Bomber {
	
	class BombSys {
		public:
			BombSys(gloop::GLoop &gloop) {
				gloop::StageManager	&stages = gloop.get_stage_manager();

				gloop::SystemHook	vis = gloop::SystemHook{BOMB_SYS_NAME, 4, 100, false, [this](GEcm::Register &reg, gloop::GLoop &){
					updateBombState(reg);
					return gloop::HookStatus::OK; }};

				stages.get_stage(gloop::StageType::LOOP).add_hook(gloop::HookType::EARLY, vis);

				gloop::SystemHook	update = gloop::SystemHook{BOMB_SYS_NAME, 5, 60, false, [this](GEcm::Register &reg, gloop::GLoop &){
					updateBombTimer(reg);
					return gloop::HookStatus::OK; }};
				stages.get_stage(gloop::StageType::LOOP).add_hook(gloop::HookType::MIDDLE, update);
			}

		private:
			void	updateBombState(GEcm::Register &reg) {
				auto	bombentities = reg.global_view<Bomber::Bomb, Bomber::Pos, Bomber::Graphics, Bomber::Collision>();

				for (auto bomb = bombentities.begin(); bomb != bombentities.end(); bomb++) {
					auto egraph = std::get<Bomber::Graphics *>(bomb->comps);
					if (!egraph->isVisible) {
						auto bombpos = std::get<Bomber::Pos *>(bomb->comps);
						auto	playerentities = reg.global_view<Bomber::Stat, Bomber::Pos>();
						bool	onIt = true;
						for (auto player = playerentities.begin(); player != playerentities.end(); player++) {
							auto playerpos = std::get<Bomber::Pos *>(player->comps);
							if (playerpos->x == bombpos->x && playerpos->y == bombpos->y)
								onIt = false;
						}
						if (onIt) {
							egraph->state = Bomber::State::UPDATE;
							egraph->isVisible = true;
							*(std::get<Bomber::Collision *>(bomb->comps)) = Bomber::Collision::BLOCK_PLAYER;
						}
					}
				}
			}

			void	updateBombTimer(GEcm::Register &reg) {
				auto	bombentities = reg.global_view<Bomber::Bomb, Bomber::Graphics>();
				gloop::gtimer	now = std::chrono::system_clock::now();

				for (auto bomb = bombentities.begin(); bomb != bombentities.end(); bomb++) {
					auto	elasped = std::chrono::duration_cast<std::chrono::milliseconds>(now - std::get<Bomber::Bomb *>(bomb->comps)->put_timer).count();
					if (elasped >= BOMB_TIMER) {
						std::get<Bomber::Graphics *>(bomb->comps)->state = Bomber::State::DELETE;
						createExplosionRadius(reg, std::get<Bomber::Bomb *>(bomb->comps), bomb->id);
					}
				}
			}

			void	createExplosionRadius(GEcm::Register &reg, Bomber::Bomb *bomb, const GEcm::entity_type id) {
				auto	pos = reg.get<Bomber::Pos>(id);
				createExplosion(reg, pos, bomb->parent);
				bool up = true;
				bool down = true;
				bool left = true;
				bool right = true;
				for (int i = 1; i < bomb->explosion_radius + 1; i++) {
					if (up)
						placeExplosion({pos.x, pos.y + i, pos.z}, up, bomb->parent, reg);
					if (down)
						placeExplosion({pos.x, pos.y - i, pos.z}, down, bomb->parent, reg);
					if (left)
						placeExplosion({pos.x - i, pos.y, pos.z}, left, bomb->parent, reg);
					if (right)
						placeExplosion({pos.x + i, pos.y, pos.z}, right, bomb->parent, reg);
				}
			}

			void	placeExplosion(Bomber::Pos pos, bool &conti, const GEcm::entity_type parent, GEcm::Register &reg) {
				if ((pos.x % 2 == 0 && pos.y % 2 == 0) || pos.x == 0 || pos.y == 0 || pos.x == Bomber::MAX_X - 1 || pos.y == Bomber::MAX_Y - 1) {
					conti = false;
					return;
				}
				createExplosion(reg, pos, parent);
			}

			void	createExplosion(GEcm::Register &reg, Bomber::Pos pos, const GEcm::entity_type parent) {
				GEcm::entity_type	ent = reg.create();
				reg.construct<Bomber::Pos>(ent, pos);
				reg.construct<Bomber::Collision>(ent, Bomber::Collision::KILL);
				reg.construct<Bomber::Graphics>(ent, true, Bomber::FIRE_T, Bomber::ItemType::SPHERE, Bomber::State::NEW, 0.5);
				reg.construct<Bomber::Explosion>(ent, parent, 1, std::chrono::system_clock::now());
			}

			/* Data */

	}; // BombSys

} // Bomber