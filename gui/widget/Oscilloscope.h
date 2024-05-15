#pragma once
//==============================================================================
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace widget {
//==============================================================================
template<typename SampleType>
class Oscilloscope : public juce::Component
{
  using RingBuffer = dmt::dsp::data::RingAudioBuffer<SampleType>;

public:
  //==============================================================================
  Oscilloscope(RingBuffer& ringBuffer, int channel)
    : ringBuffer(ringBuffer)
    , lastFullyDrawnSample(0.0f, 0.0f)
  {
  }
  //==============================================================================
  void resized() override {}
  //==============================================================================
  void paint(juce::Graphics& g) noexcept override
  {
    const int width = getWidth();
    const int height = getHeight();
    const int halfHeight = height / 2;
    float samplesPerPixel = 1.0f;

    const int firstSampleIndex = ringBuffer.getOldestUnqueriedIndex();
    const int lastSampleIndex = ringBuffer.getNewestUnqueriedIndex();
    const int samplesToDraw = lastSampleIndex - firstSampleIndex;

    const int pixelToDraw = floor(samplesToDraw / samplesPerPixel);

    // Image move
    const int destX = 0 - pixelToDraw;
    image.moveImageSection(destX,   // destX
                           0,       // destY
                           0,       // srcX
                           0,       // srcY
                           width,   // width
                           height); // height

    // Generate path for new samples
    const int drawStartX = width - pixelToDraw;
    float pixelsPerSample = 1.0f / samplesPerPixel;

    lastFullyDrawnSample.applyTransform(
      juce::AffineTransform::translation(-samplesPerPixel, 0.0f));

    juce::Path path;
    path.startNewSubPath(lastFullyDrawnSample);

    for (int i = 0; i < samplesToDraw; ++i) {
      const float sample = ringBuffer.getSample(0, firstSampleIndex + i);
      const float x = (float)drawStartX + i * pixelsPerSample;
      const float y = halfHeight + sample * halfHeight;
      const auto point = juce::Point<float>(x, y);
      path.lineTo(point);
      if (i == samplesToDraw - 1) {
        lastFullyDrawnSample = point;
      }
    }

    // Draw path to image
    juce::Graphics imageGraphics(image);
    imageGraphics.setColour(juce::Colours::white);
    imageGraphics.strokePath(path, juce::PathStrokeType(1.0f));

    // Draw image to screen
    g.drawImage(image, 0, 0, width, height, 0, 0, width, height);
  }
  //==============================================================================
private:
  RingBuffer& ringBuffer;
  juce::Image image;
  juce::Point<float> lastFullyDrawnSample;
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscilloscope)
};
} // namespace widget
} // namespace gui
} // namespace dmt