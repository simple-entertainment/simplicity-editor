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

#include "engine/TimedSerialCompositeEngine.h"
#include "Container.h"
#include "Directories.h"

using namespace simplicity;
using namespace simplicity::editor;
using namespace std;

int main()
{
	unique_ptr<CompositeEngine> compositeEngine(new TimedSerialCompositeEngine);
	Simplicity::setCompositeEngine(move(compositeEngine));

	string projectHome = "/home/simplegsb/Software/bobs-island/build";
	string editorHome = Directories::getHere();

	Container container(editorHome);

	Directories::setHere(projectHome);

	Logs::info("simplicity::editor", "Loading project code...");

	char* error = nullptr;

	void* game = dlopen("lib/libbobs-island-server-for-editor.so", RTLD_NOW);
	error = dlerror();
	if (error != nullptr)
	{
		Logs::error("simplicity::editor", "Failed to load project code with error '%s'", error);
		return 1;
	}

	Logs::info("simplicity::editor", "Setting up engine...");

	function<void()> setupEngine = (void(*)()) dlsym(game, "setupEngine");
	error = dlerror();
	if (error != nullptr)
	{
		Logs::error("simplicity::editor", "Failed to setup engine with error '%s'", error);
		return 2;
	}
	setupEngine();

	Logs::info("simplicity::editor", "Setting up scene...");

	function<void()> setupScene = (void(*)()) dlsym(game, "setupScene");
	error = dlerror();
	if (error != nullptr)
	{
		Logs::error("simplicity::editor", "Failed to setup scene with error '%s'", error);
		return 2;
	}
	setupScene();

	Logs::info("simplicity::editor", "Running project in container...");

	container.run();

	dlclose(game);

	return 0;
}
