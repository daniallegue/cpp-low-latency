#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "macros.h"

namespace Common {
	template <typename T>
	class MemPool final {
	private:
		struct ObjectBlock
		{
			T object_;
			bool is_free_ = true;
		};
	
		std::vector<ObjectBlock> store_;
		size_t next_free_idx = 0;

		auto updateNextFreeIndex() noexcept {
			const auto initial_free_index = next_free_idx;
			while (!store_[next_free_idx].is_free_) {
				++next_free_idx;
				if (UNLIKELY(next_free_index_ == store_.size())) {
					//Branch prediction
					next_free_idx = 0;
				}
				if (UNLIKELY(initial_free_index == next_free_idx)) {
					ASSERT(initial_free_index != next_free_index_, "Memory Pool out of space.");
				}
			}
		}

	public:
		explicit MemPool(std::size_t num_elems) :
			store_(num_elems, {T(), true})
			ASSERT(reinterpret_cast<const ObjectBlock *>
				(& (store[0].object_)) == &(store[0]), "T object should be first member of ObjectBlock.")

		template<typename... Args>
		T* allocate(Args... args) noexcept {
			auto obj_block = &(store_[next_free_idx]);
			ASSERT(obj_block->is_free_, "Expected free ObjectBlock at index:" + std::to_string(next_free_idx));
			T* ret = &(obj_block->object_);
			ret = new(ret) T(args...);
			obj_block->is_free_ = false;

			updateNextFreeIndex();

			return ret;
		}


		
		MemPool() = delete;

		MemPool(const MemPool&) = delete;

		MemPool(const MemPool&&) = delete;

		MemPool& operator=(const MemPool&) = delete;

		MemPool& operator=(const MemPool&&) = delete;
	};
}