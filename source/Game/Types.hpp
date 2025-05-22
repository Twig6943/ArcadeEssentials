#pragma once
#include <cstdint>
#include "Kernel.hpp"

using u64 = std::uint64_t;
using s64 = std::int64_t;

using u32 = std::uint32_t;
using s32 = std::int32_t;

using u16 = std::uint16_t;
using s16 = std::int16_t;

using u8 = std::uint8_t;
using s8 = std::int8_t;

namespace Types {

	class TrackedRefId {
	public:
		inline TrackedRefId() {}
		operator u32() const { return id; }
	private:
		u32 id;
		static u32 s_nextId;
	};

	template <class T> class SmartPointer {
	public:
		T* target;
		TrackedRefId* ref_id;
	public:
	};

	class ReferenceCounter {
	public:
		enum Flags {
			GarbageCollectible = 0x80000000,
			GarbageCollected = 0x40000000,
			FlagsMask = 0xC0000000,
			CategoryMask = 0x3FC00000,
			CategoryShift = 22,
			RefCountMask = ~(FlagsMask | CategoryMask)
		};

		mutable volatile u32 ref_count;
	public:
		inline virtual ~ReferenceCounter() {
		}
	};

	class SmartHandleIndirection : public ReferenceCounter {
	public:
		mutable volatile void* pointer;
		Kernel::Thread::SpinLock pointer_lock;
	public:
	};

	class ReferenceCountable : public ReferenceCounter {
	public:
		SmartPointer<SmartHandleIndirection> indirection;
	public:
		inline virtual ~ReferenceCountable() override {
		}
	};

	class GarbageCollectible : public ReferenceCountable {
	public:
		inline virtual ~GarbageCollectible() override {
		}
		inline virtual void Dispose() {
		}
	};

	static_assert(sizeof(ReferenceCounter) == 8);
	static_assert(sizeof(ReferenceCountable) == 16);
	static_assert(sizeof(GarbageCollectible) == 16);
};