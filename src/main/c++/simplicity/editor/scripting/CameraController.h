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
#ifndef CAMERACONTROLLER_H_
#define CAMERACONTROLLER_H_

#include <map>

#include <simplicity/input/Button.h>
#include <simplicity/input/Keyboard.h>
#include <simplicity/messaging/Message.h>
#include <simplicity/scripting/Script.h>

namespace simplicity
{
	namespace editor
	{
		class SIMPLE_API CameraController : public Script
		{
			public:
				CameraController();

				void execute() override;

				void onPauseScene() override;

				void onResumeScene() override;

			private:
				std::map<Keyboard::Button, Button::State> buttonStates;

				int mouseCurrentX;

				int mouseCurrentY;

				int mouseNewX;

				int mouseNewY;

				bool rotating;

				float rotationX;

				float rotationY;

				bool onKeyboardButton(const Message& message);

				bool onMouseButton(const Message& message);

				bool onMouseMove(const Message& message);
		};
	}
}

#endif /* CAMERACONTROLLER_H_ */
