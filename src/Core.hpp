#pragma once

#include "MapSys.hpp"
#include "GLoop.hpp"

namespace Bomber {

	class Core {
		public:
			Core(/* args */) {}
			~Core() {}

			void	run() {
				GEcm::Register	reg(100, 50);
				Bomber::MapSys	map(_gloop);

				_gloop.run(reg);
			}
		private:
			gloop::GLoop	_gloop;
			/* data */
	}; // Core

} // Bomber