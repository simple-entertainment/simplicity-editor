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
#include <simplicity/Simplicity.h>

#include "PlayContext.h"

namespace simplicity
{
	namespace editor
	{
		void PlayContext::advance()
		{
			Simplicity::playOneFrame();
		}

		void PlayContext::dispose()
		{
			Simplicity::stop();
			Simplicity::finishPlayback();
		}

		void PlayContext::enter()
		{
			Simplicity::startPlayback();
		}

		void PlayContext::exit()
		{
			Simplicity::pause();
			Simplicity::finishPlayback();
		}

		void PlayContext::init()
		{
		}
	}
}
