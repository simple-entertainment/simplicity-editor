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

#include "DynamicLibrary.h"

using namespace std;

namespace simplicity
{
	namespace editor
	{
		DynamicLibrary::DynamicLibrary(const Resource& library) :
				handle(nullptr),
				library(library)
		{
			load();
		}

		DynamicLibrary::~DynamicLibrary()
		{
			unload();
		}

		bool DynamicLibrary::callFunction(const string& name)
		{
			if (handle == nullptr)
			{
				Logs::error("simplicity::editor", "Failed to call function, library is not loaded");
				return false;
			}

			function<void()> function = (void(*)()) dlsym(handle, name.c_str());
			char* error = dlerror();
			if (error != nullptr)
			{
				Logs::error("simplicity::editor", "Failed to call function with error '%s'", error);
				return false;
			}

			function();

			return true;
		}

		bool DynamicLibrary::load()
		{
			handle = dlopen(library.getAbsolutePath().c_str(), RTLD_NOW);

			char* error = dlerror();
			if (error != nullptr)
			{
				Logs::error("simplicity::editor", "Failed to load library with error '%s'", error);
				return false;
			}

			return true;
		}

		bool DynamicLibrary::reload()
		{
			unload();

			return load();
		}

		void DynamicLibrary::unload()
		{
			if (handle != nullptr)
			{
				dlclose(handle);
			}
		}
	}
}
