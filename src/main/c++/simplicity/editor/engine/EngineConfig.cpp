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
#include "EngineConfig.h"
#include "EngineConfigCompiler.h"

using namespace rapidjson;
using namespace std;

namespace simplicity
{
	namespace editor
	{
		namespace EngineConfig
		{
			void compile(const std::string& projectHome)
			{
				FileSystemDataStore projectDataStore(Resource::Type::USER, projectHome);

				unique_ptr<istream> istream =
						projectDataStore.get("simplicity.json", false)->getInputStream();
				JSONInputStream inputStream(*istream);
				EngineConfigCompiler compiler;
				Reader().Parse(inputStream, compiler);
				compiler.write(*projectDataStore.get("src/generated/simplicity-generated-engine.cpp", false));
			}
		}
	}
}
