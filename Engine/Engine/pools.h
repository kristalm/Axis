#pragma once
#include <type_traits>
#include <array>
#include <cassert>
#include <list>

using namespace std;

namespace end
{
	template<typename T, size_t N>
	class pool
	{
		using storage_type = std::aligned_storage_t<sizeof(T), alignof(T)>;

		size_t free_start = 0;

		union entry_t
		{
			storage_type storage;
			size_t next;
		};

		std::array<entry_t, N> data;

	public:

		pool()
		{
			for (int i = 0; i < data.size(); i++)
			{
				data[i].next = i + 1;
			}

		}

		template<typename...Args>
		T* allocate(Args&&...args)
		{
			if (free_start == N)
			{
				return nullptr;
			}
			void* space = &data[free_start];


			free_start = data[free_start].next;


			return new (space) T{ std::forward<Args>(args)... };
		}

		void free(T* ptr)
		{
			T* firstUp = (T*)&data[0];
			auto differential = ptr - firstUp;



			if (differential >= 0 && differential < N)
			{
				ptr->~T();
				data[differential].next = free_start;
				free_start = differential;
			}


		}
	};
}

template<typename T, uint16_t N>
class sorted_pool
{
public:

	using storage_type = std::aligned_storage_t<sizeof(T), alignof(T)>;

	uint16_t particles = 0;

	std::array<storage_type, N> sortedParticlePool;

public:

	template<typename...Args>
	T* allocate(Args&&...args)
	{
		if (particles == N)
		{
			return nullptr;
		}

		void* poolSpace = &sortedParticlePool[particles++];



		return new (poolSpace) T{ std::forward<Args>(args)... };
	}

	void free(T* ptr)
	{
		T* firstUp = (T*)&sortedParticlePool[0];
		auto differential = ptr - firstUp;



		if (differential >= 0 && differential < N)
		{
			ptr->~T();
			sortedParticlePool[differential] = sortedParticlePool[--particles];
		}

	}

	int activeParticle()
	{
		return particles;
	}
	T& operator[](int k)
	{
		return (T&)sortedParticlePool[k];
	}



};
