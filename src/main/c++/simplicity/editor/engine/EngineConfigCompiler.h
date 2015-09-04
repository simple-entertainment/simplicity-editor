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
#ifndef SIMPLICITY_EDITOR_ENGINECONFIGCOMPILER_H
#define SIMPLICITY_EDITOR_ENGINECONFIGCOMPILER_H

#include <set>
#include <stack>
#include <vector>

#include <simplicity/resources/Resource.h>

#include <rapidjson/reader.h>

namespace simplicity
{
	namespace editor
	{
		class EngineConfigCompiler
		{
			public:
				EngineConfigCompiler();

				bool Bool(bool b);

				bool Double(double d);

				bool EndArray(rapidjson::SizeType elementCount);

				bool EndObject(rapidjson::SizeType memberCount);

				bool Int(int i);

				bool Int64(int64_t i);

				bool Key(const char* str, rapidjson::SizeType length, bool copy);

				bool Null();

				bool StartArray();

				bool StartObject();

				bool String(const char* str, rapidjson::SizeType length, bool copy);

				bool Uint(unsigned int u);

				bool Uint64(uint64_t u);

				void write(Resource& destination);

			private:
				std::string compiledComment;

				std::string compiledConstructor;

				std::vector<std::string> compiledEngines;

				std::set<std::string> compiledIncludes;

				std::string compiledInitFunction;

				std::vector<std::string> compiledPropertyAssignments;

				unsigned int index;

				std::string key;

				std::vector<float> numbers;

				std::stack<std::string> objects;

				std::string commaSeparatedListFromNumbers();

				std::string setterFromKey();
		};
	}
}

#endif /* SIMPLICITY_EDITOR_ENGINECONFIGCOMPILER_H */
