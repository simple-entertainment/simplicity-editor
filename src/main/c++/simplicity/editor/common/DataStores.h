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
#ifndef SIMPLICITY_EDITOR_DATASTORES_H
#define SIMPLICITY_EDITOR_DATASTORES_H

#include <simplicity/resources/DataStore.h>

namespace simplicity
{
	namespace editor
	{
		namespace DataStores
		{
			DataStore* getEditorHome();

			DataStore* getProjectHome();

			void init(const std::string& projectBuildLocation);
		}
	}
}

#endif //SIMPLICITY_EDITOR_DATASTORES_H
