#pragma once

#include "MapSys.hpp"
#include "PlayerSys.hpp"
#include "BombSys.hpp"
#include "GLoop.hpp"
#include "ExplosionSys.hpp"
#include "Register.hpp"
#include "IrrlichtSystem.hpp"
#include "BackgroundSys.hpp"

namespace Bomber {

	class Core {
		public:
			Core(/* args */) : _reg(100, 50), _irrlichtSystem(_gloop) {

			}
			~Core() {}

			void	run() {
				Bomber::MapSys	map(_gloop);
				Bomber::BackgroundSys background(_gloop);
				Bomber::PlayerSys	players(_gloop);
				Bomber::BombSys	bombs(_gloop);
				Bomber::ExplosionSys	explosions(_gloop);

				_gloop.run(_reg);
			}
		private:
			gloop::GLoop	_gloop;
			GEcm::Register	_reg;
			IrrlichtSystem	_irrlichtSystem;
			/* data */
	}; // Core

} // Bomber