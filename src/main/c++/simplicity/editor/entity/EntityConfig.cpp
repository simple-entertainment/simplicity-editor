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
#include <rapidjson/reader.h>

#include <simplicity/resources/FileSystemDataStore.h>

#include "../common/JSONInputStream.h"
#include "EntityConfig.h"
#include "EntityConfigCompiler.h"

using namespace rapidjson;
using namespace std;

namespace simplicity
{
	namespace editor
	{
		namespace EntityConfig
		{
			void compile(const std::string& projectHome)
			{
				FileSystemDataStore projectDataStore(Resource::Type::USER, projectHome);

				unique_ptr<istream> istream =
						projectDataStore.get("scene.json", false)->getInputStream();
				JSONInputStream inputStream(*istream);
				EntityConfigCompiler compiler;
				Reader().Parse(inputStream, compiler);
				compiler.write(*projectDataStore.get("src/generated/simplicity-generated-scene.cpp", false));
			}
		}
	}
}
