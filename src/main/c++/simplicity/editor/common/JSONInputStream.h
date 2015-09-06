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
#ifndef SIMPLICITY_EDITOR_JSONINPUTSTREAM_H
#define SIMPLICITY_EDITOR_JSONINPUTSTREAM_H

#include <istream>

namespace simplicity
{
	namespace editor
	{
		class JSONInputStream
		{
			public:
				typedef char Ch;

				JSONInputStream(std::istream& inputStream);

				void Flush();

				Ch Peek() const;

				void Put(Ch c);

				Ch* PutBegin();

				size_t PutEnd(Ch* c);

				Ch Take();

				size_t Tell() const;

			private:
				std::istream& inputStream;
		};
	}
}

#endif /* SIMPLICITY_EDITOR_JSONINPUTSTREAM_H */
