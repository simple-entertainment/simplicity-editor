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
#include <simplicity/rendering/Camera.h>
#include <simplicity/rendering/RenderingEngine.h>
#include <simplicity/scripting/ScriptingEngine.h>
#include <simplicity/Simplicity.h>
#include <simplicity/windowing/WindowEngine.h>

#include "EditContext.h"
#include "scripting/CameraController.h"

using namespace std;

namespace simplicity
{
	namespace editor
	{
		EditContext::EditContext() :
				compositeEngine(),
				editCameraEntity(new Entity),
				gameCameraEntity(nullptr)
		{
			unique_ptr<Engine> scriptingEngine(new ScriptingEngine);
			compositeEngine.addEngine(move(scriptingEngine));

			compositeEngine.onPlay();
		}

		void EditContext::advance()
		{
			Simplicity::getEngine<WindowEngine>()->advance(*Simplicity::getScene());
			Simplicity::getEngine<RenderingEngine>()->advance(*Simplicity::getScene());

			compositeEngine.advance(*Simplicity::getScene());
		}

		void EditContext::dispose()
		{
			compositeEngine.onStop();
		}

		void EditContext::enter()
		{
			compositeEngine.onResumeScene(*Simplicity::getScene());

			gameCameraEntity = Simplicity::getEngine<RenderingEngine>()->getCamera();
			Camera* gameCamera= gameCameraEntity->getComponent<Camera>();
			Camera* editCamera = editCameraEntity->getComponent<Camera>();

			editCamera->setProjection(gameCamera->getProjection());
			editCamera->setTransform(gameCamera->getTransform());
			editCameraEntity->setTransform(gameCameraEntity->getTransform());

			Simplicity::getEngine<RenderingEngine>()->setCamera(editCameraEntity.get());
		}

		void EditContext::exit()
		{
			compositeEngine.onPauseScene(*Simplicity::getScene());

			Simplicity::getEngine<RenderingEngine>()->setCamera(gameCameraEntity);
		}

		void EditContext::init()
		{
			compositeEngine.onBeforeOpenScene(*Simplicity::getScene());
			compositeEngine.onOpenScene(*Simplicity::getScene());

			unique_ptr<Component> editCamera(new Camera);
			editCameraEntity->addComponent(move(editCamera));
			unique_ptr<Component> editCameraController(new CameraController);
			editCameraEntity->addComponent(move(editCameraController));
		}
	}
}
