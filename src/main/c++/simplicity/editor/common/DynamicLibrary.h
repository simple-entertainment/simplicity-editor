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
#ifndef SIMPLICITY_EDITOR_DYNAMICLIBRARY_H
#define SIMPLICITY_EDITOR_DYNAMICLIBRARY_H

#include <simplicity/resources/Resource.h>

namespace simplicity
{
	namespace editor
	{
		class DynamicLibrary
		{
			public:
				DynamicLibrary(const Resource& library);

				~DynamicLibrary();

				bool callFunction(const std::string& name);

				bool reload();

			private:
				void* handle;

				const Resource& library;

				bool load();

				void unload();
		};
	}
}

#endif /* SIMPLICITY_EDITOR_DYNAMICLIBRARY_H */
