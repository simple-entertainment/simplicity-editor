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
#include <rapidjson/reader.h>

#include <simplicity/resources/FileSystemDataStore.h>

#include "EngineConfig.h"
#include "EngineConfigCompiler.h"

using namespace rapidjson;
using namespace std;

namespace simplicity
{
	namespace editor
	{
		namespace EngineConfig
		{
			class IStreamWrapper {
			public:
				typedef char Ch;
				IStreamWrapper(std::istream& is) : is_(is) {
				}
				Ch Peek() const { // 1
					int c = is_.peek();
					return c == std::char_traits<char>::eof() ? '\0' : (Ch)c;
				}
				Ch Take() { // 2
					int c = is_.get();
					return c == std::char_traits<char>::eof() ? '\0' : (Ch)c;
				}
				size_t Tell() const { return (size_t)is_.tellg(); } // 3
				Ch* PutBegin() { assert(false); return 0; }
				void Put(Ch) { assert(false); }
				void Flush() { assert(false); }
				size_t PutEnd(Ch*) { assert(false); return 0; }
			private:
				IStreamWrapper(const IStreamWrapper&);
				IStreamWrapper& operator=(const IStreamWrapper&);
				std::istream& is_;
			};

			/*void compile(const Engine& engine, unsigned int index)
			{
				PhysicsEngine* physicsEngine = dynamic_cast<PhysicsEngine*>(&engine);
				if (physicsEngine != nullptr)
				{
					resource->appendData("\tunique_ptr<Engine> windowEngine(new ");
					resource->appendData(metaData["type"]);
					resource->appendData("(\"");
					resource->appendData(metaData["title"]);
					resource->appendData("\")");
					resource->appendData("\tSimplicity::addEngine(move(windowEngine));");

					return;
				}

				WindowEngine* windowEngine = dynamic_cast<WindowEngine*>(&engine);
				if (windowEngine != nullptr)
				{
					resource->appendData("\tunique_ptr<Engine> windowEngine(new ");
					resource->appendData(metaData["type"]);
					resource->appendData("(\"");
					resource->appendData(metaData["title"]);
					resource->appendData("\")");
					resource->appendData("\tSimplicity::addEngine(move(windowEngine));");

					return;
				}


			}*/

			void compile(const std::string& projectHome)
			{
				FileSystemDataStore projectDataStore(Resource::Type::USER, projectHome);

				unique_ptr<istream> istream =
						projectDataStore.get("simplicity.json", false)->getInputStream();
				IStreamWrapper inputStream(*istream);
				EngineConfigCompiler compiler;
				Reader().Parse(inputStream, compiler);
				compiler.write(*projectDataStore.get("src/generated/simplicity-generated-engine.cpp", false));
			}
		}
	}
}
