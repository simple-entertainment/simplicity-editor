/*      _                 _ _      _ _
 *     (_)               | (_)    (_) |
 *  ___ _ _ __ ___  _ __ | |_  ___ _| |_ _   _
 * / __| | '_ ` _ \| '_ \| | |/ __| | __| | | |
 * \__ \ | | | | | | |_) | | | (__| | |_| |_| |
 * |___/_|_| |_| |_| .__/|_|_|\___|_|\__|\__, |
 *                 | |                    __/ |
 *                 |_|                   |___/
 *
 * This file is part of simplicity. See the LICENSE file for the full license governing this code.
 */
#ifndef SIMPLICITY_EDITOR_EDITCONTEXT_H
#define SIMPLICITY_EDITOR_EDITCONTEXT_H

#include <simplicity/engine/SerialCompositeEngine.h>
#include <simplicity/entity/Entity.h>

#include "Context.h"

namespace simplicity
{
	namespace editor
	{
		class EditContext : public Context
		{
			public:
				EditContext();

				~EditContext();

				void advance();

				void enter();

				void exit();

			private:
				SerialCompositeEngine compositeEngine;

				std::unique_ptr<Entity> editCameraEntity;

				Entity* gameCameraEntity;

				bool initialized;
		};
	}
}

#endif /* SIMPLICITY_EDITOR_EDITCONTEXT_H */
