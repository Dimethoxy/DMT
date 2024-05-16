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

public:
  //==============================================================================
  Oscilloscope(RingBuffer& ringBuffer, int channel)
    : ringBuffer(ringBuffer)
    , lastFullyDrawnSample(0.0f, 0.0f)
    , image(Image(PixelFormat::ARGB, 1, 1, true))
    , channel(channel)
  {
  }
  //==============================================================================
  void resized() override
  {
    Image oldImage(image);
    image = Image(Image::RGB, jmax(1, getWidth()), jmax(1, getHeight()), true);
    image.clear(image.getBounds(), Colours::black);

    if (oldImage.isValid())
      image = oldImage.rescaled(image.getWidth(), image.getHeight());
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
    const int firstSamplesToDraw = readPosition; // bufferSize - samplesToDraw;

    const int pixelToDraw = samplesToDraw / samplesPerPixel;
    ringBuffer.incrementReadPosition(channel, samplesToDraw);

    if (pixelToDraw < 2) {
      // g.drawImage(image, 0, 0, width, height, 0, 0, width, height);
      return; // Draw ing less than 4 pixels is not worth it
    }

    // Image move
    const int destX = 0 - pixelToDraw;
    image.moveImageSection(destX,   // destX
                           0,       // destY
                           0,       // srcX
                           0,       // srcY
                           width,   // width
                           height); // height

    // Clear the new part of the image
    juce::Rectangle<int> clearRect(width - pixelToDraw, 0, pixelToDraw, height);
    image.clear(clearRect, juce::Colours::transparentBlack);

    // Generate path for new samples
    const int drawStartX = width - pixelToDraw;
    float pixelsPerSample = 1.0f / samplesPerPixel;

    lastFullyDrawnSample.applyTransform(
      juce::AffineTransform::translation(-pixelToDraw, 0.0f));

    juce::Path path;
    path.startNewSubPath(lastFullyDrawnSample);

    for (int i = 0; i < samplesToDraw; ++i) {
      const int sampleIndex = firstSamplesToDraw + i;
      const float sample = ringBuffer.getSample(channel, sampleIndex);
      const float x = (float)drawStartX + i * pixelsPerSample;
      const float y = halfHeight + sample * halfHeight;
      const auto point = juce::Point<float>(x, y);
      path.lineTo(point);
      if (i == samplesToDraw - 1) {
        lastFullyDrawnSample = point;
      }
    }

    juce::Graphics imageGraphics(image);
    imageGraphics.setColour(juce::Colours::white);
    imageGraphics.strokePath(path, juce::PathStrokeType(2.0f));
  }

private:
  RingBuffer& ringBuffer;
  Image image;
  juce::Point<float> lastFullyDrawnSample;
  int channel;
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscilloscope)
};
} // namespace widget
} // namespace gui
} // namespace dmt