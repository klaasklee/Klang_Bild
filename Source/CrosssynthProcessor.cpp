/*
  ==============================================================================

    CrosssynthProcessor.cpp
    Created: 30 Mar 2024 2:44:23pm
    Author:  lhinz

  ==============================================================================
*/

#include "CrosssynthProcessor.h"

CrosssynthProcessor::CrosssynthProcessor() :
    fft(fftOrder),
    window(fftSize + 1, juce::dsp::WindowingFunction<float>::WindowingMethod::hann, false)
{
    // Note that the window is of length `fftSize + 1` because JUCE's windows
    // are symmetrical, which is wrong for overlap-add processing. To make the
    // window periodic, set size to 1025 but only use the first 1024 samples.
}

void CrosssynthProcessor::reset()
{
    count = 0;
    pos = 0;

    // Zero out the circular buffers.
    std::fill(inputFifoA.begin(), inputFifoA.end(), 0.0f);
    std::fill(outputFifoA.begin(), outputFifoA.end(), 0.0f);
    std::fill(inputFifoB.begin(), inputFifoB.end(), 0.0f);
    std::fill(outputFifoB.begin(), outputFifoB.end(), 0.0f);

    smoothingOrder = 4;
}

void CrosssynthProcessor::processBlock(float* dataA, float* dataB, int numSamples, bool bypassed)
{
    for (int i = 0; i < numSamples; ++i) {
        dataA[i] = processSample(dataA[i], dataB[i], bypassed);
    }
}

float CrosssynthProcessor::processSample(float sampleA, float sampleB, bool bypassed)
{
    // Push the new sample value into the input FIFO.
    inputFifoA[pos] = sampleA;
    inputFifoB[pos] = sampleB;

    // Read the output value from the output FIFO. Since it takes fftSize
    // timesteps before actual samples are read from this FIFO instead of
    // the initial zeros, the sound output is delayed by fftSize samples,
    // which we will report as our latency.
    float outputSample = outputFifoA[pos];

    // Once we've read the sample, set this position in the FIFO back to
    // zero so we can add the IFFT results to it later.
    outputFifoA[pos] = 0.0f;
    outputFifoB[pos] = 0.0f;

    // Advance the FIFO index and wrap around if necessary.
    pos += 1;
    if (pos == fftSize) {
        pos = 0;
    }

    // Process the FFT frame once we've collected hopSize samples.
    count += 1;
    if (count == hopSize) {
        count = 0;
        processFrame(bypassed);
    }

    return outputSample;
}

void CrosssynthProcessor::processFrame(bool bypassed)
{
    const float* inputPtrA = inputFifoA.data();
    float* fftPtrA = fftDataA.data();
    const float* inputPtrB = inputFifoB.data();
    float* fftPtrB = fftDataB.data();

    // Copy the input FIFO into the FFT working space in two parts.
    std::memcpy(fftPtrA, inputPtrA + pos, (fftSize - pos) * sizeof(float));
    std::memcpy(fftPtrB, inputPtrB + pos, (fftSize - pos) * sizeof(float));
    if (pos > 0) {
        std::memcpy(fftPtrA + fftSize - pos, inputPtrA, pos * sizeof(float));
        std::memcpy(fftPtrB + fftSize - pos, inputPtrB, pos * sizeof(float));
    }

    // Apply the window to avoid spectral leakage.
    window.multiplyWithWindowingTable(fftPtrA, fftSize);
    window.multiplyWithWindowingTable(fftPtrB, fftSize);

    if (!bypassed) {
        // Perform the forward FFT.
        fft.performRealOnlyForwardTransform(fftPtrA, true);
        fft.performRealOnlyForwardTransform(fftPtrB, true);

        // Do stuff with the FFT data.
        processSpectrum(fftPtrA, fftPtrB, numBins);

        // Perform the inverse FFT.
        fft.performRealOnlyInverseTransform(fftPtrA);
        fft.performRealOnlyInverseTransform(fftPtrB);
    }

    // Apply the window again for resynthesis.
    window.multiplyWithWindowingTable(fftPtrA, fftSize);
    window.multiplyWithWindowingTable(fftPtrB, fftSize);

    // Scale down the output samples because of the overlapping windows.
    for (int i = 0; i < fftSize; ++i) {
        fftPtrA[i] *= windowCorrection;
        fftPtrB[i] *= windowCorrection;
    }

    // Add the IFFT results to the output FIFO.
    for (int i = 0; i < pos; ++i) {
        outputFifoA[i] += fftDataA[i + fftSize - pos];
        outputFifoB[i] += fftDataB[i + fftSize - pos];
    }
    for (int i = 0; i < fftSize - pos; ++i) {
        outputFifoA[i + pos] += fftDataA[i];
        outputFifoB[i + pos] += fftDataB[i];
    }
}

void CrosssynthProcessor::processSpectrum(float* dataA, float* dataB, int numBins)
{
    // The spectrum data is floats organized as [re, im, re, im, ...]
    // but it's easier to deal with this as std::complex values.
    auto* cdataA = reinterpret_cast<std::complex<float>*>(dataA);
    auto* cdataB = reinterpret_cast<std::complex<float>*>(dataB);

    
    float smoothMagnitudeB = 0;

    for (int i = 0; i < numBins; ++i) {
        // Usually we want to work with the magnitude and phase rather
        // than the real and imaginary parts directly.
        float magnitudeA = std::abs(cdataA[i]);
        float phaseA = std::arg(cdataA[i]);


        if (i < numBins - smoothingOrder) {
            smoothMagnitudeB = 0;
            for (int n = 0; n < smoothingOrder; n++) {
                smoothMagnitudeB += std::abs(cdataB[i + n]);
            }
            smoothMagnitudeB /= smoothingOrder;
        }
        // Convert magnitude and phase back into a complex number.
        cdataA[i] = std::polar(magnitudeA * smoothMagnitudeB / 2, phaseA);

    }
}