#pragma once
#include <string>
#include <cstdint>
#include <vector>



std::vector<double> loadWavToMonoDouble(const std::string& filename, uint32_t& outSampleRate);

