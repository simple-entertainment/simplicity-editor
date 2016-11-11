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
#include <sstream>

#include <simplicity/Simplicity.h>
#include <simplicity/math/MathFunctions.h>
#include <simplicity/model/Model.h>
#include <simplicity/rendering/Camera.h>

#include "../engine/TimedSerialCompositeEngine.h"
#include "UIController.h"

using namespace simplicity::simcef;
using namespace std;

namespace simplicity
{
	namespace editor
	{
		UIController::UIController() :
				browser(nullptr)
		{
		}

		void UIController::execute()
		{
			if (Simplicity::getState() != Simplicity::State::PLAYING)
			{
				return;
			}

			TimedSerialCompositeEngine* timedCompositeEngine =
					static_cast<TimedSerialCompositeEngine*>(Simplicity::getCompositeEngine());

			stringstream setEntities;
			setEntities << "setEntities([";
			for (Entity* entity : Simplicity::getScene()->getEntities())
			{
				if (setEntities.str() != "setEntities([")
				{
					setEntities << ", ";
				}

				setEntities << "{";

				setEntities << "'name': ";
				if (entity->getName() == "")
				{
					setEntities << "'Entity " << entity->getId() << "', ";
				}
				else
				{
					setEntities << "'" << entity->getName() << "', ";
				}
				setEntities << "'position': " << getPosition3(entity->getTransform()) << ", ";

				setEntities << "'components': [";
				stringstream components;
				for (Component* component : entity->getComponents<Component>())
				{
					if (components.str() != "")
					{
						components << ", ";
					}

					if (dynamic_cast<Camera*>(component) != nullptr)
					{
						components << "'Camera'";
					}
					if (dynamic_cast<Model*>(component) != nullptr)
					{
						components << "'Model'";
					}
					if (dynamic_cast<Script*>(component) != nullptr)
					{
						components << "'Script'";
					}
				}
				setEntities << components.str() << "]";

				setEntities << "}";
			}
			setEntities << "]);";
			browser->executeJavaScript(setEntities.str());

			stringstream setFps;
			setFps << "setFps(" << timedCompositeEngine->getFramesPerSecond() << ");";
			browser->executeJavaScript(setFps.str());

			stringstream setFrameTimes;
			setFrameTimes << "setFrameTime('total', " << Simplicity::getDeltaTime() << ");";
			setFrameTimes << "setFrameTime('engines', " << timedCompositeEngine->getFrameTime() << ");";
			for (unsigned int index = 0; index < timedCompositeEngine->getEngineFrameTimes().size(); index++)
			{
				setFrameTimes << "setFrameTime('engine" << index << "', " << timedCompositeEngine->getEngineFrameTimes()[index] << ");";
			}
			browser->executeJavaScript(setFrameTimes.str());
		}

		void UIController::onAddEntity()
		{
			browser = getEntity()->getComponent<CEFBrowser>();
		}
	}
}
