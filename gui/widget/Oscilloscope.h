#pragma once
//==============================================================================
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace widget {
//==============================================================================
template<typename SampleType>
class Oscilloscope : public juce::Thread
{
  using RingBuffer = dmt::dsp::data::RingAudioBuffer<SampleType>;
  using Image = juce::Image;
  using Graphics = juce::Graphics;
  using String = juce::String;
  using Thread = juce::Thread;
  using PixelFormat = juce::Image::PixelFormat;
  using ReadWriteLock = juce::ReadWriteLock;

  using Settings = dmt::Settings;
  const float& size = Settings::Window::size;

public:
  //============================================================================
  Oscilloscope(RingBuffer& ringBuffer, const int channel)
    : Thread(String("Oscilloscope" + channel))
    , ringBuffer(ringBuffer)
    , channel(channel)
  {
    startThread();
  }
  //============================================================================
  ~Oscilloscope() override { stopThread(1000); }
  //============================================================================
  juce::Image getImage() const
  {
    const ScopedReadLock readLock(imageLock);
    return image.createCopy();
  }
  //============================================================================
  void setBounds(juce::Rectangle<int> newBounds)
  {
    resizeImage(newBounds.getWidth(), newBounds.getHeight());
    bounds = newBounds;
  }
  //============================================================================
  juce::Rectangle<int> getBounds() const { return bounds; }
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
protected:
  //============================================================================
  void run() override
  {
    while (!threadShouldExit()) {
      wait(10000);
      const ScopedWriteLock writeLock(imageLock);
      render();
    }
  }
  //============================================================================
  void resizeImage(const int width, const int height)
  {
    TRACER("Oscilloscope::resizeImage");
    // Resize the image
    const ScopedWriteLock writeLock(imageLock);
    image = Image(PixelFormat::ARGB, width + 10, height, true);

    // Draw a mid line on the image
    juce::Graphics imageGraphics(image);
    imageGraphics.setColour(juce::Colours::white);
    imageGraphics.drawLine(0, height / 2, width + 10, height / 2, 3.0f);
  }
  //==============================================================================
  void render()
  {
    TRACER("Oscilloscope::render");
    const int width = bounds.getWidth();
    const int height = bounds.getHeight();
    const int halfHeight = height / 2;
    const float currentScale = 300.0f / (float)width;
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

    const float startY = halfHeight + currentSample * halfHeight * amplitude;
    const auto startPoint = Point(currentX, startY);

    juce::Path path;
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

    juce::Graphics imageGraphics(image);
    imageGraphics.setColour(juce::Colours::white);
    imageGraphics.strokePath(path, strokeType);
  }

private:
  //============================================================================
  RingBuffer& ringBuffer;
  const int channel;
  juce::Rectangle<int> bounds = juce::Rectangle<int>(0, 0, 1, 1);
  Image image = Image(PixelFormat::ARGB, 1, 1, true);
  ReadWriteLock imageLock;
  //==============================================================================
  SampleType currentSample = 0.0f;
  float currentX = 0.0f;
  float rawSamplesPerPixel = 10.0f;
  float amplitude = 1.0f;
  float thickness = 3.0f;
  //============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscilloscope)
};
} // namespace widget
} // namespace gui
} // namespace dmt