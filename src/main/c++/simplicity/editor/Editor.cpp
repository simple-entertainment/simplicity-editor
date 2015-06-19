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
#include <simplicity/messaging/Subject.h>
#include <simplicity/rendering/Camera.h>
#include <simplicity/rendering/RenderingEngine.h>
#include <simplicity/resources/Resources.h>
#include <simplicity/Simplicity.h>
#include <simplicity/windowing/WindowEngine.h>

#include <simplicity/rocket/ui/RocketDocument.h>
#include <simplicity/rocket/ui/RocketEngine.h>
#include <simplicity/rocket/ui/RocketFontFace.h>

#include "Editor.h"
#include "engine/TimedSerialCompositeEngine.h"
#include "ui/HeaderController.h"
#include "GodCameraController.h"

using namespace simplicity::rocket;
using namespace std;

namespace simplicity
{
	namespace editor
	{
		namespace Editor
		{
			void augmentSimplicity();
			void fly();
			bool onKeyboardButton(const Message& message);

			bool flying = false;
			unique_ptr<Entity> godCamera(new Entity);

			void augmentSimplicity(unique_ptr<Renderer> renderer)
			{
				unique_ptr<Engine> uiEngine(new RocketEngine(move(renderer), Category::UNCATEGORIZED));
				Simplicity::addEngine(move(uiEngine));

				unique_ptr<Entity> header(new Entity);
				Resource* headerResource = Resources::get("src/main/rml/header.rml", Category::UNCATEGORIZED);
				unique_ptr<Component> headerUi(new RocketDocument(*headerResource));
				header->addUniqueComponent(move(headerUi));
				Resource* consoleFontResource = Resources::get("src/main/resources/fonts/Ubuntu-Regular.ttf",
															   Category::UNCATEGORIZED);
				unique_ptr<Component> headerFont(new RocketFontFace(*consoleFontResource));
				header->addUniqueComponent(move(headerFont));
				unique_ptr<Component> headerController(new HeaderController);
				header->addUniqueComponent(move(headerController));
				Simplicity::getScene()->addEntity(move(header));

				Messages::registerRecipient(Subject::KEYBOARD_BUTTON, onKeyboardButton);
			}

			void fly()
			{
				Entity* currentCamera = Simplicity::getEngine<RenderingEngine>()->getCamera();
				Camera* currentCameraCamera = currentCamera->getComponent<Camera>();
				Camera* godCameraCamera = godCamera->getComponent<Camera>();

				godCameraCamera->setProjection(currentCameraCamera->getProjection());
				godCameraCamera->setTransform(currentCameraCamera->getTransform());
				godCamera->setTransform(currentCamera->getTransform());

				Simplicity::getEngine<RenderingEngine>()->setCamera(godCamera.get());

				godCamera->getComponent<GodCameraController>()->onResumeScene(*Simplicity::getScene(), *godCamera);

				flying = true;
				while (flying)
				{
					Simplicity::getEngine<WindowEngine>()->advance();
					godCamera->getComponent<GodCameraController>()->execute(*godCamera);
					Simplicity::getEngine<RenderingEngine>()->advance();
				}

				godCamera->getComponent<GodCameraController>()->onPauseScene(*Simplicity::getScene(), *godCamera);

				// Revert to the camera being used by the game.
				Simplicity::getEngine<RenderingEngine>()->setCamera(currentCamera);
			}

			bool onKeyboardButton(const Message& message)
			{
				const KeyboardButtonEvent* event = static_cast<const KeyboardButtonEvent*>(message.body);
				if (event->button == Keyboard::Button::GRAVE && event->buttonState == Button::State::UP)
				{
					WindowEngine* windowEngine = Simplicity::getEngine<WindowEngine>();
					if (flying)
					{
						flying = false;
						if (Simplicity::getScene()->capturesMouse())
						{
							windowEngine->captureMouse();
						}
					}
					else
					{
						Simplicity::pause();
						if (Simplicity::getScene()->capturesMouse())
						{
							windowEngine->releaseMouse();
						}
					}
				}

				return false;
			}

			void run(unique_ptr<Renderer> renderer)
			{
				augmentSimplicity(move(renderer));

				while (true)
				{
					Simplicity::play();
					fly();
				}
			}

			void setup()
			{
				unique_ptr<CompositeEngine> compositeEngine(new TimedSerialCompositeEngine);
				Simplicity::setCompositeEngine(move(compositeEngine));

				unique_ptr<Component> godCameraCamera(new Camera);
				godCamera->addUniqueComponent(move(godCameraCamera));
				unique_ptr<Component> godCameraController(new GodCameraController);
				godCamera->addUniqueComponent(move(godCameraController));
			}
		}
	}
}
