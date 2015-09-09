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
#include <unistd.h>

#include <simplicity/logging/Logs.h>
#include <simplicity/resources/FileSystemDataStore.h>
#include <simplicity/resources/Resources.h>
#include <simplicity/Simplicity.h>

#include "common/DataStores.h"
#include "common/DynamicLibrary.h"
#include "engine/EngineConfig.h"
#include "engine/TimedSerialCompositeEngine.h"
#include "entity/EntityConfig.h"
#include "Container.h"

using namespace simplicity;
using namespace simplicity::editor;
using namespace std;

int main()
{
	unique_ptr<CompositeEngine> compositeEngine(new TimedSerialCompositeEngine);
	Simplicity::setCompositeEngine(move(compositeEngine));

	Container container;

	string projectLocation = "/home/simplegsb/Software/bobs-island";
	DataStores::init(projectLocation);
	chdir((projectLocation + "/build").c_str());

	EngineConfig::compile(projectLocation);
	EntityConfig::compile(projectLocation);

	Logs::info("simplicity::editor", "Loading project...");
	DynamicLibrary project(*DataStores::getProjectHome()->get("build/lib/libbobs-island-module.so"));

	Logs::info("simplicity::editor", "Setting up engine...");
	project.callFunction("simplicity_generated_setupEngine");
	project.callFunction("simplicity_setupEngine");

	Logs::info("simplicity::editor", "Setting up scene...");
	project.callFunction("simplicity_generated_setupScene");
	project.callFunction("simplicity_setupScene");

	Logs::info("simplicity::editor", "Running project in container...");
	container.run();

	return 0;
}
