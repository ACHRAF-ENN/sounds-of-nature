/*
#include <iostream>
#include <vector>
#include <cstdint>
#include "./AudioIO/wav_parser.hpp"




int main(){
	try{
		uint32_t sampleRate = 0;
		std::string testFile = "./data/raw/audio/1-115545-A-48.wav";

		


		std::vector<double> audioData = loadWavToMonoDouble(testFile, sampleRate);
		
		std::cout << "--- WAV PARSER TEST ---" << "\n";
		std::cout << "Successfully loaded: " << testFile << "\n";
		std::cout << "Sample Rate:       " << sampleRate << " Hz\n";
		std::cout << "Total Samples:     " << audioData.size() << "\n";
		std::cout << "Audio Duration:    " << static_cast<double>(audioData.size()) / 
			sampleRate << " seconds\n";
		
		std::cout << "\nFirst 5 normalized samples:\n";
		for(size_t i = 0; i < 5 && i < audioData.size(); ++i) {
		    std::cout << "  [" << i << "] " << audioData[i] << "\n";
		}
    	} catch (const std::exception& e) {
		std::cerr << "TEST FAILED: " << e.what() << "\n";
	}
    return 0;
}
*/
#include "DSP/dsp.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    const uint32_t sr = 22050;
    const double frameMs = 25.0, strideMs = 10.0;

    // Expected sizes, computed independently of the function under test
    const size_t N = static_cast<size_t>(sr * frameMs / 1000.0);   // 551
    const size_t H = static_cast<size_t>(sr * strideMs / 1000.0);  // 220
    assert(N == 551 && H == 220);

    // 440 Hz sine, 1 second
    std::vector<double> sine(sr);
    for (size_t i = 0; i < sine.size(); ++i)
        sine[i] = std::sin(2.0 * M_PI * 440.0 * i / sr);

    auto frames = frameAndWindow(sine, sr, frameMs, strideMs);

    // Test 1: frame count and frame size
    size_t expected = 1 + (sine.size() - N) / H;   // 98
    assert(frames.size() == expected);
    for (auto& f : frames) assert(f.size() == N);
    std::cout << "Test 1 OK: " << frames.size() << " frames of " << N << " samples\n";

    // Test 2: constant signal of 1.0 -> frame must equal the Hamming window
    std::vector<double> ones(sr, 1.0);
    auto onesFrames = frameAndWindow(ones, sr, frameMs, strideMs);
    for (size_t i = 0; i < N; ++i) {
        double w = 0.54 - 0.46 * std::cos(2.0 * M_PI * i / (N - 1));
        assert(std::abs(onesFrames[0][i] - w) < 1e-12);
    }
    assert(std::abs(onesFrames[0][0] - 0.08) < 1e-12);       // edge = 0.08
    assert(std::abs(onesFrames[0][N - 1] - 0.08) < 1e-12);
    std::cout << "Test 2 OK: window values are correct\n";

    // Test 3: every frame is the right slice of the audio, windowed
    for (size_t k : {size_t(0), frames.size() / 2, frames.size() - 1}) {
        size_t s = k * H;
        for (size_t i = 0; i < N; ++i) {
            double w = 0.54 - 0.46 * std::cos(2.0 * M_PI * i / (N - 1));
            assert(std::abs(frames[k][i] - sine[s + i] * w) < 1e-12);
        }
    }
    std::cout << "Test 3 OK: frames line up with the audio (stride is right)\n";

    // Test 4: the last frame must not run past the end of the audio
    assert((frames.size() - 1) * H + N <= sine.size());
    std::cout << "Test 4 OK: no out-of-bounds frame\n";

    // Test 5: audio shorter than one frame returns nothing
    std::vector<double> tiny(N - 1, 0.5);
    assert(frameAndWindow(tiny, sr, frameMs, strideMs).empty());
    std::cout << "Test 5 OK: short input returns empty\n";

    std::cout << "All tests passed\n";
}


