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
#ifndef SIMPLICITY_EDITOR_CONTEXT_H
#define SIMPLICITY_EDITOR_CONTEXT_H

namespace simplicity
{
	namespace editor
	{
		class Context
		{
			public:
				virtual void advance() = 0;

				virtual void enter() = 0;

				virtual void exit() = 0;
		};
	}
}

#endif /* SIMPLICITY_EDITOR_CONTEXT_H */
