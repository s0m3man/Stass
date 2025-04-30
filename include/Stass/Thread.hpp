#pragma once

#include "Main.hpp"

namespace Stass {
	class Thread {
	public:
		using id = std::thread::id;

		bool isWorking() const {
			return working;
		}

		id getId() const {
			return thread.get_id();
		}

		template <typename Func, typename... Args>
		void Execute(Func&& func, Args&&... args) {
			working = true;

			thread = std::thread(func, args...);

			working = false;
		}

	private:
		std::thread thread;

		bool working = false;
	};
}