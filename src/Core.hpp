#pragma once

#include "GLoop.hpp"
#include "Register.hpp"
#include "IrrlichtSystem.hpp"

namespace Bomber {

	class Core {
		public:
			Core(/* args */) :_irrlichtSystem(_gloop) {

			}
			~Core() {}

			void	run() {
				_gloop.run(_reg);
			}
		private:
			gloop::GLoop	_gloop;
			GEcm::Register	_reg;
			IrrlichtSystem	_irrlichtSystem;
			/* data */
	}; // Core

} // Bomber