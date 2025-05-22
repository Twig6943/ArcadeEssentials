#pragma once
#include <cstdint>

namespace Kernel {
	namespace Thread {
		class Atomic {};
		class CriticalSection {};
		class Event {};
		class Mutex {};
		class Semaphore {};
		class SingleWriterMultipleReader {};
		class SpinLock {
		private:
			volatile std::uint32_t lock;
		};
	};
};