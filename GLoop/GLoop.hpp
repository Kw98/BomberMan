#pragma once

#include <algorithm>
//#include "ThreadPool.hpp"
#include "Loader.hpp"
#include "StageManager.hpp"

#define LOADED_FUNCTION	("HooksRegister")
using fcnt = void	(*)(GEcm::Register &, gloop::StageManager &);

namespace gloop {

	class GLoop {
		public:
			GLoop() = default;
			~GLoop() = default;

			void	run(GEcm::Register &reg);

			void	load_system(const std::string &libname, GEcm::Register &reg) {
				tools::load_s<fcnt>	loaded = _loader.load<fcnt>(libname, LOADED_FUNCTION);
				loaded.function(reg, _stageM);
			}

			void	unload_system(const std::string &libname) {
				_loader.unload(libname);
			}

			gloop::StageManager	&get_stage_manager() noexcept { return _stageM; }

		private:
			gloop::HookStatus		run_hooks(gloop::Stage &, GEcm::Register &);
			gloop::HookStatus		run_loop_hooks(gloop::Stage &, GEcm::Register &);

			gloop::HookStatus		run_one_hook(gloop::Stage::hookMap &, GEcm::Register &);
			gloop::HookStatus		run_one_loop_hook(gloop::Stage::hookMap &, GEcm::Register &);
			/* data */
			gloop::StageManager		_stageM;
			tools::Loader			_loader;
	};
};