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

#include "../engine/TimedSerialCompositeEngine.h"
#include "HeaderController.h"

using namespace Rocket::Core;
using namespace simplicity::rocket;
using namespace std;

namespace simplicity
{
	namespace editor
	{
		HeaderController::HeaderController() :
				headerUi(nullptr)
		{
		}

		void HeaderController::execute(Entity& entity)
		{
			unsigned int fps =
					static_cast<TimedSerialCompositeEngine*>(Simplicity::getCompositeEngine())->getFramesPerSecond();

			Element* fpsElement = headerUi->getDocument()->GetElementById("fps");

			stringstream stream;
			stream << fps;
			dynamic_cast<ElementText*>(fpsElement->GetFirstChild())->SetText(stream.str().c_str());

			/* Code that creates new elements
			 *
					Element* fpsDiv = Factory::InstanceElement(divs[0], "div", "div", XMLAttributes());
					divs[0]->AppendChild(fpsDiv);
					stream << "FPS: " << debugEngine->getFramesPerSecond();
					fpsDiv->AppendChild(getDocument()->CreateTextNode(stream.str().c_str()));

					Element* frameTimeDiv = Factory::InstanceElement(divs[0], "div", "div", XMLAttributes());
					divs[0]->AppendChild(frameTimeDiv);
					stream.str("");
					stream.clear();
					stream << "Frame Time: " << debugEngine->getFrameTime();
					frameTimeDiv->AppendChild(getDocument()->CreateTextNode(stream.str().c_str()));

					for (unsigned int index = 0; index < debugEngine->getEngines().size(); index++)
					{
						Element* engineFrameTimeDiv = Factory::InstanceElement(divs[0], "div", "div", XMLAttributes());
						divs[0]->AppendChild(engineFrameTimeDiv);
						stream.str("");
						stream.clear();
						stream << "Engine[" << index << "] Frame Time: " << debugEngine->getEngineFrameTimes()[index];
						engineFrameTimeDiv->AppendChild(getDocument()->CreateTextNode(stream.str().c_str()));
					}*/
		}

		void HeaderController::onAddEntity(Entity& entity)
		{
			headerUi = entity.getComponent<RocketDocument>();
		}
	}
}
