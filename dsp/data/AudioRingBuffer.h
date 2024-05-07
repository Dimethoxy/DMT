#include <JuceHeader.h>

namespace dmt {
namespace dsp {
namespace data {
//==============================================================================
template<typename SampleType>
class FifoBuffer : juce::AbstractFifo
{
  using BufferData = std::vector<std::vector<float>>;
  using ChannelData = std::vector<float>;

public:
  void addToFifo(const juce::AudioBuffer<SampleType>& target)
  {
    // Prepare to write
    const int numSamples = target.getNumSamples();
    int start1, size1, start2, size2;
    prepareToWrite(numSamples, start1, size1, start2, size2);

    // Block 1
    if (size1 > 0)
      for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        buffer.copyFrom(
          channel, start1, samples.getReadPointer(channel), size1);

    // Block 2
    if (size2 > 0)
      for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        buffer.copyFrom(
          channel, start2, samples.getReadPointer(channel, size1), size2);

    // Finish writing
    finishedWrite(size1 + size2);
  }

  void readFromFifo(juce::AudioBuffer<FloatType>& samples, int numSamples = -1)
  {
    // Prepare to read
    const int numSamples = samples.getNumSamples();
    int start1, size1, start2, size2;
    prepareToRead(readSamples, start1, size1, start2, size2);

    // Block 1
    if (size1 > 0)
      for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        samples.copyFrom(
          channel, 0, buffer.getReadPointer(channel, start1), size1);

    // Block 2
    if (size2 > 0)
      for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        samples.copyFrom(
          channel, size1, buffer.getReadPointer(channel, start2), size2);

    // Finish reading
    finishedRead(size1 + size2);
  }

private:
  juce::AudioBuffer<SampleType> buffer;
};
} // namespace data
} // namespace dsp
} // namespace dmt
