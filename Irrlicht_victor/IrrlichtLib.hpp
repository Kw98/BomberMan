//
// Created by Ludovica Pagliarani on 07/05/2018.
//

#ifndef IRRLICHTLIB_HPP
#define IRRLICHTLIB_HPP

#include <iostream>
#include <map>
#ifdef _WIN32
#include <irrlicht.h>
#else
#include <irrlicht/irrlicht.h>
#endif
//#include <EventManager.hpp>
#include <string>
#include <memory>
#include "LibEventManager.hpp"

struct vec3df {
	double	x;
	double	y;
	double	z;
};

struct	vec2d {
	int	x;
	int	y;
};

struct	sphere_t {
	double	diameter;
	bool	rotate;
};

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "irrKlang.lib")
#endif

namespace graphic {
	typedef struct infos_s
	{
		size_t _x;
		size_t _y;
		size_t _w;
		size_t _h;
		size_t _maxW;
		size_t _maxH;
		std::string _path;
		std::string _name;
		std::string _desc;
		graphic::controllerUser _type;
	} infos_t;
	class IrrlichtLib
	{
		public:
			IrrlichtLib();
			~IrrlichtLib();
			irr::video::ITexture	*findTextureOrCreate(const std::string &);
			irr::gui::IGUIImage	*drawImage(const infos_t &);
			irr::IrrlichtDevice	*getDevice() const noexcept { return _device; }
			void			displayAll();
			void			clearGui() noexcept;
			void			clearScene() noexcept;
            void modifyLight(int nbr);
            void setSkinTransparency(irr::s32 alpha, irr::gui::IGUISkin *skin);
            irr::s32 getLight() { return _light; }
			irr::scene::ISceneNode	*createCube(const vec3df &, const std::string &, irr::s32);
			irr::scene::ISceneNode	*createSphere(const vec3df &, const std::string &, irr::s32);
			irr::scene::ISceneNode	*createSphere(const vec3df &, const std::string &, irr::s32, const sphere_t &);
			void			setCamera(irr::scene::ISceneNode *);
			void			setCamera(const vec3df &, const vec3df &t);
			irr::gui::IGUIEditBox	*drawEditBox(const infos_t &);
			irr::gui::IGUIButton	*printButton(const infos_t &);
			void			drawText(size_t, size_t, size_t, std::string const&);
			irr::gui::IGUIScrollBar	*scrollBarButton(const infos_t &);
			std::shared_ptr<LibEventManager> const& getEventManager() const noexcept { return _eventManager; }
			vec2d const		&getScreenSize() const noexcept { return _screenSize; }
			void createListBox(const std::string &name);
		private:
			vec2d						_screenSize;
			irr::video::IVideoDriver			*_driver;
			irr::scene::ISceneManager			*_sceneManager;
			irr::IrrlichtDevice				*_device;
			irr::gui::IGUIEnvironment			*_guiEnv;
			std::map<std::string, irr::video::ITexture *>	_mapTexture;
			std::shared_ptr<graphic::LibEventManager>	_eventManager{nullptr};
            irr::s32 _light;
		    irr::gui::IGUIListBox*    _listBox;
	};
	void driverChoiceConsole(irr::video::E_DRIVER_TYPE &);
}
#endif //IRRLICHTLIB_HPP