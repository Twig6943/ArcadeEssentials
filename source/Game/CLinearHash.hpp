#pragma once
#include <cstdint>

template <typename T> struct SEntry {
	SEntry* next;
	std::uint32_t hash;
	const void* test_key;
	std::int32_t key_len;
	T value;
};

template <typename T> struct CLinearHash {
	std::uint32_t max_segments;
	std::uint32_t segment_shift;
	std::uint32_t segment_mask;
	std::uint32_t remove_threshold;
	SEntry<T>*** bucket_segment;
	SEntry<T>** entry_segment;
	std::uint32_t segment_count;
	std::uint32_t entry_count;
	std::uint32_t split_control;
	std::uint32_t split_index;
	std::uint32_t front_mask;
	SEntry<T>* entry_pool;
};