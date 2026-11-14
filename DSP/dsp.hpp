#pragma once
#include <vector>
#include <cstdint>



std::vector<std::vector<double>> frameAndWindow(
		const std::vector<double> &audio,
		uint32_t sampleRate,
		double frameSizeMs = 25.0,
		double frameStrideMs = 10.0
		);
