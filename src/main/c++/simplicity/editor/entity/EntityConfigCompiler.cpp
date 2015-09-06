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
#include <simplicity/math/MathConstants.h>

#include "EntityConfigCompiler.h"

using namespace std;

namespace simplicity
{
	namespace editor
	{
		EntityConfigCompiler::EntityConfigCompiler() :
				compiledComment(),
				compiledComponentComment(),
				compiledComponentPosition(),
				compiledComponentProperties(),
				compiledComponentRotation(),
				compiledComponents(),
				compiledEntities(),
				compiledIncludes(),
				compiledPosition(),
				compiledRotation(),
				componentCount(0),
				componentType(),
				modelRecipeComponentTypes()
		{
			compiledIncludes.insert("#include <simplicity/API.h>");

			modelRecipeComponentTypes.insert("simplicity::Box");
			modelRecipeComponentTypes.insert("simplicity::Circle");
			modelRecipeComponentTypes.insert("simplicity::Cylinder");
			modelRecipeComponentTypes.insert("simplicity::Hemisphere");
			modelRecipeComponentTypes.insert("simplicity::Prism");
			modelRecipeComponentTypes.insert("simplicity::Pyramid");
			modelRecipeComponentTypes.insert("simplicity::Rectangle");
			modelRecipeComponentTypes.insert("simplicity::Sphere");
		}

		void EntityConfigCompiler::compileBoolean(unsigned int index, const string& parent, const string& name,
												  bool value)
		{
			if (parent == "properties")
			{
				stringstream stream;
				stream << boolalpha << value;
				compileProperty(index, name, stream.str());
			}
		}

		void EntityConfigCompiler::compileBooleanArray(unsigned int index, const string& parent, const string& name,
													   const vector<bool>& value)
		{
		}

		string EntityConfigCompiler::compileComponentConstructor(unsigned int index)
		{
			stringstream stream;

			bool modelRecipeComponentType = false;
			if (modelRecipeComponentTypes.find(componentType) != modelRecipeComponentTypes.end())
			{
				stream << "\tsim::ModelFactory::Recipe recipe" << componentCount + index << ";" << endl;
				modelRecipeComponentType = true;
			}

			if (componentType == "simplicity::Box")
			{
				stream << "\trecipe" << componentCount + index <<
						".shape = sim::ModelFactory::Recipe::Shape::BOX;" << endl;
			}
			else if (componentType == "simplicity::Circle")
			{
				stream << "\trecipe" << componentCount + index <<
						".shape = sim::ModelFactory::Recipe::Shape::CIRCLE;" << endl;
			}
			else if (componentType == "simplicity::Cylinder")
			{
				stream << "\trecipe" << componentCount + index <<
						".shape = sim::ModelFactory::Recipe::Shape::CYLINDER;" << endl;
			}
			else if (componentType == "simplicity::Hemisphere")
			{
				stream << "\trecipe" << componentCount + index <<
						".shape = sim::ModelFactory::Recipe::Shape::HEMISPHERE;" << endl;
			}
			else if (componentType == "simplicity::Prism")
			{
				stream << "\trecipe" << componentCount + index <<
						".shape = sim::ModelFactory::Recipe::Shape::PRISM;" << endl;
			}
			else if (componentType == "simplicity::Pyramid")
			{
				stream << "\trecipe" << componentCount + index <<
						".shape = sim::ModelFactory::Recipe::Shape::PYRAMID;" << endl;
			}
			else if (componentType == "simplicity::Rectangle")
			{
				stream << "\trecipe" << componentCount + index <<
						".shape = sim::ModelFactory::Recipe::Shape::RECTANGLE;" << endl;
			}
			else if (componentType == "simplicity::Sphere")
			{
				stream << "\trecipe" << componentCount + index <<
						".shape = sim::ModelFactory::Recipe::Shape::SPHERE;" << endl;
			}
			else
			{
				stream << "\tstd::unique_ptr<" << componentType << "> component" << componentCount + index <<
						"(new " << componentType << ");" << endl;
			}

			if (modelRecipeComponentType)
			{
				for (string compiledComponentProperty : compiledComponentProperties)
				{
					stream << compiledComponentProperty << endl;
				}
				compiledComponentProperties.clear();

				stream << "\tstd::unique_ptr<sim::Component> component" << componentCount + index <<
						"(sim::ModelFactory::cookMesh(recipe" << componentCount + index << "));";
			}

			return stream.str();
		}

		void EntityConfigCompiler::compileNumber(unsigned int index, const string& parent, const string& name,
												 float value)
		{
			if (parent == "properties")
			{
				stringstream stream;
				stream << value;
				compileProperty(index, name, stream.str());
			}
		}

		void EntityConfigCompiler::compileNumberArray(unsigned int index, const string& parent, const string& name,
													  const vector<float>& value)
		{
			if (parent == "entities")
			{
				stringstream stream;
				stream << "entity" << index;

				if (name == "position")
				{
					compiledPosition = compilePosition(stream.str(), value);
				}
				else if (name == "rotation" && value.size() == 3)
				{
					compiledRotation = compileRotation(stream.str(), value);
				}
			}
			else if (parent == "components")
			{
				stringstream stream;
				stream << "component" << componentCount + index;

				if (name == "position")
				{
					compiledComponentPosition = compilePosition(stream.str(), value);
				}
				else if (name == "rotation")
				{
					compiledComponentRotation = compileRotation(stream.str(), value);
				}
			}
			else if (parent == "properties")
			{
				compileProperty(index, name, compileMathObject(value));
			}
		}

		void EntityConfigCompiler::compileObject(unsigned int index, const string& parent, const std::string& name)
		{
			if (!name.empty())
			{
				return;
			}

			if (parent == "entities")
			{
				stringstream stream;
				stream << compiledComment << endl;
				stream << "\tstd::unique_ptr<sim::Entity> entity" << index <<
						"(new sim::Entity(sim::Category::UNCATEGORIZED, \"" <<
						compiledComment.substr(4, compiledComment.size()) << "\"));" << endl;
				if (!compiledRotation.empty())
				{
					stream << compiledRotation << endl;
				}
				if (!compiledPosition.empty())
				{
					stream << compiledPosition << endl;
				}
				for (string compiledComponent : compiledComponents)
				{
					stream << compiledComponent << endl;
				}
				stream << "\tsim::Simplicity::getScene()->addEntity(std::move(entity" << index << "));";

				compiledEntities.push_back(stream.str());
				componentCount += compiledComponents.size();

				compiledComment = "";
				compiledComponents.clear();
				compiledPosition = "";
				compiledRotation = "";
			}
			else if (parent == "components")
			{
				stringstream stream;
				stream << compiledComponentComment << endl;
				stream << compileComponentConstructor(index) << endl;
				if (!compiledComponentRotation.empty())
				{
					stream << compiledComponentRotation << endl;
				}
				if (!compiledComponentPosition.empty())
				{
					stream << compiledComponentPosition << endl;
				}
				for (string compiledComponentProperty : compiledComponentProperties)
				{
					stream << compiledComponentProperty << endl;
				}
				stream << "\tentity" << compiledEntities.size() << "->addUniqueComponent(std::move(component" <<
						componentCount + index << "));";

				compiledComponents.push_back(stream.str());

				compiledComponentComment = "";
				compiledComponentPosition = "";
				compiledComponentProperties.clear();
				compiledComponentRotation = "";
				componentType = "";
			}
		}

		string EntityConfigCompiler::compilePosition(const string& object, const vector<float>& value)
		{
			if (value.size() != 3)
			{
				return "!error compiling position: requires 3 elements!";
			}

			if (value[0] == 0.0f && value[1] == 0.0f && value[2] == 0.0f)
			{
				return "";
			}

			stringstream stream;
			stream << "\tsim::setPosition(" << object << "->getTransform(), " << compileMathObject(value) << ");";

			return stream.str();
		}

		void EntityConfigCompiler::compileProperty(unsigned int index, const string& name, const string& compiledValue)
		{
			if (componentType.empty())
			{
				compiledComponentProperties.push_back("!error compiling property: component type not specified!");
			}
			else if (modelRecipeComponentTypes.find(componentType) != modelRecipeComponentTypes.end())
			{
				if (name == "depth" && componentType == "simplicity::Pyramid")
				{
					stringstream stream;
					stream << "\trecipe" << componentCount + index << ".dimensions[0] = " <<
					compiledValue << "; // depth";
					compiledComponentProperties.push_back(stream.str());
				}
				else if (name == "depth")
				{
					stringstream stream;
					stream << "\trecipe" << componentCount + index << ".dimensions[3] = " <<
					compiledValue << "; // depth";
					compiledComponentProperties.push_back(stream.str());
				}
				else if (name == "height")
				{
					stringstream stream;
					stream << "\trecipe" << componentCount + index << ".dimensions[1] = " <<
					compiledValue << "; // height";
					compiledComponentProperties.push_back(stream.str());
				}
				else if (name == "length")
				{
					stringstream stream;
					stream << "\trecipe" << componentCount + index << ".dimensions[1] = " <<
							compiledValue << "; // length";
					compiledComponentProperties.push_back(stream.str());
				}
				else if (name == "radius")
				{
					stringstream stream;
					stream << "\trecipe" << componentCount + index << ".dimensions[0] = " <<
							compiledValue << " * 2.0f; // radius (converted to diameter)";
					compiledComponentProperties.push_back(stream.str());
				}
				else if (name == "width")
				{
					stringstream stream;
					stream << "\trecipe" << componentCount + index << ".dimensions[0] = " <<
					compiledValue << "; // width";
					compiledComponentProperties.push_back(stream.str());
				}
				else
				{
					stringstream stream;
					stream << "\trecipe" << componentCount + index << "." << name << " = " << compiledValue << ";";
					compiledComponentProperties.push_back(stream.str());
				}
			}
			else
			{
				stringstream stream;
				stream << "\tcomponent" << componentCount + index << "->" << compileSetter(name, compiledValue) << ";";
				compiledComponentProperties.push_back(stream.str());
			}
		}

		string EntityConfigCompiler::compileRotation(const string& object, const vector<float>& value)
		{
			if (value.size() != 3)
			{
				return "!error compiling rotation: requires 3 elements!";
			}

			stringstream stream;

			if (value[0] != 0.0f)
			{
				float radiansX = value[0] / 180.0f * MathConstants::PI;
				stream << "\tsim::rotate(" << object << "->getTransform(), " << compileFloat(radiansX) <<
						", sim::Vector3(1.0f, 0.0f, 0.0f));";
			}

			if (value[1] != 0.0f)
			{
				if (!stream.str().empty())
				{
					stream << endl;
				}

				float radiansY = value[1] / 180.0f * MathConstants::PI;
				stream << "\tsim::rotate(" << object << "->getTransform(), " << compileFloat(radiansY) <<
						", sim::Vector3(0.0f, 1.0f, 0.0f));";
			}

			if (value[2] != 0.0f)
			{
				if (!stream.str().empty())
				{
					stream << endl;
				}

				float radiansZ = value[2] / 180.0f * MathConstants::PI;
				stream << "\tsim::rotate(" << object << "->getTransform(), " << compileFloat(radiansZ) <<
						", sim::Vector3(0.0f, 0.0f, 1.0f));";
			}

			return stream.str();
		}

		void EntityConfigCompiler::compileString(unsigned int index, const string& parent, const string& name,
												 const string& value)
		{
			if (parent == "entities")
			{
				if (name == "name")
				{
					compiledComment = "\t// " + value;
				}
			}
			else if (parent == "components")
			{
				if (name == "name")
				{
					compiledComponentComment = "\t// " + value;
				}
				else if (name == "header")
				{
					compiledIncludes.insert("#include <" + value + ">");
				}
				else if (name == "type")
				{
					componentType = value;
				}
			}
			else if (parent == "properties")
			{
				compileProperty(index, name, "\"" + value + "\"");
			}
		}

		void EntityConfigCompiler::compileStringArray(unsigned int index, const string& parent, const string& name,
													  const vector<string>& value)
		{
		}

		void EntityConfigCompiler::write(Resource& destination)
		{
			stringstream stream;
			stream << "// This file has been generated by the simplicity editor." << endl;
			stream << "// If you edit this file, be prepared for your changes to be overwritten!" << endl;
			stream << "// To manually configure an entity, press the 'manual' button in the simplicity editor." << endl;
			for (string compiledInclude : compiledIncludes)
			{
				stream << compiledInclude << endl;
			}
			stream << endl;
			stream << "extern \"C\" void simplicity_generated_setupScene()" << endl;
			stream << "{" << endl;
			for (int index = 0; index < compiledEntities.size(); index++)
			{
				if (index > 0)
				{
					stream << endl;
				}
				stream << compiledEntities[index] << endl;
			}
			stream << "}" << endl;

			destination.setData(stream.str());
		}
	}
}
