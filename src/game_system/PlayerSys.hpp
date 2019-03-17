#pragma once

#include <iostream>
#include <unordered_map>
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

				gloop::SystemHook actions = gloop::SystemHook{PLAYER_SYS_NAME, 5, 100, false, [this](GEcm::Register &reg, gloop::GLoop &){
					if (reg.has<Bomber::Stat>(_playerO))
						playerAction(reg, _playerO);
					if (reg.has<Bomber::Stat>(_playerT))
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
					auto key = in->keys.front();
					if (_movements.find(key) != _movements.end())
						movePlayer(std::get<Bomber::Pos *>(view), _movements.at(key), reg, ent);
					else if (key == Bomber::Keys::K_SPACE || key == Bomber::Keys::K_ENTER)
						putDownBomb(std::get<Bomber::Pos *>(view), std::get<Bomber::Stat *>(view), reg, ent);
					while (!in->keys.empty())
						in->keys.pop();
				}
			}

			void	putDownBomb(Bomber::Pos *epos, Bomber::Stat *estats, GEcm::Register &reg, const GEcm::entity_type ent) {
				auto	entities = reg.global_view<Bomber::Bomb>();
				int	nb = 0;
				for (auto entity = entities.begin(); entity != entities.end(); entity++) {
					auto	bomb = std::get<Bomber::Bomb *>(entity->comps);
					if (ent == bomb->parent)
						nb++;
				}
				if (nb >= estats->max_bomb)
					return;
				GEcm::entity_type	bomb = reg.create();
				reg.construct<Bomber::Pos>(bomb, *epos);
				reg.construct<Bomber::Collision>(bomb, Bomber::Collision::PASS);
				reg.construct<Bomber::Graphics>(bomb, false, Bomber::BOMB_T, Bomber::ItemType::SPHERE, Bomber::State::NEW, 0.25);
				reg.construct<Bomber::Bomb>(bomb, ent, estats->explosion_radius, std::chrono::system_clock::now());

			}

			void	movePlayer(Bomber::Pos *epos, Bomber::Pos npos, GEcm::Register &reg, const GEcm::entity_type ent) {
				auto	entities = reg.global_view<Bomber::Collision, Bomber::Pos>();
				GEcm::entity_type	contact = ent;
				npos.x += epos->x;
				npos.y += epos->y;
				for (auto entity = entities.begin(); entity != entities.end(); entity++) {
					auto	pos = std::get<Bomber::Pos *>(entity->comps);
					if (npos.x == pos->x && npos.y == pos->y && ent != entity->id) {
						contact = entity->id;
						break;
					}
				}
				if (contact == ent 
					|| (reg.get<Bomber::Collision>(contact) != Bomber::Collision::BLOCK_ALL 
					&& reg.get<Bomber::Collision>(contact) != Bomber::Collision::BLOCK_PLAYER)) {
					epos->x = npos.x;
					epos->y = npos.y;
					reg.get<Bomber::Graphics>(ent).state = Bomber::State::UPDATE;
					if (reg.get<Bomber::Collision>(contact) == Bomber::Collision::GET)
						getBonus(reg, ent, contact);
				}
				//CHECK IF BONUS HERE
			}

			void	getBonus(GEcm::Register &reg, const GEcm::entity_type player, const GEcm::entity_type bonus) {
				auto b = reg.get<Bomber::Bomber_bonus>(bonus);

				reg.get<Bomber::Graphics>(bonus).state = Bomber::State::DELETE;
				if (b == Bomber::Bomber_bonus::BIG_EXPLOSION)
					reg.get<Bomber::Stat>(player).explosion_radius += 1;
				else if (b == Bomber::Bomber_bonus::MORE_BOMB)
					reg.get<Bomber::Stat>(player).max_bomb += 1;
				else if (b == Bomber::Bomber_bonus::LIFE)
					reg.get<Bomber::Stat>(player).life += 1;
			}

			/* Data */
			GEcm::entity_type	_playerO;
			GEcm::entity_type	_playerT;
			const std::unordered_map<Bomber::Keys, Bomber::Pos>	_movements = {
				{Bomber::Keys::ARROW_D, {0, -1, 0}},
				{Bomber::Keys::ARROW_U, {0, 1, 0}},
				{Bomber::Keys::ARROW_L, {1, 0, 0}},
				{Bomber::Keys::ARROW_R, {-1, 0, 0}},
				{Bomber::Keys::K_D, {-1, 0, 0}},
				{Bomber::Keys::K_Z, {0, 1, 0}},
				{Bomber::Keys::K_Q, {1, 0, 0}},
				{Bomber::Keys::K_S, {0, -1, 0}},
			};

	}; // MapSys

} // Bomber