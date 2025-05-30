#pragma once
#include <array>
#include <cstddef>

template <std::size_t BUFFER_LEN> struct PIDController {
public:
	float proportional_constant;
	float integral_constant;
	float derivative_constant;
private:
	std::array<float, BUFFER_LEN> error_buffer;
};