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
#include <sstream>

#include "ConfigCompiler.h"

using namespace rapidjson;
using namespace std;

namespace simplicity
{
	namespace editor
	{
		ConfigCompiler::ConfigCompiler() :
				arrays(),
				booleanArray(),
				indices(),
				inLeafArray(false),
				key(),
				numberArray(),
				levels(),
				stringArray()
		{
		}

		ConfigCompiler::~ConfigCompiler()
		{
		}

		bool ConfigCompiler::Bool(bool b)
		{
			if (inLeafArray)
			{
				booleanArray.push_back(b);
			}
			else
			{
				compileBoolean(indices.top(), levels.top(), key, b);
				key = "";
			}

			return true;
		}

		string ConfigCompiler::commaSeparatedListFromNumberArray(const std::vector<float>& value) const
		{
			stringstream stream;

			for (unsigned int index = 0; index < value.size(); index++)
			{
				stream << compileFloat(value[index]);
				if (index < value.size() - 1)
				{
					stream << ", ";
				}
			}

			return stream.str();
		}

		string ConfigCompiler::compileFloat(float value) const
		{
			stringstream stream;
			stream << value;
			if (stream.str().find('.') == string::npos)
			{
				stream << ".0";
			}
			stream << "f";

			return stream.str();
		}

		string ConfigCompiler::compileMathObject(const vector<float>& value) const
		{
			string className = "";
			if (numberArray.size() == 3)
			{
				className = "sim::Vector3";
			}
			else if (numberArray.size() == 4)
			{
				className = "sim::Vector4";
			}
			else if (numberArray.size() == 9)
			{
				className = "sim::Matrix33";
			}
			else if (numberArray.size() == 16)
			{
				className = "sim::Matrix44";
			}

			if (!className.empty())
			{
				return className + "(" + commaSeparatedListFromNumberArray(value) + ")";
			}

			return "!error compiling math object: requires 3, 4, 9 or 16 elements!";
		}

		string ConfigCompiler::compileSetter(const string& property, const string& compiledValue) const
		{
			string capitalProperty = property;
			capitalProperty[0] = static_cast<char>(toupper(capitalProperty[0]));
			return string("set") + capitalProperty + "(" + compiledValue + ")";
		}

		bool ConfigCompiler::Double(double d)
		{
			if (inLeafArray)
			{
				numberArray.push_back(static_cast<float>(d));
			}
			else
			{
				compileNumber(indices.top(), levels.top(), key, static_cast<float>(d));
				key = "";
			}

			return true;
		}

		bool ConfigCompiler::EndArray(SizeType elementCount)
		{
			string name = "";
			if (!arrays.empty() && arrays.top() == levels.top())
			{
				name = levels.top();
			}

			if (!name.empty())
			{
				arrays.pop();
				indices.pop();
				levels.pop();
			}

			if (inLeafArray)
			{
				if (!booleanArray.empty())
				{
					compileBooleanArray(indices.top(), levels.top(), name, booleanArray);
				}
				else if (!numberArray.empty())
				{
					compileNumberArray(indices.top(), levels.top(), name, numberArray);
				}
				else if (!stringArray.empty())
				{
					compileStringArray(indices.top(), levels.top(), name, stringArray);
				}

				booleanArray.clear();
				numberArray.clear();
				stringArray.clear();
				inLeafArray = false;
			}

			return true;
		}

		bool ConfigCompiler::EndObject(SizeType memberCount)
		{
			if (levels.size() == 0)
			{
				return true;
			}

			string name = "";
			if (arrays.size() == 0 || arrays.top() != levels.top())
			{
				name = levels.top();
			}

			if (!name.empty())
			{
				levels.pop();
			}

			compileObject(indices.top(), levels.top(), name);

			// If this is an anonymous object it must be in an array.
			if (name.empty())
			{
				indices.top()++;
			}

			return true;
		}

		bool ConfigCompiler::Int(int i)
		{
			if (inLeafArray)
			{
				numberArray.push_back(i);
			}
			else
			{
				compileNumber(indices.top(), levels.top(), key, i);
				key = "";
			}

			return true;
		}

		bool ConfigCompiler::Int64(int64_t i)
		{
			if (inLeafArray)
			{
				numberArray.push_back(i);
			}
			else
			{
				compileNumber(indices.top(), levels.top(), key, i);
				key = "";
			}

			return true;
		}

		bool ConfigCompiler::Key(const char* str, SizeType length, bool copy)
		{
			key = str;

			return true;
		}

		bool ConfigCompiler::Null()
		{
			return true;
		}

		bool ConfigCompiler::StartArray()
		{
			if (!key.empty())
			{
				arrays.push(key);
				indices.push(0);
				levels.push(key);
				key = "";
			}

			inLeafArray = true;

			return true;
		}

		bool ConfigCompiler::StartObject()
		{
			if (!key.empty())
			{
				levels.push(key);
				key = "";
			}

			inLeafArray = false;

			return true;
		}

		bool ConfigCompiler::String(const char* str, SizeType length, bool copy)
		{
			if (inLeafArray)
			{
				stringArray.push_back(str);
			}
			else
			{
				compileString(indices.top(), levels.top(), key, str);
				key = "";
			}

			return true;
		}

		bool ConfigCompiler::Uint(unsigned int u)
		{
			if (inLeafArray)
			{
				numberArray.push_back(u);
			}
			else
			{
				compileNumber(indices.top(), levels.top(), key, u);
				key = "";
			}

			return true;
		}

		bool ConfigCompiler::Uint64(uint64_t u)
		{
			if (inLeafArray)
			{
				numberArray.push_back(u);
			}
			else
			{
				compileNumber(indices.top(), levels.top(), key, u);
				key = "";
			}

			return true;
		}
	}
}
