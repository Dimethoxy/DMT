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

  using Settings = dmt::Settings;
  const float& size = Settings::Layout::size;

public:
  //==============================================================================
  Oscilloscope(RingBuffer& ringBuffer, int channel)
    : ringBuffer(ringBuffer)
    , currentSample(0.0f)
    , currentX(0.0f)
    , image(Image(PixelFormat::ARGB, 1, 1, true))
    , channel(channel)
    , rawSamplesPerPixel(1.0f)
    , amplitude(1.0f)
    , thickness(3.0f)
  {
  }
  //==============================================================================
  void resized() override { isResized = true; }
  //==============================================================================
  void paint(juce::Graphics& g) override
  {
    TRACE_COMPONENT();
    g.drawImageAt(image, 0, 0);
  }
  //==============================================================================
  void render()
  {
    TRACE_COMPONENT();

    if (isResized) {
      resizeImage();
      isResized = false;
    }

    renderNextFrame();
  }
  //==============================================================================
  void resizeImage()
  {
    auto newImage =
      Image(PixelFormat::ARGB, jmax(1, getWidth()), jmax(1, getHeight()), true);
    newImage.clear(newImage.getBounds(), juce::Colours::transparentBlack);
    // image = newImage;
  }
  //==============================================================================
  void renderNextFrame()
  {
    const int width = image.getWidth();
    const int height = image.getHeight();
    const int halfHeight = height / 2;
    float samplesPerPixel = rawSamplesPerPixel * size;

    const int bufferSize = ringBuffer.getNumSamples();
    const int readPosition = ringBuffer.getReadPosition(channel);
    const int samplesToRead = bufferSize - readPosition;

    const int maxSamplesToDraw = floor(samplesPerPixel * (float)width);
    const int samplesToDraw = jmin(samplesToRead, maxSamplesToDraw);
    const int firstSamplesToDraw = readPosition;

    const int pixelToDraw = samplesToDraw / samplesPerPixel;
    ringBuffer.incrementReadPosition(channel, samplesToDraw);

    // Image move
    auto newImage =
      Image(PixelFormat::ARGB, jmax(1, getWidth()), jmax(1, getHeight()), true);
    const int destX = 0 - pixelToDraw;
    newImage.moveImageSection(destX,      // destX
                              0,          // destY
                              0,          // srcX
                              0,          // srcY
                              width + 10, // width
                              height);    // height

    // Clear the new part of the image
    juce::Rectangle<int> clearRect(
      width - pixelToDraw + 10, 0, pixelToDraw, height);
    // newImage.clear(clearRect, juce::Colours::transparentBlack);

    // Generate path for new samples
    currentX = currentX - (int)currentX + width - pixelToDraw;
    float pixelsPerSample = 1.0f / samplesPerPixel;

    const float startY = halfHeight + currentSample * halfHeight * amplitude;
    const auto startPoint = Point(currentX, startY);

    juce::Path path;
    path.preallocateSpace(3 * samplesToDraw + 2 * 3);

    path.startNewSubPath(startPoint);

    for (int i = 0; i < samplesToDraw; ++i) {
      const int sampleIndex = firstSamplesToDraw + i;
      currentSample = ringBuffer.getSample(channel, sampleIndex);
      currentX += pixelsPerSample;
      const float y = halfHeight + currentSample * halfHeight * amplitude;
      const auto point = juce::Point<float>(currentX, y);
      path.lineTo(point);
    }

    PathStrokeType strokeType(thickness * size,
                              juce::PathStrokeType::JointStyle::mitered,
                              juce::PathStrokeType::EndCapStyle::butt);

    juce::Graphics imageGraphics(newImage);
    imageGraphics.setColour(juce::Colours::white);
    imageGraphics.strokePath(path, strokeType);
    // image = newImage;
  }
  //==============================================================================
  void setRawSamplesPerPixel(float newRawSamplesPerPixel) noexcept
  {
    this->rawSamplesPerPixel = newRawSamplesPerPixel;
  }
  //==============================================================================
  void setAmplitude(float newAmplitude) noexcept
  {
    this->amplitude = newAmplitude;
  }
  //==============================================================================
  void setThickness(float newThickness) noexcept
  {
    this->thickness = newThickness;
  }
  //==============================================================================
private:
  RingBuffer& ringBuffer;
  Image image;
  SampleType currentSample;
  CriticalSection imageLock;
  float currentX;
  const int channel;
  std::atomic<bool> isResized;
  //==============================================================================
  float rawSamplesPerPixel;
  float amplitude;
  float thickness;
  //==============================================================================
  std::condition_variable imageCondition;
  std::mutex imageMutex;
  std::atomic<bool> imageReady;
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscilloscope)
};
} // namespace widget
} // namespace gui
} // namespace dmt