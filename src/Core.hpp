#pragma once

#include "MapSys.hpp"
#include "GLoop.hpp"
#include "Register.hpp"
#include "IrrlichtSystem.hpp"

namespace Bomber {

	class Core {
		public:
			Core(/* args */) : _reg(100, 50), _irrlichtSystem(_gloop) {

			}
			~Core() {}

			void	run() {
				Bomber::MapSys	map(_gloop);

				_gloop.run(_reg);
			}
		private:
			gloop::GLoop	_gloop;
			GEcm::Register	_reg;
			IrrlichtSystem	_irrlichtSystem;
			/* data */
	}; // Core

} // Bomber