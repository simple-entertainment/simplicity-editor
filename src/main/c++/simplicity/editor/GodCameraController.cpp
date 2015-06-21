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
#include <functional>

#include <simplicity/input/KeyboardButtonEvent.h>
#include <simplicity/input/MouseButtonEvent.h>
#include <simplicity/input/MouseMoveEvent.h>
#include <simplicity/math/MathFunctions.h>
#include <simplicity/messaging/Messages.h>
#include <simplicity/messaging/Subject.h>
#include <simplicity/Simplicity.h>

#include "GodCameraController.h"

using namespace std;

namespace simplicity
{
	namespace editor
	{
		GodCameraController::GodCameraController() :
				buttonStates(),
				mouseCurrentX(-1),
				mouseCurrentY(-1),
				mouseNewX(-1),
				mouseNewY(-1),
				rotating(false),
				rotationX(0.0f),
				rotationY(0.0f)
		{
		}

		void GodCameraController::execute(Entity& entity)
		{
			// Reset rotation.
			Vector3 position = getPosition3(entity.getTransform());
			entity.getTransform().setIdentity();
			setPosition(entity.getTransform(), position);

			// Apply Y axis rotation so that forward is the correct direction.
			rotate(entity.getTransform(), rotationY, Vector4(0.0f, 1.0f, 0.0f, 1.0f));

			float translationDelta = Simplicity::getDeltaTime() * 100.0f;
			if (buttonStates[Keyboard::Button::LEFT_SHIFT] == Button::State::DOWN)
			{
				translationDelta *= 10.0f;
			}

			if (buttonStates[Keyboard::Button::W] == Button::State::DOWN)
			{
				translate(entity.getTransform(), Vector4(0.0f, 0.0f, -translationDelta, 1.0f));
			}

			if (buttonStates[Keyboard::Button::A] == Button::State::DOWN)
			{
				translate(entity.getTransform(), Vector4(-translationDelta, 0.0f, 0.0f, 1.0f));
			}

			if (buttonStates[Keyboard::Button::S] == Button::State::DOWN)
			{
				translate(entity.getTransform(), Vector4(0.0f, 0.0f, translationDelta, 1.0f));
			}

			if (buttonStates[Keyboard::Button::D] == Button::State::DOWN)
			{
				translate(entity.getTransform(), Vector4(translationDelta, 0.0f, 0.0f, 1.0f));
			}

			if (buttonStates[Keyboard::Button::E] == Button::State::DOWN ||
					buttonStates[Keyboard::Button::SPACE] == Button::State::DOWN)
			{
				translate(entity.getTransform(), Vector4(0.0f, translationDelta, 0.0f, 1.0f));
			}

			if (buttonStates[Keyboard::Button::C] == Button::State::DOWN)
			{
				translate(entity.getTransform(), Vector4(0.0f, -translationDelta, 0.0f, 1.0f));
			}

			if (mouseCurrentX != -1)
			{
				// Reset rotation.
				Vector3 position = getPosition3(entity.getTransform());
				entity.getTransform().setIdentity();
				setPosition(entity.getTransform(), position);

				if (rotating)
				{
					rotationY += (mouseNewX - mouseCurrentX) * -0.01f;
					rotationX += (mouseNewY - mouseCurrentY) * -0.01f;
				}

				// Apply new rotation.
				rotate(entity.getTransform(), rotationY, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
				rotate(entity.getTransform(), rotationX, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
			}

			mouseCurrentX = mouseNewX;
			mouseCurrentY = mouseNewY;
		}

		bool GodCameraController::onKeyboardButton(const Message& message)
		{
			const KeyboardButtonEvent* event = static_cast<const KeyboardButtonEvent*>(message.body);
			buttonStates[event->button] = event->buttonState;

			return false;
		}

		bool GodCameraController::onMouseButton(const Message& message)
		{
			const MouseButtonEvent* event = static_cast<const MouseButtonEvent*>(message.body);
			if (event->button == Mouse::Button::LEFT)
			{
				if (event->buttonState == Button::State::DOWN)
				{
					rotating = true;
				}
				else if (event->buttonState == Button::State::UP)
				{
					rotating = false;
				}
			}

			return false;
		}

		bool GodCameraController::onMouseMove(const Message& message)
		{
			const MouseMoveEvent* event = static_cast<const MouseMoveEvent*>(message.body);

			mouseNewX = event->x;
			mouseNewY = event->y;

			return false;
		}

		void GodCameraController::onResumeScene(Scene& scene, Entity& entity)
		{
			buttonStates[Keyboard::Button::LEFT_SHIFT] = Button::State::UP;
			buttonStates[Keyboard::Button::W] = Button::State::UP;
			buttonStates[Keyboard::Button::A] = Button::State::UP;
			buttonStates[Keyboard::Button::S] = Button::State::UP;
			buttonStates[Keyboard::Button::D] = Button::State::UP;
			buttonStates[Keyboard::Button::E] = Button::State::UP;
			buttonStates[Keyboard::Button::C] = Button::State::UP;
			buttonStates[Keyboard::Button::SPACE] = Button::State::UP;

			Messages::registerRecipient(Subject::KEYBOARD_BUTTON, bind(&GodCameraController::onKeyboardButton, this,
																	   placeholders::_1));
			Messages::registerRecipient(Subject::MOUSE_BUTTON, bind(&GodCameraController::onMouseButton, this,
																	placeholders::_1));
			Messages::registerRecipient(Subject::MOUSE_MOVE, bind(&GodCameraController::onMouseMove, this,
																  placeholders::_1));
		}

		void GodCameraController::onPauseScene(Scene& scene, Entity& entity)
		{
			Messages::deregisterRecipient(Subject::KEYBOARD_BUTTON, bind(&GodCameraController::onKeyboardButton, this,
																		 placeholders::_1));
			Messages::deregisterRecipient(Subject::MOUSE_BUTTON, bind(&GodCameraController::onMouseButton, this,
																	  placeholders::_1));
			Messages::deregisterRecipient(Subject::MOUSE_MOVE, bind(&GodCameraController::onMouseMove, this,
																	placeholders::_1));
		}
	}
}
