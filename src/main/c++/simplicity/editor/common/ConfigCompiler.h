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
#ifndef SIMPLICITY_EDITOR_CONFIGPARSER_H
#define SIMPLICITY_EDITOR_CONFIGPARSER_H

#include <stack>
#include <string>
#include <vector>

#include <rapidjson/rapidjson.h>

namespace simplicity
{
	namespace editor
	{
		class ConfigCompiler
		{
			public:
				ConfigCompiler();

				virtual ~ConfigCompiler();

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

			protected:
				std::string compileFloat(float value) const;

				std::string compileMathObject(const std::vector<float>& value) const;

				std::string compileSetter(const std::string& property, const std::string& compiledValue) const;

			private:
				std::stack<std::string> arrays;

				std::vector<bool> booleanArray;

				std::stack<unsigned int> indices;

				bool inLeafArray;

				std::string key;

				std::stack<std::string> levels;

				std::vector<float> numberArray;

				std::vector<std::string> stringArray;

				std::string commaSeparatedListFromNumberArray(const std::vector<float>& value) const;

				virtual void compileBoolean(unsigned int index, const std::string& parent, const std::string& name,
											bool value) = 0;

				virtual void compileBooleanArray(unsigned int index, const std::string& parent, const std::string& name,
												 const std::vector<bool>& value) = 0;

				virtual void compileNumber(unsigned int index, const std::string& parent, const std::string& name,
										   float value) = 0;

				virtual void compileNumberArray(unsigned int index, const std::string& parent, const std::string& name,
												const std::vector<float>& value) = 0;

				virtual void compileObject(unsigned int index, const std::string& parent, const std::string& name) = 0;

				virtual void compileString(unsigned int index, const std::string& parent, const std::string& name,
										   const std::string& value) = 0;

				virtual void compileStringArray(unsigned int index, const std::string& parent, const std::string& name,
												const std::vector<std::string>& value) = 0;
		};
	}
}

#endif /* SIMPLICITY_EDITOR_CONFIGPARSER_H */
