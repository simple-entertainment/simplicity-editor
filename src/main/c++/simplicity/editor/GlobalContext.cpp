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
#include <simplicity/scripting/ScriptingEngine.h>
#include <simplicity/Simplicity.h>

#include "GlobalContext.h"
#include "scripting/UIController.h"

using namespace simplicity::simcef;
using namespace std;

namespace simplicity
{
	namespace editor
	{
		GlobalContext::GlobalContext(const string& editorHome) :
				compositeEngine(),
				initialized(false),
				uiDataStore(editorHome),
				uiEngine(nullptr),
				uiEntity(nullptr)
		{
			std::unique_ptr<Engine> scriptingEngine(new ScriptingEngine);
			compositeEngine.addEngine(move(scriptingEngine));

			std::unique_ptr<CEFEngine> uiEngine(new CEFEngine);
			this->uiEngine = uiEngine.get();
			compositeEngine.addEngine(move(uiEngine));

			compositeEngine.onPlay();
		}

		GlobalContext::~GlobalContext()
		{
			compositeEngine.onStop();
		}

		void GlobalContext::advance()
		{
			compositeEngine.advance();
		}

		void GlobalContext::enter()
		{
			compositeEngine.onResumeScene(*Simplicity::getScene());

			if (!initialized)
			{
				uiEntity = uiEngine->createUIEntity(*uiDataStore.get("src/main/html/ui.html", Category::UNCATEGORIZED,
																	 false));
				Entity* uiEntityRaw = uiEntity.get();

				unique_ptr<Component> uiController(new UIController);
				uiEntity->addUniqueComponent(move(uiController));
				Simplicity::getScene()->addEntity(move(uiEntity)); // TODO Is there another way to get this rendered?
				compositeEngine.onAddEntity(*uiEntityRaw);

				initialized = true;
			}
		}

		void GlobalContext::exit()
		{
			compositeEngine.onPauseScene(*Simplicity::getScene());
		}
	}
}
