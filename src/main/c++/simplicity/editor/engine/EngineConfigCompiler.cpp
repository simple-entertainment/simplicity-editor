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

#include "EngineConfigCompiler.h"

using namespace std;

namespace simplicity
{
	namespace editor
	{
		EngineConfigCompiler::EngineConfigCompiler() :
				compiledComment(),
				compiledConstructor(),
				compiledEngines(),
				compiledFactories(),
				compiledIncludes(),
				compiledInitFunction(),
				compiledProperties(),
				interface(),
				type()
		{
			compiledIncludes.insert("#include <simplicity/API.h>");
		}

		void EngineConfigCompiler::compileBoolean(unsigned int index, const string& parent, const string& name,
												  bool value)
		{
		}

		void EngineConfigCompiler::compileBooleanArray(unsigned int index, const string& parent, const string& name,
													   const vector<bool>& value)
		{
		}

		void EngineConfigCompiler::compileNumber(unsigned int index, const string& parent, const string& name,
												 float value)
		{
		}

		void EngineConfigCompiler::compileNumberArray(unsigned int index, const string& parent, const string& name,
													  const vector<float>& value)
		{
			if (parent == "properties")
			{
				stringstream stream;
				stream << "\tengine" << index << "->" << compileSetter(name, compileMathObject(value)) << ";";
				compiledProperties.push_back(stream.str());
			}
		}

		void EngineConfigCompiler::compileObject(unsigned int index, const string& parent, const std::string& name)
		{
			if (!name.empty())
			{
				return;
			}

			if (parent == "engines")
			{
				stringstream stream;
				stream << compiledComment << endl;
				stream << compiledConstructor << endl;
				for (string compiledProperty : compiledProperties)
				{
					stream << compiledProperty << endl;
				}
				if (!compiledInitFunction.empty())
				{
					stream << compiledInitFunction << endl;
				}
				stream << "\tsim::Simplicity::addEngine(std::move(engine" << index << "));";

				compiledEngines.push_back(stream.str());

				compiledComment = "";
				compiledConstructor = "";
				compiledInitFunction = "";
				compiledProperties.clear();
			}
			else if (parent == "factories")
			{
				stringstream stream;
				stream << compiledComment << endl;
				stream << compiledConstructor << endl;
				stream << compiledInitFunction;

				compiledFactories.push_back(stream.str());

				compiledComment = "";
				compiledConstructor = "";
				compiledInitFunction = "";
				interface= "";
				type= "";
			}
		}

		void EngineConfigCompiler::compileString(unsigned int index, const string& parent, const string& name,
												 const string& value)
		{
			if (parent == "engines")
			{
				if (name == "type")
				{
					stringstream stream;
					stream << "\tstd::unique_ptr<" << value << "> engine" << index << "(new " << value << ");";
					compiledConstructor = stream.str();
				}
			}
			else if (parent == "factories")
			{
				if (name == "interface")
				{
					interface = value;
				}
				else if (name == "type")
				{
					type = value;
				}

				if (!interface.empty() && !type.empty())
				{
					stringstream stream;
					stream << "\tstd::unique_ptr<sim::" << interface << "> factory" << index << "(new " << type << ");";
					compiledConstructor = stream.str();

					stream.str("");
					stream.clear();
					stream << "\tsim::" << interface << "::setInstance(std::move(factory" << index << "));";
					compiledInitFunction = stream.str();
				}
			}

			if (parent == "engines" || parent == "factories")
			{
				if (name == "header")
				{
					compiledIncludes.insert("#include <" + value + ">");
				}
				else if (name == "initFunction")
				{
					stringstream stream;
					stream << "\tengine" << index << "->" << value << "();";
					compiledInitFunction = stream.str();
				}
				else if (name == "name")
				{
					compiledComment = "\t// " + value;
				}
			}
			else if (parent == "properties")
			{
				stringstream stream;
				stream << "\tengine" << index << "->" << compileSetter(name, "\"" + value + "\"") << ";";
				compiledProperties.push_back(stream.str());
			}
		}

		void EngineConfigCompiler::compileStringArray(unsigned int index, const string& parent, const string& name,
													  const vector<string>& value)
		{
			if (name == "headers")
			{
				for (string header : value)
				{
					compiledIncludes.insert("#include <" + header + ">");
				}
			}
		}

		void EngineConfigCompiler::write(Resource& destination)
		{
			stringstream stream;
			stream << "// This file has been generated by the simplicity editor." << endl;
			stream << "// If you edit this file, be prepared for your changes to be overwritten!" << endl;
			stream << "// To manually configure an engine, press the 'manual' button in the simplicity editor." << endl;
			for (string compiledInclude : compiledIncludes)
			{
				stream << compiledInclude << endl;
			}
			stream << endl;
			stream << "extern \"C\" void simplicity_generated_setupEngine()" << endl;
			stream << "{" << endl;
			for (int index = 0; index < compiledEngines.size(); index++)
			{
				if (index > 0)
				{
					stream << endl;
				}
				stream << compiledEngines[index] << endl;
			}
			for (string compiledFactory : compiledFactories)
			{
				stream << endl;
				stream << compiledFactory << endl;
			}
			stream << "}" << endl;

			destination.setData(stream.str());
		}
	}
}
