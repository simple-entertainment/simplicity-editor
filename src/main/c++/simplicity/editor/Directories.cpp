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

#include "Directories.h"

using namespace std;

namespace simplicity
{
	namespace editor
	{
		namespace Directories
		{
			string getHere()
			{
				char* cwd = getcwd(nullptr, 0);
				string here = cwd;
				delete[] cwd;

				return here;
			}

			void setHere(const string& here)
			{
				chdir(here.c_str());
			}
		}
	}
}
