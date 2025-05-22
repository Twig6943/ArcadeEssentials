#pragma once

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
			volatile uint32_t lock;
		};
	};
};