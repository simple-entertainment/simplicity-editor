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

#include <simplicity/resources/Resource.h>

#include "../common/ConfigCompiler.h"

namespace simplicity
{
	namespace editor
	{
		class EngineConfigCompiler : public ConfigCompiler
		{
			public:
				EngineConfigCompiler();

				void write(Resource& destination);

			private:
				std::string compiledComment;

				std::string compiledConstructor;

				std::vector<std::string> compiledEngines;

				std::vector<std::string> compiledFactories;

				std::set<std::string> compiledIncludes;

				std::string compiledInitFunction;

				std::vector<std::string> compiledProperties;

				std::string interface;

				std::string type;

				void compileBoolean(unsigned int index, const std::string& parent, const std::string& name,
										bool value) override;

				void compileBooleanArray(unsigned int index, const std::string& parent, const std::string& name,
											 const std::vector<bool>& value) override;

				void compileNumber(unsigned int index, const std::string& parent, const std::string& name,
									   float value) override;

				void compileNumberArray(unsigned int index, const std::string& parent, const std::string& name,
											const std::vector<float>& value) override;

				void compileObject(unsigned int index, const std::string& parent, const std::string& name) override;

				void compileString(unsigned int index, const std::string& parent, const std::string& name,
								   const std::string& value) override;

				void compileStringArray(unsigned int index, const std::string& parent, const std::string& name,
										const std::vector<std::string>& value) override;
		};
	}
}

#endif /* SIMPLICITY_EDITOR_ENGINECONFIGCOMPILER_H */
