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
#include <simplicity/Simplicity.h>

#include "Container.h"

using namespace std;

namespace simplicity
{
	namespace editor
	{
		Container::Container(const string& editorHome) :
				editContext(),
				globalContext(editorHome),
				playContext()
		{
		}

		bool Container::onKeyboardButton(const Message& message)
		{
			const KeyboardButtonEvent* event = static_cast<const KeyboardButtonEvent*>(message.body);
			if (event->button == Keyboard::Button::GRAVE && event->buttonState == Button::State::UP)
			{
				switchContext();
			}

			return false;
		}

		void Container::run()
		{
			Messages::registerRecipient(Subject::KEYBOARD_BUTTON, bind(&Container::onKeyboardButton, this,
																	   placeholders::_1));

			// Play one frame so the initial scene displays correctly.
			globalContext.enter();
			playContext.enter();
			playContext.advance();

			switchContext();

			while (Simplicity::getState() != Simplicity::State::STOPPING)
			{
				globalContext.advance();

				if (Simplicity::getState() == Simplicity::State::PLAYING)
				{
					playContext.advance();
				}
				else
				{
					editContext.advance();
				}
			}
		}

		void Container::switchContext()
		{
			if (Simplicity::getState() == Simplicity::State::PLAYING)
			{
				playContext.exit();
				editContext.enter();
			}
			else
			{
				editContext.exit();
				playContext.enter();
			}
		}
	}
}
