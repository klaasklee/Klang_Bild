/*
  ==============================================================================

    CrosssynthProcessor.h
    Created: 30 Mar 2024 2:44:23pm
    Author:  lhinz

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

/**
  STFT analysis and resynthesis of audio data.

  Each channel should have its own CrosssynthProcessor.
 */
class CrosssynthProcessor
{
public:
    CrosssynthProcessor();

    int getLatencyInSamples() const { return fftSize; }

    void reset();
    float processSample(float sampleA, float sampleB, bool bypassed);
    void processBlock(float* dataA, float* dataB, int numSamples, bool bypassed);
    void setSmoothingOrder(int smoOrder) {
        smoothingOrder = smoOrder;
    }
    
    void setTrebleBoost(float boost) {
        trebleBoost = boost;
    }

private:
    void processFrame(bool bypassed);
    void processSpectrum(float* dataA, float* dataB, int numBins);

    // The FFT has 2^order points and fftSize/2 + 1 bins.
    static constexpr int fftOrder = 10;
    static constexpr int fftSize = 1 << fftOrder;      // 1024 samples
    static constexpr int numBins = fftSize / 2 + 1;    // 513 bins
    static constexpr int overlap = 4;                  // 75% overlap
    static constexpr int hopSize = fftSize / overlap;  // 256 samples

    // Gain correction for using Hann window with 75% overlap.
    static constexpr float windowCorrection = 2.0f / 3.0f;

    juce::dsp::FFT fft;
    juce::dsp::WindowingFunction<float> window;

    // Counts up until the next hop.
    int count = 0;

    // Write position in input FIFO and read position in output FIFO.
    int pos = 0;

    // Circular buffers for incoming and outgoing audio data.
    std::array<float, fftSize> inputFifoA;
    std::array<float, fftSize> outputFifoA;
    std::array<float, fftSize> inputFifoB;
    std::array<float, fftSize> outputFifoB;

    // The FFT working space. Contains interleaved complex numbers.
    std::array<float, fftSize * 2> fftDataA;
    std::array<float, fftSize * 2> fftDataB;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CrosssynthProcessor)

    int smoothingOrder = 4;
    float trebleBoost = 0;
};