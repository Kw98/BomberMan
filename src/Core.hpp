#pragma once

#include "GLoop.hpp"

namespace Bomber {

	class Core {
		public:
			Core(/* args */) {}
			~Core() {}

			void	run() {}
		private:
			gloop::GLoop	_gloop;
			/* data */
	}; // Core

} // Bomber