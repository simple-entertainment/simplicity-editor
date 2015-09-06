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
#ifndef SIMPLICITY_EDITOR_ENTITYCONFIGCOMPILER_H
#define SIMPLICITY_EDITOR_ENTITYCONFIGCOMPILER_H

#include <set>

#include <simplicity/resources/Resource.h>

#include "../common/ConfigCompiler.h"

namespace simplicity
{
	namespace editor
	{
		class EntityConfigCompiler : public ConfigCompiler
		{
			public:
				EntityConfigCompiler();

				void write(Resource& destination);

			private:
				std::string compiledComment;

				std::string compiledComponentComment;

				std::string compiledComponentPosition;

				std::vector<std::string> compiledComponentProperties;

				std::string compiledComponentRotation;

				std::vector<std::string> compiledComponents;

				std::vector<std::string> compiledEntities;

				std::set<std::string> compiledIncludes;

				std::string compiledPosition;

				std::string compiledRotation;

				unsigned int componentCount;

				std::string componentType;

				std::set<std::string> modelRecipeComponentTypes;

				void compileBoolean(unsigned int index, const std::string& parent, const std::string& name,
									bool value) override;

				void compileBooleanArray(unsigned int index, const std::string& parent, const std::string& name,
										 const std::vector<bool>& value) override;

				std::string compileComponentConstructor(unsigned int index);

				void compileNumber(unsigned int index, const std::string& parent, const std::string& name,
								   float value) override;

				void compileNumberArray(unsigned int index, const std::string& parent, const std::string& name,
										const std::vector<float>& value) override;

				void compileObject(unsigned int index, const std::string& parent, const std::string& name) override;

				std::string compilePosition(const std::string& object, const std::vector<float>& value);

				void compileProperty(unsigned int index, const std::string& name, const std::string& compiledValue);

				std::string compileRotation(const std::string& object, const std::vector<float>& value);

				void compileString(unsigned int index, const std::string& parent, const std::string& name,
								   const std::string& value) override;

				void compileStringArray(unsigned int index, const std::string& parent, const std::string& name,
										const std::vector<std::string>& value) override;
		};
	}
}

#endif /* SIMPLICITY_EDITOR_ENTITYCONFIGCOMPILER_H */
