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
#include <simplicity/resources/FileSystemDataStore.h>

#include "DataStores.h"

using namespace std;

namespace simplicity
{
	namespace editor
	{
		namespace DataStores
		{
			unique_ptr<DataStore> editorHome;
			unique_ptr<DataStore> projectHome;

			DataStore* getEditorHome()
			{
				return editorHome.get();
			}

			DataStore* getProjectHome()
			{
				return projectHome.get();
			}

			void init(const string& projectLocation)
			{
				editorHome = unique_ptr<DataStore>(new FileSystemDataStore(Resource::Type::ASSET, "."));
				projectHome = unique_ptr<DataStore>(new FileSystemDataStore(Resource::Type::ASSET, projectLocation));
			}
		}
	}
}
