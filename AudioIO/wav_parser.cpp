#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>


#pragma pack(push, 1)
struct WavHeader {
	char chunkId[4];
	uint32_t chunkSize;
	char format[4];

	char subchunkFormat[4];
	uint32_t subchunk1Size;
	uint16_t audioFormat;
	uint16_t numChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;

	char subchunk2ID[4];
	uint32_t subchunk2Size;
};
#pragma pack(pop)

std::vector<double> loadWavToMonoDouble(const std::string& filename, uint32_t& outSampleRate){

	std::ifstream file(filename, std::ios::binary);
	
	if(!file.is_open()){
		throw std::runtime_error("Could not open file: " + filename);
	}

	WavHeader header;

	file.read(reinterpret_cast<char*>(&header), sizeof(WavHeader));	

	if(std::string(header.chunkId,4)!="RIFF" || std::string(header.format,4)!="WAVE"){
		throw std::runtime_error("file is not standard RIFF/WAVE format. ");
	}

	if(header.audioFormat != 1){
		throw std::runtime_error("audio is not uncompressed PCM.");
	}

	if(header.bitsPerSample != 16){
		throw std::runtime_error("this parser only supports 16 bits sampled audio.");
	}

	outSampleRate = header.sampleRate;

	uint32_t numSamples = header.subchunk2Size / (header.bitsPerSample / 8);

	std::vector<uint16_t> rawAudio(numSamples); 

	file.read(reinterpret_cast<char*>(rawAudio.data()), header.subchunk2Size);
	file.close();

	//we convert to mono if needed and then normalize to [-1,1] : 
	uint32_t frames = numSamples / header.numChannels;
	std::vector<double> monoAudio(frames);

	for(uint32_t i{}; i < frames; i++){
		double sample = 0.0;
		if(header.numChannels == 2){
			double left = rawAudio[i * 2] / 32768.0;
			double right = rawAudio[i * 2 + 1] / 32768.0;
			sample = (left + right) / 2.0;
		}else{
			sample = rawAudio[i] / 32768.0;	
		}
		monoAudio[i] = sample;
	}
	return monoAudio; 
}


