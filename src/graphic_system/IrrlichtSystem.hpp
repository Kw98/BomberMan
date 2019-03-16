#pragma once

#include "GLoop.hpp"
#include "Components.hpp"
#include <iostream>
#include <irrlicht/irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifndef SCREEN_SIZE
#define WIDTH 1280
#define HEIGHT 920
#endif

class IrrlichtSystem
{
public:
	IrrlichtSystem(gloop::GLoop &gloop) {
		_device = createDevice( video::EDT_OPENGL, dimension2d<u32>(WIDTH, HEIGHT), 16, false, false, false, 0);
		_device->setWindowCaption(L"Bomberman !");
		_driver = _device->getVideoDriver();
		_smgr = _device->getSceneManager();
		_guienv = _device->getGUIEnvironment();
		initIrrlicht(gloop);

		setCamera({Bomber::MAX_X / 2, (Bomber::MAX_Y / 2) - 10, 20.0}, {Bomber::MAX_X / 2, Bomber::MAX_Y / 2, 0.0});
		displayIrrlicht(gloop);
	};

	~IrrlichtSystem() {
		_device->drop();
	};

private:

	void initIrrlicht(gloop::GLoop &gloop) {
		auto &stageManager = gloop.get_stage_manager();
		auto &stage = stageManager.get_stage(gloop::StageType::INIT);
		gloop::SystemHook info = { "initIrrlicht", 1, 1, true,
			[this](GEcm::Register &reg, gloop::GLoop &gloop) {
				gloop.get_stage_manager(); // useless line
				reg.size(); //useless line

				auto entityLists = reg.global_view<Bomber::Graphics, Bomber::Pos>();
				for (auto entity = entityLists.begin(); entity != entityLists.end(); entity++) {
					auto &entityGraphics = std::get<Bomber::Graphics *>(entity->comps);
					auto &entityPos = std::get<Bomber::Pos *>(entity->comps);

					if (entityGraphics->isVisible) {
						createCube({static_cast<double>(entityPos->x), static_cast<double>(entityPos->y), static_cast<double>(entityPos->z)}, entityGraphics->texture, entity->id);
					}

				}
				//check si is visible est tru => create cube ou sphere si true sinon pass
				return gloop::HookStatus::OK;
			}
		};
		stage.add_hook(gloop::HookType::LAST, info);
	}

	irr::scene::ISceneNode	*createCube(const vec3df &pos, const std::string &path, irr::s32 id)
	{
		irr::scene::ISceneNode *n = _smgr->addCubeSceneNode(1);

		if (n) {
			n->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z));
			n->setMaterialTexture(0, _driver->getTexture(path.c_str()));
			n->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			n->setID(id);
		}
		return (n);
	}

	void	setCamera(const vec3df &pos, const vec3df &target)
	{
		irr::scene::ICameraSceneNode *cam= _smgr->addCameraSceneNode();
		cam->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z));
		cam->setTarget(irr::core::vector3df(target.x, target.y, target.z));
		cam->setID(1);
	}

	void displayIrrlicht(gloop::GLoop &gloop) {
		auto &stageManager = gloop.get_stage_manager();
		auto &stage = stageManager.get_stage(gloop::StageType::LOOP);
		gloop::SystemHook info = { "displayIrrlicht", 1, 1, true,
			[this](GEcm::Register &reg, gloop::GLoop &gloop) {
				gloop.get_stage_manager(); // useless line
				reg.size(); //useless line

				if (_device->run()) {
					_driver->beginScene(true, true, SColor(255, 100, 101, 140));
					_smgr->drawAll();
					_guienv->drawAll();
					_driver->endScene();
				}
				return gloop::HookStatus::OK;
			}
		};
		stage.add_hook(gloop::HookType::LAST, info);
	}

	void detectKeyManager(gloop::GLoop &gloop) {
		auto &stageManager = gloop.get_stage_manager();
		auto &stage = stageManager.get_stage(gloop::StageType::LOOP);
		gloop::SystemHook info = { "detectKeyManager", 5, 1, true,
			[this](GEcm::Register &reg, gloop::GLoop &gloop) {
				reg.size(); //useless line
				gloop.get_stage_manager(); // useless line

				return gloop::HookStatus::OK;
			}
		};
		stage.add_hook(gloop::HookType::MIDDLE, info);
	}

private:
	IrrlichtDevice *_device;
	IVideoDriver* _driver;
	ISceneManager* _smgr;
	IGUIEnvironment* _guienv;
};