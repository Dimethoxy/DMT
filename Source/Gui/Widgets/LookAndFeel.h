///*
//  ==============================================================================
//
//    LookAndFeel.h
//    Created: 10 Mar 2023 7:02:10pm
//    Author:  Lunix
//
//  ==============================================================================
//*/
//
//#pragma once
//
//#include <JuceHeader.h>
//
//namespace dmt {
//namespace gui {
//namespace widgets {
//class LookAndFeel : public juce::LookAndFeel_V4
//{
//  using Settings = dmt::AppSettings;
//
//  void drawLinearSlider(juce::Graphics& g,
//                        int x,
//                        int y,
//                        int width,
//                        int height,
//                        float sliderPos,
//                        float minSliderPos,
//                        float maxSliderPos,
//                        const juce::Slider::SliderStyle style,
//                        juce::Slider& slider)
//  {
//    // Precalculate some values
//    auto bounds = slider.getLocalBounds();
//    auto yReduction = bounds.getHeight() * 0.2;
//    bounds.reduce(0.0f, yReduction);
//    g.setColour(juce::Colours::white);
//    // g.drawRect(bounds, 1.0f); //Debug only
//
//    // Calculate the Rail
//    auto startPoint = juce::Point<float>(
//      bounds.getCentreX(),
//      bounds.getY() +
//        (2.0f * Settings::LinearSlider::tumbSize) * Settings::size);
//    auto endPoint = juce::Point<float>(
//      bounds.getCentreX(),
//      bounds.getBottom() -
//        (2.0f * Settings::LinearSlider::tumbSize) * Settings::size);
//    auto railWidth = Settings::size * 5.0f;
//    auto railHeight = endPoint.getY() - startPoint.getY();
//    auto corner = Settings::size * 2.5f;
//    auto rail = juce::Rectangle<float>(startPoint.getX() - railWidth / 2.0f,
//                                       startPoint.getY(),
//                                       railWidth,
//                                       railHeight);
//
//    // Draw the rail
//    g.setColour(Settings::Colours::accentAlpha);
//    g.fillRoundedRectangle(rail, corner);
//
//    // Precalculate some values for the thumb
//    auto sliderValue = slider.getValue();
//    auto sliderRange = slider.getMaximum() - slider.getMinimum();
//    auto sliderFactor = (sliderValue - slider.getMinimum()) / sliderRange;
//    auto thumbWidth = Settings::LinearSlider::tumbSize * railWidth *
//                      (slider.isMouseButtonDown() ? 2.9f : 2.5f);
//    auto thumbPos = endPoint.getY() - (sliderFactor * railHeight);
//    auto thumbPoint = juce::Point<float>(startPoint.getX(), thumbPos);
//    auto thumbBounds =
//      juce::Rectangle<float>(thumbPoint.getX() - thumbWidth / 2.0f,
//                             thumbPoint.getY() - thumbWidth / 2.0f,
//                             thumbWidth,
//                             thumbWidth);
//    auto thumbCorePadding = thumbBounds.reduced(Settings::size * 2.0f);
//
//    // Precalculate some values for the value rail
//    auto valueRailHeight = endPoint.getY() - thumbBounds.getY();
//    auto valueRail =
//      juce::Rectangle<float>(startPoint.getX() - railWidth / 2.0f,
//                             thumbPoint.getY() - thumbWidth / 2.0f,
//                             railWidth,
//                             valueRailHeight);
//
//    // Draw the value rail
//    g.setColour(Settings::Colours::accent);
//    g.fillRoundedRectangle(valueRail, corner);
//
//    // Draw the thumb
//    g.setColour(Settings::Colours::font);
//    g.fillEllipse(thumbBounds);
//    g.setColour(Settings::Colours::background);
//    g.fillEllipse(thumbCorePadding);
//  }
//};
//} // namespace widgets
//} // namespace gui
//} // namespace dmt