#pragma once

#include <random>
#include "Components.hpp"
#include "GLoop.hpp"

namespace Bomber {

	class MapSys {
		public:
			MapSys(gloop::GLoop &);
			~MapSys() {}

		private:
			void	map_generator(GEcm::Register &);

			void	generate_wall_ground(GEcm::Register &);
			void	generate_box(GEcm::Register &);
			/* Data */
			std::default_random_engine	_rand;

	}; // MapSys

} // Bomber