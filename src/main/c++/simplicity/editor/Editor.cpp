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
#include <dlfcn.h>

#include <simplicity/logging/Logs.h>
#include <simplicity/resources/FileSystemDataStore.h>
#include <simplicity/resources/Resources.h>
#include <simplicity/Simplicity.h>

#include "engine/EngineConfig.h"
#include "engine/TimedSerialCompositeEngine.h"
#include "entity/EntityConfig.h"
#include "Container.h"
#include "Directories.h"

using namespace simplicity;
using namespace simplicity::editor;
using namespace std;

int main()
{
	unique_ptr<CompositeEngine> compositeEngine(new TimedSerialCompositeEngine);
	Simplicity::setCompositeEngine(move(compositeEngine));

	string projectHome = "/home/simplegsb/Software/bobs-island";
	string editorHome = Directories::getHere();

	Container container(editorHome);

	Directories::setHere(projectHome + "/build");

	EngineConfig::compile(projectHome);
	EntityConfig::compile(projectHome);

	Logs::info("simplicity::editor", "Loading project...");

	char* error = nullptr;

	void* game = dlopen("lib/libbobs-island-module.so", RTLD_NOW);
	error = dlerror();
	if (error != nullptr)
	{
		Logs::error("simplicity::editor", "Failed to load project with error '%s'", error);
		return 1;
	}

	Logs::info("simplicity::editor", "Setting up engine...");

	function<void()> simplicity_generated_setupEngine = (void(*)()) dlsym(game, "simplicity_generated_setupEngine");
	error = dlerror();
	if (error != nullptr)
	{
		Logs::error("simplicity::editor", "Failed to setup engine with error '%s'", error);
		return 2;
	}
	simplicity_generated_setupEngine();

	function<void()> simplicity_setupEngine = (void(*)()) dlsym(game, "simplicity_setupEngine");
	error = dlerror();
	if (error != nullptr)
	{
		Logs::error("simplicity::editor", "Failed to setup engine with error '%s'", error);
		return 2;
	}
	simplicity_setupEngine();

	Logs::info("simplicity::editor", "Setting up scene...");

	function<void()> simplicity_generated_setupScene = (void(*)()) dlsym(game, "simplicity_generated_setupScene");
	error = dlerror();
	if (error != nullptr)
	{
		Logs::error("simplicity::editor", "Failed to setup scene with error '%s'", error);
		return 2;
	}
	simplicity_generated_setupScene();

	function<void()> simplicity_setupScene = (void(*)()) dlsym(game, "simplicity_setupScene");
	error = dlerror();
	if (error != nullptr)
	{
		Logs::error("simplicity::editor", "Failed to setup scene with error '%s'", error);
		return 2;
	}
	simplicity_setupScene();

	Logs::info("simplicity::editor", "Running project in container...");

	container.run();

	dlclose(game);

	return 0;
}
