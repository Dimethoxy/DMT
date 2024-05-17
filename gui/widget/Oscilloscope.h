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
  using PixelFormat = juce::Image::PixelFormat;
  using Image = juce::Image;
  using Point = juce::Point<float>;

public:
  //==============================================================================
  Oscilloscope(RingBuffer& ringBuffer, int channel)
    : ringBuffer(ringBuffer)
    , currentSample(0.0f)
    , currentX(0.0f)
    , image(Image(PixelFormat::ARGB, 1, 1, true))
    , channel(channel)
  {
  }
  //==============================================================================
  void resized() override
  {
    Image oldImage(image);
    image =
      Image(Image::ARGB, jmax(1, getWidth() + 10), jmax(1, getHeight()), true);
    image.clear(image.getBounds(), Colours::transparentBlack);

    // Draw a mid line on the image
    juce::Graphics imageGraphics(image);
    imageGraphics.setColour(juce::Colours::white);
    imageGraphics.drawLine(
      0, getHeight() / 2, getWidth() + 10, getHeight() / 2, 3.0f);
  }
  //==============================================================================
  void paint(juce::Graphics& g) noexcept override
  {
    g.drawImageAt(image, 0, 0);
  }
  //==============================================================================
  void prepareToPaint()
  {
    const int width = getWidth();
    const int height = getHeight();
    const int halfHeight = height / 2;
    const float currentScale = 300.0f / (float)width;
    float samplesPerPixel = 200.0f * currentScale;

    const int bufferSize = ringBuffer.getNumSamples();
    const int readPosition = ringBuffer.getReadPosition(channel);
    const int samplesToRead = bufferSize - readPosition;

    const int maxSamplesToDraw = floor(samplesPerPixel * (float)width);
    const int samplesToDraw = jmin(samplesToRead, maxSamplesToDraw);
    const int firstSamplesToDraw = readPosition;

    const int pixelToDraw = samplesToDraw / samplesPerPixel;
    ringBuffer.incrementReadPosition(channel, samplesToDraw);

    // Image move
    const int destX = 0 - pixelToDraw;
    image.moveImageSection(destX,      // destX
                           0,          // destY
                           0,          // srcX
                           0,          // srcY
                           width + 10, // width
                           height);    // height

    // Clear the new part of the image
    juce::Rectangle<int> clearRect(
      width - pixelToDraw + 10, 0, pixelToDraw, height);
    image.clear(clearRect, juce::Colours::transparentBlack);

    // Generate path for new samples
    currentX = currentX - (int)currentX + width - pixelToDraw;
    float pixelsPerSample = 1.0f / samplesPerPixel;

    const float startY = halfHeight + currentSample * halfHeight;
    const auto startPoint = Point(currentX, startY);

    juce::Path path;
    path.startNewSubPath(startPoint);

    for (int i = 0; i < samplesToDraw; ++i) {
      const int sampleIndex = firstSamplesToDraw + i;
      currentSample = ringBuffer.getSample(channel, sampleIndex);
      currentX += pixelsPerSample;
      const float y = halfHeight + currentSample * halfHeight;
      const auto point = juce::Point<float>(currentX, y);
      path.lineTo(point);
    }

    PathStrokeType strokeType(3.0f,
                              juce::PathStrokeType::JointStyle::curved,
                              juce::PathStrokeType::EndCapStyle::rounded);

    juce::Graphics imageGraphics(image);
    imageGraphics.setColour(juce::Colours::white);
    imageGraphics.strokePath(path, strokeType);
  }

private:
  RingBuffer& ringBuffer;
  Image image;
  SampleType currentSample;
  float currentX;
  const int channel;
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscilloscope)
};
} // namespace widget
} // namespace gui
} // namespace dmt