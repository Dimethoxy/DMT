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
    : AbstractFifo(bufferSize)
  {
    buffer.setSize(channels, bufferSize);
  }
  //============================================================================
  void addToFifo(const juce::AudioBuffer<SampleType>& target)
  {
    const int numSamples = target.getNumSamples();
    int firstBlockStart, firstBlockSize, secondBlockStart, secondBlockSize;

    prepareToWrite(numSamples,
                   firstBlockStart,
                   firstBlockSize,
                   secondBlockStart,
                   secondBlockSize);

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
      if (firstBlockSize > 0)
        buffer.copyFrom(channel,         // destChannel
                        firstBlockStart, // destStartSample
                        target,          // source
                        channel,         // sourceChannel
                        0,               // sourceStartSample
                        firstBlockSize); // numSamples
      if (secondBlockSize > 0)
        buffer.copyFrom(channel,          // destChannel
                        secondBlockStart, // destStartSample
                        target,           // source
                        channel,          // sourceChannel
                        firstBlockSize,   // sourceStartSample
                        secondBlockSize); // numSamples
    }

    finishedWrite(firstBlockSize + secondBlockSize);
  }
  //============================================================================
  void readFromFifo(juce::AudioBuffer<SampleType>& target)
  {
    const int numSamples = target.getNumSamples();
    int firstBlockStart, firstBlockSize, secondBlockStart, secondBlockSize;

    prepareToRead(numSamples,
                  firstBlockStart,
                  firstBlockSize,
                  secondBlockStart,
                  secondBlockSize);

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
      if (firstBlockSize > 0)
        target.copyFrom(channel,         // destChannel
                        0,               // destStartSample
                        buffer,          // source
                        channel,         // sourceChannel
                        firstBlockStart, // sourceStartSample
                        firstBlockSize); // numSamples
      if (secondBlockSize > 0)
        target.copyFrom(channel,          // destChannel
                        firstBlockSize,   // destStartSample
                        buffer,           // source
                        channel,          // sourceChannel
                        secondBlockStart, // sourceStartSample
                        secondBlockSize); // numSamples
    }

    finishedRead(firstBlockSize + secondBlockSize);
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
  int getNumSamples() const { return buffer.getNumSamples(); }
  //============================================================================
private:
  juce::AudioBuffer<SampleType> buffer;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FifoAudioBuffer)
};
} // namespace data
} // namespace dsp
} // namespace dmt
