#pragma once

#include <array>
#include <cstdint>

namespace end
{
	struct float2
	{
		float x;
		float y;

		inline float& operator[](int i) { return (&x)[i]; }
		inline float operator[](int i)const { return (&x)[i]; }

		inline float* data() { return &x; }
		inline const float* data()const { return &x; }
		inline constexpr size_t size()const { return 2; }

		float2 operator+(float2 const &f1)
		{
			float2 vector;

			vector.x = x + f1.x;
			vector.y = y + f1.y;

			return vector;
		}
		void operator+=(float2 const &f1)
		{
			x = x + f1.x;
			y = y + f1.y;
		}
		float2 operator-(float2 const &f1)
		{
			float2 vector;

			vector.x = x - f1.x;
			vector.y = y - f1.y;

			return vector;
		}
		void operator-=(float2 const &f1)
		{
			x = x - f1.x;
			y = y - f1.y;
		}

		float2 operator*(float2 const &f1)
		{
			float2 vector;

			vector.x = x * f1.x;
			vector.y = y * f1.y;

			return vector;
		}
		void operator*= (float2 const &f1)
		{
			x = x * f1.x;
			y = y * f1.y;
		}
		float2 operator/(float2 const &f1)
		{
			float2 vector;

			vector.x = x / f1.x;
			vector.y = y / f1.y;

			return vector;
		}
		void operator/= (float2 const &f1)
		{
			x = x / f1.x;
			y = y / f1.y;
		}
	};

	struct float3
	{
		union
		{
			struct
			{
				float x;
				float y;
				float z;
			};

			float2 xy;
		};

		

		inline float& operator[](int i) { return (&x)[i]; }
		inline float operator[](int i)const { return (&x)[i]; }

		inline float* data() { return &x; }
		inline const float* data()const { return &x; }
		inline constexpr size_t size()const { return 3; }

		float3 operator+(float3 const &f1)
		{
			float3 vector;

			vector.x = x + f1.x;
			vector.y = y + f1.y;
			vector.z = z + f1.z;

			return vector;
		}
		void operator+=(float3 const &f1)
		{
			x = x + f1.x;
			y = y + f1.y;
			z = z + f1.z;
		}
		float3 operator-(float3 const &f1)
		{
			float3 vector;

			vector.x = x - f1.x;
			vector.y = y - f1.y;
			vector.z = z - f1.z;

			return vector;
		}
		void operator-=(float3 const &f1)
		{
			x = x - f1.x;
			y = y - f1.y;
			z = z - f1.z;
		}

		float3 operator*(float3 const &f1)
		{
			float3 vector;

			vector.x = x * f1.x;
			vector.y = y * f1.y;
			vector.z = z * f1.z;

			return vector;
		}
		void operator*= (float3 const &f1)
		{
			x = x * f1.x;
			y = y * f1.y;
			z = z * f1.z;
		}
		float3 operator/(float3 const &f1)
		{
			float3 vector;

			vector.x = x / f1.x;
			vector.y = y / f1.y;
			vector.z = z / f1.z;

			return vector;
		}
		void operator/= (float3 const &f1)
		{
			x = x / f1.x;
			y = y / f1.y;
			z = z / f1.z;
		}

		inline float3 operator*(float t)
		{
			float3 vector;
			vector.x = t * x;
			vector.y = t * y;
			vector.z = t * z;
			return vector;
		}

	};

	struct float4
	{
		union
		{
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};

			float3 xyz;

			struct
			{
				float2 xy;
				float2 zw;
			};
		};

		

		inline float& operator[](int i) { return (&x)[i]; }
		inline float operator[](int i)const { return (&x)[i]; }

		inline float* data() { return &x; }
		inline const float* data()const { return &x; }
		inline constexpr size_t size()const { return 4; }


		float4 operator+(float4 const &f1)
		{
			float4 vector; 

			vector.x = x + f1.x;
			vector.y = y + f1.y;
			vector.z = z + f1.z;
			vector.w = w + f1.w;
		
			return vector;
		}
		void operator+=(float4 const &f1)
		{
			x = x + f1.x;
			y = y + f1.y;
			z = z + f1.z;
			w = w + f1.w;
		}
		float4 operator-(float4 const &f1)
		{
			float4 vector;

			vector.x = x - f1.x;
			vector.y = y - f1.y;
			vector.z = z - f1.z;
			vector.w = w - f1.w;

			return vector;
		}
		void operator-=(float4 const &f1)
		{
			x = x - f1.x;
			y = y - f1.y;
			z = z - f1.z;
			w = w - f1.w;
		}

		float4 operator*(float4 const &f1)
		{
			float4 vector;

			vector.x = x * f1.x;
			vector.y = y * f1.y;
			vector.z = z * f1.z;
			vector.w = w * f1.w;

			return vector;
		}
		void operator*= (float4 const &f1)
		{
			x = x * f1.x;
			y = y * f1.y;
			z = z * f1.z;
			w = w * f1.w;
		}
		float4 operator/(float4 const &f1)
		{
			float4 vector;

			vector.x = x / f1.x;
			vector.y = y / f1.y;
			vector.z = z / f1.z;
			vector.w = w / f1.w;

			return vector;
		}
		void operator/= (float4 const &f1)
		{
			x = x / f1.x;
			y = y / f1.y;
			z = z / f1.z;
			w = w / f1.w;
		}
		inline float4 operator*(float t)
		{
			float4 vector;
			vector.x = t * x;
			vector.y = t * y;
			vector.z = t * z;
			vector.w = t * w;
			return vector;
		}
		

	};

	struct alignas(8) float2_a : float2 {};

	struct alignas(16) float3_a : float3 {};

	struct alignas(16) float4_a : float4 {};

	using float4x4 = std::array< float4, 4 >;
	using float4x4_a = std::array< float4_a, 4 >;
	

	inline float dot(float4 const &v, float4 const &w)
	{
		float prod;
		prod = (v.x * w.x) + (v.y * w.y) + (v.z * w.z) + (v.w * w.w);
		return prod;
	}
	inline float dot(float3 const &v, float3 const &w)
	{
		float prod;
		prod = (v.x * w.x) + (v.y * w.y) + (v.z * w.z);
		return prod;
	}
	inline float dot(float2 const &v, float2 const &w)
	{
		float prod;
		prod = (v.x * w.x) + (v.y * w.y);
		return prod;
	}

	inline float4 cross(float4 const &v, float4 const &w)
	{
		float4 value;
		value.x = ((v.y * w.z) - (v.z * w.y));
		value.y = -((v.x * w.z) - (v.z * w.x));
		value.z = ((v.x * w.y) - (v.y * w.x));
		value.w = 0;

		return value;
	}
	inline float3 cross(float3 const &v, float3 const &w)
	{
		float3 value;
		value.x = ((v.y * w.z) - (v.z * w.y));
		value.y = -((v.x * w.z) - (v.z * w.x));
		value.z = ((v.x * w.y) - (v.y * w.x));

		return value;
	}
	
	inline float4 normalize(float4 const &v, float4 const &w)
	{
		float square = dot(v, v);
		float length = sqrtf(square);

		float normal;
		normal = length;
		float4 final_v;
		if ((fabs(normal)) < std::numeric_limits<float>::epsilon())
		{
			final_v.w = 0;
			final_v.x = 0;
			final_v.y = 0;
			final_v.z = 0;

		}
		else
		{
			final_v.x = (v.x / normal);
			final_v.y = (v.y / normal);
			final_v.z = (v.z / normal);
			final_v.w = (v.w / normal);
		}
		return v;
	}
	inline float3 normalize(float3 const &v, float3 const &w)
	{
		float square = dot(v, v);
		float length = sqrtf(square);

		float normal;
		normal = length;
		float4 final_v;
		if ((fabs(normal)) < std::numeric_limits<float>::epsilon())
		{
			final_v.w = 0;
			final_v.x = 0;
			final_v.y = 0;

		}
		else
		{
			final_v.x = (v.x / normal);
			final_v.y = (v.y / normal);
			final_v.z = (v.z / normal);
		}
		return v;
	}


	inline float4 lerp(float4 v, float4 w, float t)
	{
		return (w - v) * t + v;
	}
	inline float3 lerp(float3 v, float3 w, float t)
	{
		return (w - v) * t + v;
	}
	
}

namespace end
{

	struct colored_vertex
	{
		float3 pos = { 0.0f, 0.0f, 0.0f };
		float4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		colored_vertex() = default;
		colored_vertex(const colored_vertex&) = default;

		inline colored_vertex(const float3& p, const float4& c) : pos{ p }, color{ c } {}
		inline colored_vertex(const float3& p, const float3& c) : pos{ p }, color{ c.x, c.y, c.z, 1.0f } {}
		inline colored_vertex(const float3& p, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : pos{ p }, color{ r/255.0f, g/255.0f, b/255.0f, a/255.0f } {}
	};
}