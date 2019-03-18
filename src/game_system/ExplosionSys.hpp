#pragma once

#include <iostream>
#include <unordered_map>
#include <random>
#include <time.h>
#include "Components.hpp"
#include "GLoop.hpp"

static constexpr char	EXPLOSION_SYS_NAME[] = "EXPLOSION_SYS";
static constexpr int	EXPLOSION_TIMER = 500;

namespace Bomber {
	
	class ExplosionSys {
		public:
			ExplosionSys(gloop::GLoop &gloop) {
				_rand.seed(time(NULL));
				gloop::StageManager	&stages = gloop.get_stage_manager();

				gloop::SystemHook	kill = gloop::SystemHook{EXPLOSION_SYS_NAME, 4, 50, false, [this](GEcm::Register &reg, gloop::GLoop &){
					killPlayerBox(reg);

					return gloop::HookStatus::OK; }};

				stages.get_stage(gloop::StageType::LOOP).add_hook(gloop::HookType::MIDDLE, kill);
			}

		private:
			void	killPlayerBox(GEcm::Register &reg) {
				auto	explosions = reg.global_view<Bomber::Explosion, Bomber::Pos>();
				auto	others = reg.global_view<Bomber::Collision, Bomber::Pos>();
				gloop::gtimer	now = std::chrono::system_clock::now();

				for (auto explosion = explosions.begin(); explosion != explosions.end(); explosion++) {
					auto	elasped = std::chrono::duration_cast<std::chrono::milliseconds>(now - std::get<Bomber::Explosion *>(explosion->comps)->timer).count();
					if (elasped >= EXPLOSION_TIMER) {
						reg.get<Bomber::Graphics>(explosion->id).state = Bomber::State::DELETE;
						continue;
					}
					auto expos = std::get<Bomber::Pos *>(explosion->comps);
					for (auto entity = others.begin(); entity != others.end(); entity++) {
						auto epos = std::get<Bomber::Pos *>(entity->comps);
						if (explosion->id == entity->id || epos->x != expos->x || epos->y != expos->y)
							continue;
						if (reg.has<Bomber::Stat>(entity->id)) {
							reg.get<Bomber::Stat>(entity->id).life -= 1;
							if (reg.get<Bomber::Stat>(entity->id).life <= 0)
								reg.get<Bomber::Graphics>(entity->id).state = Bomber::State::DELETE;
						} else if (reg.has<Bomber::Bomber_types>(entity->id)) {
							killBox(reg, *epos);
							reg.get<Bomber::Graphics>(entity->id).state = Bomber::State::DELETE;
						}
						break;
					}
				}
			}

			void	killBox(GEcm::Register &reg, Bomber::Pos pos) {
				std::uniform_int_distribution<>	distrib;
				int	chance = distrib(_rand) % 100;
				if (chance <= 50)
					return;
				else if (chance <= 70)
					createBonus(reg, pos, Bomber::Bomber_bonus::MORE_BOMB, Bomber::MORE_BOMB_BONUS_T);
				else if (chance <= 90)
					createBonus(reg, pos, Bomber::Bomber_bonus::BIG_EXPLOSION, Bomber::BIG_EXPL_BONUS_T);
				else if (chance < 100)
					createBonus(reg, pos, Bomber::Bomber_bonus::LIFE, Bomber::LIFE_BONUS_T);
			}

			void	createBonus(GEcm::Register &reg, Bomber::Pos pos, Bomber::Bomber_bonus b, const std::string &texture) {
				GEcm::entity_type	bonus = reg.create();
				reg.construct<Bomber::Pos>(bonus, pos);
				reg.construct<Bomber::Collision>(bonus, Bomber::Collision::GET);
				reg.construct<Bomber::Bomber_bonus>(bonus, b);
				reg.construct<Bomber::Graphics>(bonus, true, texture, Bomber::ItemType::SPHERE, Bomber::State::NEW, 0.35);
			}

			/* Data */
			std::default_random_engine	_rand;
	}; // ExplosionSys

} // Bomber