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
#ifndef HEADERCONTROLLER_H_
#define HEADERCONTROLLER_H_

#include <simplicity/scripting/Script.h>

#include <simplicity/rocket/ui/RocketDocument.h>

namespace simplicity
{
	namespace editor
	{
		class SIMPLE_API HeaderController : public Script, Rocket::Core::EventListener
		{
			public:
				HeaderController();

				void execute(Entity& entity) override;

				void onAddEntity(Entity& entity) override;

				void ProcessEvent(Rocket::Core::Event& event);

			private:
				rocket::RocketDocument* headerUi;
		};
	}
}

#endif /* HEADERCONTROLLER_H_ */
