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
#ifndef SIMPLICITY_EDITOR_GLOBALCONTEXT_H
#define SIMPLICITY_EDITOR_GLOBALCONTEXT_H

#include <simplicity/engine/SerialCompositeEngine.h>
#include <simplicity/resources/FileSystemDataStore.h>

#include <simplicity/cef/main/CEFEngine.h>

#include "Context.h"

namespace simplicity
{
	namespace editor
	{
		class GlobalContext : public Context
		{
			public:
				GlobalContext(const std::string& editorHome);

				void advance();

				void dispose();

				void enter();

				void exit();

				void init();

			private:
				SerialCompositeEngine compositeEngine;

				FileSystemDataStore uiDataStore;

				simcef::CEFEngine* uiEngine;

				std::unique_ptr<Entity> uiEntity;
		};
	}
};

#endif /* SIMPLICITY_EDITOR_GLOBALCONTEXT_H */
