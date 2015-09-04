/*
 * Copyright © 2015 Simple Entertainment Limited
 *
 * This file is part of The Simplicity Engine.
 *
 * The Simplicity Engine is free software: you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * The Simplicity Engine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with The Simplicity Engine. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <simplicity/common/AddressEquals.h>
#include <simplicity/common/Timer.h>
#include <simplicity/logging/Logs.h>

#include "TimedSerialCompositeEngine.h"

using namespace std;
using namespace std::chrono;

namespace simplicity
{
	TimedSerialCompositeEngine::TimedSerialCompositeEngine() :
			engineFrameTimes(),
			engines(),
			frameCount(0),
			framesPerSecond(0),
			frameTime(0.0f)
	{
	}

	void TimedSerialCompositeEngine::addEngine(unique_ptr<Engine> engine)
	{
		engineFrameTimes.push_back(0.0f);
		engines.push_back(move(engine));
	}

	void TimedSerialCompositeEngine::advance()
	{
		Timer frameTimer;

		for (unsigned int index = 0; index < engines.size(); index++)
		{
			Timer engineFrameTimer;

			engines[index]->advance();

			engineFrameTimes[index] = engineFrameTimer.getElapsedTime();
		}

		frameTime = frameTimer.getElapsedTime();

		frameCount++;

		// If at least one second has passed since the last FPS was recorded.
		time_point<high_resolution_clock> frameEndTime = high_resolution_clock::now();
		if (duration_cast<seconds>(frameEndTime - lastFrameCountTime).count() >= 1)
		{
			lastFrameCountTime = frameEndTime;
			framesPerSecond = frameCount;
			frameCount = 0;
		}
	}

	const vector<float>& TimedSerialCompositeEngine::getEngineFrameTimes() const
	{
		return engineFrameTimes;
	}

	const vector<unique_ptr<Engine>>& TimedSerialCompositeEngine::getEngines() const
	{
		return engines;
	}

	unsigned int TimedSerialCompositeEngine::getFramesPerSecond() const
	{
		return framesPerSecond;
	}

	float TimedSerialCompositeEngine::getFrameTime() const
	{
		return frameTime;
	}

	void TimedSerialCompositeEngine::onAddEntity(Entity& entity)
	{
		for (unique_ptr<Engine>& engine : engines)
		{
			engine->onAddEntity(entity);
		}
	}

	void TimedSerialCompositeEngine::onCloseScene(Scene& scene)
	{
		for (unique_ptr<Engine>& engine : engines)
		{
			engine->onCloseScene(scene);
		}
	}

	void TimedSerialCompositeEngine::onOpenScene(Scene& scene)
	{
		for (unique_ptr<Engine>& engine : engines)
		{
			engine->onOpenScene(scene);
		}
	}

	void TimedSerialCompositeEngine::onPause()
	{
		for (unique_ptr<Engine>& engine : engines)
		{
			engine->onPause();
		}
	}

	void TimedSerialCompositeEngine::onPauseScene(Scene& scene)
	{
		for (unique_ptr<Engine>& engine : engines)
		{
			engine->onPauseScene(scene);
		}
	}

	void TimedSerialCompositeEngine::onPlay()
	{
		for (unique_ptr<Engine>& engine : engines)
		{
			engine->onPlay();
		}

		lastFrameCountTime = high_resolution_clock::now();
	}

	void TimedSerialCompositeEngine::onRemoveEntity(Entity& entity)
	{
		for (unique_ptr<Engine>& engine : engines)
		{
			engine->onRemoveEntity(entity);
		}
	}

	void TimedSerialCompositeEngine::onResume()
	{
		for (unique_ptr<Engine>& engine : engines)
		{
			engine->onResume();
		}
	}

	void TimedSerialCompositeEngine::onResumeScene(Scene& scene)
	{
		for (unique_ptr<Engine>& engine : engines)
		{
			engine->onResumeScene(scene);
		}
	}

	void TimedSerialCompositeEngine::onStop()
	{
		for (unique_ptr<Engine>& engine : engines)
		{
			engine->onStop();
		}
	}

	unique_ptr<Engine> TimedSerialCompositeEngine::removeEngine(Engine* engine)
	{
		unique_ptr<Engine> removedEngine;
		vector<unique_ptr<Engine>>::iterator result =
			find_if(engines.begin(), engines.end(), AddressEquals<Engine>(*engine));

		if (result != engines.end())
		{
			removedEngine = move(*result);
			engines.erase(result);
			engine = nullptr;
		}

		return move(removedEngine);
	}
}
