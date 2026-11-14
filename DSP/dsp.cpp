#include <vector>
#include <math.h>
#include <cstdint>



const double PI = std::acos(-1.0);

std::vector<std::vector<double>> frameAndWindow(
		const std::vector<double> &audio,
		uint32_t sampleRate,
		double frameSizeMs = 25.0,
		double frameStrideMs = 10.0
		){

	size_t frameLength = static_cast<size_t>(sampleRate * frameSizeMs / 1000.0);
	size_t frameStep = static_cast<size_t>(sampleRate * frameStrideMs / 1000.0);
	size_t totalSamples = audio.size();


	if(totalSamples < frameLength){
		return {}; 
	}

	if(frameLength < 2 || frameStep ==  0){
		return {};
	}
	size_t numFrames = 1 + (totalSamples - frameLength) / frameStep;


	std::vector<double> hammingWindow(frameLength);
	for(size_t n{}; n < frameLength; n++){

		hammingWindow[n] =  0.54 - 0.46 * std::cos(2.0 * PI * n / (frameLength - 1));
	
	}


	std::vector<std::vector<double>> frames(numFrames, std::vector<double>(frameLength));
	for(size_t i{}; i < numFrames; i++){
		size_t startIndex = i * frameStep;

		for(size_t j{}; j < frameLength; j++){
			frames[i][j] = audio[startIndex + j] * hammingWindow[j];
		}
	}

	return frames;

}
