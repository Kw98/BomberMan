#pragma once

#include "Components.hpp"
#include "GLoop.hpp"

namespace Bomber {

	class MapSys {
		public:
			MapSys(gloop::GLoop &);
			~MapSys() {}

		private:
			void	map_generator(GEcm::Register &);

			void	generate_map_without_boxes(GEcm::Register &);
			/* Data */

	}; // MapSys

} // Bomber