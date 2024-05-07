#include <JuceHeader.h>

namespace dmt {
namespace dsp {
namespace data {
//==============================================================================
template<typename SampleType>
class FifoAudioBuffer : juce::AbstractFifo
{
  using BufferData = std::vector<std::vector<float>>;
  using ChannelData = std::vector<float>;
  //============================================================================
public:
  FifoAudioBuffer(int channels, int bufferSize)
    : AbstractFifo(buffersize)
  {
    buffer.setSize(channels, buffersize);
  }
  //============================================================================
  void addToFifo(const juce::AudioBuffer<SampleType>& target)
  {
    // Prepare to write
    const int numSamples = target.getNumSamples();
    int start1, size1, start2, size2;
    prepareToWrite(numSamples, start1, size1, start2, size2);

    // Block 1
    if (size1 > 0) {
      for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        buffer.copyFrom(channel, start1, target.getReadPointer(channel), size1);
      }
    }

    // Block 2
    if (size2 > 0) {
      for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        buffer.copyFrom(
          channel, start2, target.getReadPointer(channel, size1), size2);
      }
    }
    // Finish writing
    finishedWrite(size1 + size2);
  }
  //============================================================================
  void readFromFifo(juce::AudioBuffer<SampleType>& target, int numSamples = -1)
  {
    // Prepare to read
    const int numSamples = target.getNumSamples();
    int start1, size1, start2, size2;
    prepareToRead(numSamples, start1, size1, start2, size2);

    // Block 1
    if (size1 > 0) {
      for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        target.copyFrom(
          channel, 0, buffer.getReadPointer(channel, start1), size1);
      }
    }

    // Block 2
    if (size2 > 0) {
      for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        target.copyFrom(
          channel, size1, buffer.getReadPointer(channel, start2), size2);

        // Finish reading
        finishedRead(size1 + size2);
      }
    }
  }
  //============================================================================
  void setSize(const int channels, const int newBufferSize)
  {
    buffer.setSize(channels, newBufferSize);
    setTotalSize(newBufferSize);
    reset();
  }
  //============================================================================
  void clear()
  {
    buffer.clear();
    reset();
  }
  //============================================================================
  int getNumChannels() const { return buffer.getNumChannels(); }
  //============================================================================
private:
  juce::AudioBuffer<SampleType> buffer;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FifoAudioBuffer)
};
} // namespace data
} // namespace dsp
} // namespace dmt
