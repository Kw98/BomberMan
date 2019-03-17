#pragma once

#include <iostream>
#include <irrlicht/irrlicht.h>
#include <unordered_map>
#include "GLoop.hpp"
#include "Components.hpp"

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

static constexpr char	IRRLICHT_SYS_NAME[] = "IRRLICHT_SYS";

class IrrEvents : public irr::IEventReceiver {
	public:

		virtual bool OnEvent(const SEvent& event) {
			if (event.EventType == irr::EET_KEY_INPUT_EVENT)
				KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
			return false;
		}

		virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const {
			return KeyIsDown[keyCode];
		}
		
		IrrEvents() {
			for (u32 i=0; i< irr::KEY_KEY_CODES_COUNT; ++i)
				KeyIsDown[i] = false;
		}

	private:
		bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

class IrrlichtSystem
{
public:
	IrrlichtSystem(gloop::GLoop &gloop) {
		_device = createDevice( video::EDT_OPENGL, dimension2d<u32>(WIDTH, HEIGHT), 16, false, false, false, &_events);
		_device->setWindowCaption(L"Bomberman !");
		_driver = _device->getVideoDriver();
		_smgr = _device->getSceneManager();
		_guienv = _device->getGUIEnvironment();
		initIrrlicht(gloop);

		setCamera({Bomber::MAX_X / 2, (Bomber::MAX_Y / 2) - 10, 20.0}, {Bomber::MAX_X / 2, Bomber::MAX_Y / 2, 0.0});
		displayIrrlicht(gloop);
		detectKeyManager(gloop);
	};

	~IrrlichtSystem() {
		_device->drop();
	};

private:

	void initIrrlicht(gloop::GLoop &gloop) {
		auto &stageManager = gloop.get_stage_manager();
		auto &stage = stageManager.get_stage(gloop::StageType::INIT);
		gloop::SystemHook info = {IRRLICHT_SYS_NAME, 1, 1, true,
			[this](GEcm::Register &reg, gloop::GLoop &) {
				auto entityLists = reg.global_view<Bomber::Graphics, Bomber::Pos>();
				for (auto entity = entityLists.begin(); entity != entityLists.end(); entity++) {
					auto &entityGraphics = std::get<Bomber::Graphics *>(entity->comps);
					auto &entityPos = std::get<Bomber::Pos *>(entity->comps);
					manageEntity(entityGraphics, entityPos, entity->id, reg);
					entityGraphics->state = Bomber::State::NOTHING;
				}
				//check si is visible est tru => create cube ou sphere si true sinon pass
				return gloop::HookStatus::OK;
			}
		};
		stage.add_hook(gloop::HookType::LAST, info);
	}

	void	manageEntity(Bomber::Graphics *egraphic, Bomber::Pos *epos, const GEcm::entity_type ent, GEcm::Register &reg) {
		if (egraphic->state == Bomber::State::NOTHING)
			return;
		if (egraphic->state == Bomber::State::NEW) {
			createObject(egraphic, epos, ent);
		} else if (egraphic->state == Bomber::State::UPDATE) {
			_nodes[ent]->setPosition({static_cast<float>(epos->x), static_cast<float>(epos->y), static_cast<float>(epos->z)});
			_nodes[ent]->setVisible(egraphic->isVisible);
		} else if (egraphic->state == Bomber::State::DELETE)
			_todelete.push_back(ent);
	}

	void	createObject(Bomber::Graphics *egraphic, Bomber::Pos *epos, const GEcm::entity_type ent) {
		if (egraphic->type == Bomber::ItemType::CUBE) {
			_nodes[ent] = createCube({static_cast<double>(epos->x), static_cast<double>(epos->y), static_cast<double>(epos->z)}, egraphic->texture, egraphic->size, ent);
			_nodes[ent]->setVisible(egraphic->isVisible);
		} else if (egraphic->type == Bomber::ItemType::SPHERE) {
			_nodes[ent] = createSphere({static_cast<double>(epos->x), static_cast<double>(epos->y), static_cast<double>(epos->z)}, egraphic->texture, egraphic->size, ent);
			_nodes[ent]->setVisible(egraphic->isVisible);
		} else if (egraphic->type == Bomber::ItemType::MESH) {

		} else if (egraphic->type == Bomber::ItemType::TEXT) {

		} else {

		}
	}

	irr::scene::ISceneNode	*createCube(const vec3df &pos, const std::string &path, const double size, irr::s32 id) {
		irr::scene::ISceneNode *n = _smgr->addCubeSceneNode(size);

		if (n) {
			n->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z));
			n->setMaterialTexture(0, _driver->getTexture(path.c_str()));
			n->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			n->setID(id);
		}
		return n;
	}

	irr::scene::ISceneNode	*createSphere(const vec3df &pos, const std::string &path, const double size, irr::s32 id) {
		irr::scene::ISceneNode *n = _smgr->addSphereSceneNode(size);

		if (n) {
			n->setPosition(irr::core::vector3df(pos.x,pos.y, pos.z));
			n->setMaterialTexture(0, _driver->getTexture(path.c_str()));
			n->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			n->setID(id);
		}
		return n;
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
		gloop::SystemHook info = {IRRLICHT_SYS_NAME, 1, 1, true,
			[this](GEcm::Register &reg, gloop::GLoop &) {

				auto entityLists = reg.global_view<Bomber::Graphics, Bomber::Pos>();
				for (auto entity = entityLists.begin(); entity != entityLists.end(); entity++) {
					auto &entityGraphics = std::get<Bomber::Graphics *>(entity->comps);
					auto &entityPos = std::get<Bomber::Pos *>(entity->comps);
					manageEntity(entityGraphics, entityPos, entity->id, reg);
					entityGraphics->state = Bomber::State::NOTHING;
				}
				for(auto&& elem : _todelete) {
					_smgr->addToDeletionQueue(_nodes[elem]);
					_nodes.erase(elem);
					reg.delete_entity(elem);
				}
				_todelete.clear();
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
		
		gloop::SystemHook info = {IRRLICHT_SYS_NAME, 5, 100, true,
			[this](GEcm::Register &reg, gloop::GLoop &) {
				bool alter = true;
				auto entityList = reg.global_view<Bomber::Inputs>();
				for (auto entity = entityList.begin(); entity != entityList.end(); entity++) {
					auto input = std::get<Bomber::Inputs *>(entity->comps);
					if (alter)
						detectPlayerOne(input);
					else
						detectPlayerTwo(input);
					alter = !alter;
				}
				return gloop::HookStatus::OK;
			}
		};
		stage.add_hook(gloop::HookType::MIDDLE, info);
	}

	void	detectPlayerOne(Bomber::Inputs *input) {
		if (_events.IsKeyDown(irr::KEY_KEY_Z))
			input->keys.push(Bomber::Keys::K_Z);
		if (_events.IsKeyDown(irr::KEY_KEY_S))
			input->keys.push(Bomber::Keys::K_S);
		if (_events.IsKeyDown(irr::KEY_KEY_Q))
			input->keys.push(Bomber::Keys::K_Q);
		if (_events.IsKeyDown(irr::KEY_KEY_D))
			input->keys.push(Bomber::Keys::K_D);
		if (_events.IsKeyDown(irr::KEY_SPACE))
			input->keys.push(Bomber::Keys::K_SPACE);
	}

	void	detectPlayerTwo(Bomber::Inputs *input) {
		if (_events.IsKeyDown(irr::KEY_UP))
			input->keys.push(Bomber::Keys::ARROW_U);
		if (_events.IsKeyDown(irr::KEY_DOWN))
			input->keys.push(Bomber::Keys::ARROW_D);
		if (_events.IsKeyDown(irr::KEY_LEFT))
			input->keys.push(Bomber::Keys::ARROW_L);
		if (_events.IsKeyDown(irr::KEY_RIGHT))
			input->keys.push(Bomber::Keys::ARROW_R);
		if (_events.IsKeyDown(irr::KEY_BACK))
			input->keys.push(Bomber::Keys::K_ENTER);
	}

private:
	IrrEvents	_events;
	IrrlichtDevice *_device;
	IVideoDriver* _driver;
	ISceneManager* _smgr;
	IGUIEnvironment* _guienv;
	std::unordered_map<GEcm::entity_type, irr::scene::ISceneNode *>	_nodes;
	std::vector<GEcm::entity_type>	_todelete;
};