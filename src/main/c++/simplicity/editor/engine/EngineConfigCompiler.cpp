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

using namespace rapidjson;
using namespace std;

namespace simplicity
{
	namespace editor
	{
		EngineConfigCompiler::EngineConfigCompiler() :
				compiledConstructor(),
				compiledEngines(),
				compiledIncludes(),
				compiledInitFunction(),
				compiledPropertyAssignments(),
				index(0),
				key(),
				numbers(),
				objects()
		{
			compiledIncludes.insert("#include <simplicity/API.h>");
		}

		bool EngineConfigCompiler::Bool(bool b)
		{
			return true;
		}

		string EngineConfigCompiler::commaSeparatedListFromNumbers()
		{
			stringstream stream;
			for (float number : numbers)
			{
				stringstream numberStream;
				numberStream << number;
				if (numberStream.str().find('.') == string::npos)
				{
					numberStream << ".0";
				}

				stream << numberStream.str() << "f, ";
			}

			string commaSeparatedList = stream.str();
			commaSeparatedList = commaSeparatedList.substr(0, commaSeparatedList.length() - 2);

			return commaSeparatedList;
		}

		bool EngineConfigCompiler::Double(double d)
		{
			numbers.push_back(static_cast<float>(d));

			return true;
		}

		bool EngineConfigCompiler::EndArray(SizeType elementCount)
		{
			if (objects.top() == "properties")
			{
				string className = "";
				if (numbers.size() == 3)
				{
					className = "sim::Vector3";
				}
				else if (numbers.size() == 4)
				{
					className = "sim::Vector4";
				}
				else if (numbers.size() == 9)
				{
					className = "sim::Matrix33";
				}
				else if (numbers.size() == 16)
				{
					className = "sim::Matrix44";
				}

				if (className != "")
				{
					stringstream stream;
					stream << "\tengine" << index << "->" << setterFromKey() << "(" << className << "(";
					stream << commaSeparatedListFromNumbers();
					stream << "));";
					compiledPropertyAssignments.push_back(stream.str());
				}
			}

			numbers.clear();

			return true;
		}

		bool EngineConfigCompiler::EndObject(SizeType memberCount)
		{
			objects.pop();

			if (objects.size() > 0 && objects.top() == "engines")
			{
				stringstream stream;
				stream << compiledComment << endl;
				stream << compiledConstructor << endl;
				for (string compiledPropertyAssignment : compiledPropertyAssignments)
				{
					stream << compiledPropertyAssignment << endl;
				}
				if (compiledInitFunction != "")
				{
					stream << compiledInitFunction << endl;
				}
				stream << "\tsim::Simplicity::addEngine(std::move(engine" << index << "));";

				compiledEngines.push_back(stream.str());
				compiledComment = "";
				compiledConstructor = "";
				compiledInitFunction = "";
				compiledPropertyAssignments.clear();

				index++;
			}

			return true;
		}

		bool EngineConfigCompiler::Int(int i)
		{
			numbers.push_back(i);

			return true;
		}

		bool EngineConfigCompiler::Int64(int64_t i)
		{
			numbers.push_back(i);

			return true;
		}

		bool EngineConfigCompiler::Key(const char* str, SizeType length, bool copy)
		{
			key = str;

			if (objects.top() == "engines")
			{
				stringstream stream;
				stream << "\tstd::unique_ptr<" << str << "> engine" << index << "(new " << str << ");";
				compiledConstructor = stream.str();
			}

			return true;
		}

		bool EngineConfigCompiler::Null()
		{
			return true;
		}

		std::string EngineConfigCompiler::setterFromKey()
		{
			string capitalKey = key;
			capitalKey[0] = static_cast<char>(toupper(capitalKey[0]));
			return string("set") + capitalKey;
		}

		bool EngineConfigCompiler::StartArray()
		{
			numbers.clear();

			return true;
		}

		bool EngineConfigCompiler::StartObject()
		{
			objects.push(key);
			key = "";

			return true;
		}

		bool EngineConfigCompiler::String(const char* str, SizeType length, bool copy)
		{
			if (objects.top() == "properties")
			{
				stringstream stream;
				stream << "\tengine" << index << "->" << setterFromKey() << "(\"" << str << "\");";
				compiledPropertyAssignments.push_back(stream.str());
			}
			else if (key == "header" || key == "headers")
			{
				stringstream stream;
				stream << "#include <" << str << ">";
				compiledIncludes.insert(stream.str());
			}
			else if (key == "initFunction")
			{
				stringstream stream;
				stream << "\tengine" << index << "->" << str << "();";
				compiledInitFunction = stream.str();
			}
			else if (key == "name")
			{
				stringstream stream;
				stream << "\t// " << str;
				compiledComment = stream.str();
			}

			return true;
		}

		bool EngineConfigCompiler::Uint(unsigned int u)
		{
			numbers.push_back(u);

			return true;
		}

		bool EngineConfigCompiler::Uint64(uint64_t u)
		{
			numbers.push_back(u);

			return true;
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
			stream << "}" << endl;

			destination.setData(stream.str());
		}
	}
}
