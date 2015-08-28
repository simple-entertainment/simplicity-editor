/*
 * Copyright © 2015 Simple Entertainment Limited
 *
 * This file is part of The Simplicity Engine.
 *
 * The Simplicity Engine is free software: you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * The Simplicity Engine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with The Simplicity Engine. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <simplicity/input/KeyboardButtonEvent.h>
#include <simplicity/messaging/Messages.h>
#include <simplicity/messaging/Subject.h>
#include <simplicity/rendering/Camera.h>
#include <simplicity/rendering/RenderingEngine.h>
#include <simplicity/resources/Resources.h>
#include <simplicity/scripting/ScriptingEngine.h>
#include <simplicity/Simplicity.h>
#include <simplicity/windowing/WindowEngine.h>

#include <simplicity/cef/main/CEFBrowser.h>
#include <simplicity/cef/main/CEFEngine.h>

#include "Editor.h"
#include "engine/TimedSerialCompositeEngine.h"
#include "scripting/CameraController.h"
#include "scripting/UIController.h"

using namespace simplicity::simcef;
using namespace std;

namespace simplicity
{
	namespace editor
	{
		namespace Editor
		{
			void editOneFrame();
			void enterEditMode();
			void enterPlayMode();
			void finishEditing();
			bool onKeyboardButton(const Message& message);
			void startEditing();

			Entity* gameCamera = nullptr;
			unique_ptr<Entity> godCamera(new Entity);
			unique_ptr<Engine> scriptEngine = nullptr;
			unique_ptr<Engine> scriptEngineEditMode = nullptr;
			unique_ptr<CEFEngine> uiEngine = nullptr;

			void editOneFrame()
			{
				Simplicity::getEngine<WindowEngine>()->advance();
				Simplicity::getEngine<RenderingEngine>()->advance();
				scriptEngineEditMode->advance();
			}

			void enterEditMode()
			{
				Simplicity::pause();
				Simplicity::finishPlayback();
				startEditing();
			}

			void enterPlayMode()
			{
				finishEditing();
				Simplicity::startPlayback();
			}

			void finishEditing()
			{
				scriptEngineEditMode->onPauseScene(*Simplicity::getScene());

				Simplicity::getEngine<RenderingEngine>()->setCamera(gameCamera);
			}

			bool onKeyboardButton(const Message& message)
			{
				const KeyboardButtonEvent* event = static_cast<const KeyboardButtonEvent*>(message.body);
				if (event->button == Keyboard::Button::GRAVE && event->buttonState == Button::State::UP)
				{
					if (Simplicity::getState() == Simplicity::State::PLAYING)
					{
						enterEditMode();
					}
					else
					{
						enterPlayMode();
					}
				}

				return false;
			}

			void run()
			{
				unique_ptr<Entity> uiEntity = uiEngine->createUIEntity(*Resources::get("src/main/html/ui.html"));
				unique_ptr<Component> uiController(new UIController);
				uiEntity->addUniqueComponent(move(uiController));

				uiEngine->onAddEntity(*uiEntity);
				scriptEngine->onAddEntity(*uiEntity);
				Simplicity::getScene()->addEntity(move(uiEntity));

				unique_ptr<Component> godCameraCamera(new Camera);
				godCamera->addUniqueComponent(move(godCameraCamera));

				unique_ptr<Component> godCameraController(new CameraController);
				godCamera->addUniqueComponent(move(godCameraController));

				uiEngine->onAddEntity(*godCamera);
				scriptEngineEditMode->onAddEntity(*godCamera);

				// Play one frame so the initial scene displays correctly.
				Simplicity::startPlayback();
				Simplicity::playOneFrame();

				enterEditMode();

				while (Simplicity::getState() != Simplicity::State::STOPPING)
				{
					if (Simplicity::getState() == Simplicity::State::PLAYING)
					{
						Simplicity::playOneFrame();
					}
					else
					{
						editOneFrame();
					}

					scriptEngine->advance();
					uiEngine->advance();
				}

				Simplicity::finishPlayback();
			}

			void setup()
			{
				unique_ptr<CompositeEngine> compositeEngine(new TimedSerialCompositeEngine);
				Simplicity::setCompositeEngine(move(compositeEngine));

				uiEngine = unique_ptr<CEFEngine>(new CEFEngine);
				scriptEngine = unique_ptr<Engine>(new ScriptingEngine);
				scriptEngineEditMode = unique_ptr<Engine>(new ScriptingEngine);

				uiEngine->onPlay();
				scriptEngine->onPlay();
				scriptEngineEditMode->onPlay();

				uiEngine->onResumeScene(*Simplicity::getScene());
				scriptEngine->onResumeScene(*Simplicity::getScene());

				Messages::registerRecipient(Subject::KEYBOARD_BUTTON, onKeyboardButton);
			}

			void startEditing()
			{
				gameCamera = Simplicity::getEngine<RenderingEngine>()->getCamera();
				Camera* gameCameraCamera = gameCamera->getComponent<Camera>();
				Camera* godCameraCamera = godCamera->getComponent<Camera>();

				godCameraCamera->setProjection(gameCameraCamera->getProjection());
				godCameraCamera->setTransform(gameCameraCamera->getTransform());
				godCamera->setTransform(gameCamera->getTransform());

				Simplicity::getEngine<RenderingEngine>()->setCamera(godCamera.get());

				scriptEngineEditMode->onResumeScene(*Simplicity::getScene());
			}
		}
	}
}
