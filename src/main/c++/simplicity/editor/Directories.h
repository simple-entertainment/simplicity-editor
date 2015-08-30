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
#ifndef SIMPLICITY_EDITOR_DIRECTORIES_H
#define SIMPLICITY_EDITOR_DIRECTORIES_H

#include <string>

namespace simplicity
{
	namespace editor
	{
		namespace Directories
		{
			std::string getHere();

			void setHere(const std::string& here);
		}
	}
}

#endif //SIMPLICITY_EDITOR_DIRECTORIES_H
