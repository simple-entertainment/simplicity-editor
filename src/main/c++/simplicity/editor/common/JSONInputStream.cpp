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
#include "JSONInputStream.h"

using namespace std;

namespace simplicity
{
	namespace editor
	{
		JSONInputStream::JSONInputStream(istream& inputStream) :
				inputStream(inputStream)
		{
		}

		void JSONInputStream::Flush()
		{
		}

		JSONInputStream::Ch JSONInputStream::Peek() const
		{
			unsigned long c = inputStream.peek();

			if (c == char_traits<Ch>::eof())
			{
				return '\0';
			}

			return static_cast<Ch>(c);
		}

		void JSONInputStream::Put(Ch c)
		{
		}

		JSONInputStream::Ch* JSONInputStream::PutBegin()
		{
			return 0;
		}

		size_t JSONInputStream::PutEnd(Ch* c)
		{
			return 0;
		}

		size_t JSONInputStream::Tell() const
		{
			return static_cast<size_t>(inputStream.tellg());
		}

		JSONInputStream::Ch JSONInputStream::Take()
		{
			unsigned long c = inputStream.get();

			if (c == char_traits<Ch>::eof())
			{
				return '\0';
			}

			return static_cast<Ch>(c);
		}
	}
}
