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
			stringstream stream;
			TimedSerialCompositeEngine* timedEngine =
					static_cast<TimedSerialCompositeEngine*>(Simplicity::getCompositeEngine());

			Element* fpsElement = headerUi->getDocument()->GetElementById("fps");
			stream << timedEngine->getFramesPerSecond();
			dynamic_cast<ElementText*>(fpsElement->GetFirstChild())->SetText(stream.str().c_str());

			Element* timingsElement = headerUi->getDocument()->GetElementById("timings");
			while (timingsElement->GetNumChildren() > 0)
			{
				timingsElement->RemoveChild(timingsElement->GetChild(0));
			}

			if (fpsElement->IsClassSet("selected"))
			{
				for (unsigned int index = 0; index < timedEngine->getEngines().size(); index++)
				{
					Element* engineFrameTimeDiv = Factory::InstanceElement(timingsElement, "div", "div", XMLAttributes());
					timingsElement->AppendChild(engineFrameTimeDiv);
					stream.str("");
					stream.clear();
					stream << "Engine[" << index << "] " << timedEngine->getEngineFrameTimes()[index];
					engineFrameTimeDiv->AppendChild(headerUi->getDocument()->CreateTextNode(stream.str().c_str()));
				}

				Element* frameTimeDiv = Factory::InstanceElement(timingsElement, "div", "div", XMLAttributes());
				timingsElement->AppendChild(frameTimeDiv);
				stream.str("");
				stream.clear();
				stream << "Total: " << timedEngine->getFrameTime();
				frameTimeDiv->AppendChild(headerUi->getDocument()->CreateTextNode(stream.str().c_str()));
			}
		}

		void HeaderController::onAddEntity(Entity& entity)
		{
			headerUi = entity.getComponent<RocketDocument>();

			headerUi->getDocument()->GetElementById("fps")->AddEventListener("click", this);
		}

		void HeaderController::ProcessEvent(Rocket::Core::Event& event)
		{
			Element* targetElement = event.GetTargetElement();
			if (targetElement->GetId() == "fps")
			{
				if (targetElement->IsClassSet("selected"))
				{
					targetElement->SetClass("selected", false);
				}
				else
				{
					targetElement->SetClass("selected", true);
				}
			}
		}
	}
}
