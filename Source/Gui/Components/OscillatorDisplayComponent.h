////==============================================================================
//
//#pragma once
//
//#include "../../Dsp/Synth/AnalogOscillator.h"
//#include "../../Utility/AppSettings.h"
//#include "../../Utility/ChainSettings.h"
//#include "../../Utility/Shadow.h"
//#include <JuceHeader.h>
//
////==============================================================================
//namespace dmt {
//namespace gui {
//namespace components {
////==============================================================================
//class OscillatorDisplayComponent
//  : public juce::Component
//  , public juce::Timer
//{
//  using Settings = dmt::AppSettings::OscillatorDisplay;
//  const int resolution = 256;
//
//public:
//  //============================================================================
//  OscillatorDisplayComponent(juce::AudioProcessorValueTreeState& apvts)
//    : apvts(apvts)
//    , chainSettings(apvts)
//  {
//    outerShadow.radius = Settings::outerShadowRadius;
//    outerShadow.colour = Settings::outerShadowColour;
//    innerShadow.radius = Settings::innerShadowRadius;
//    innerShadow.colour = Settings::innerShadowColour;
//    osc.setSampleRate((float)resolution + 1.0f);
//    osc.setFrequency(1.0f);
//    updateDisplay(chainSettings);
//    startTimerHz(60);
//  }
//  //============================================================================
//  void paint(juce::Graphics& g) override
//  {
//    const auto bounds = this->getLocalBounds().toFloat();
//
//    g.setColour(Settings::backgroundColour);
//    g.fillEllipse(bounds);
//
//    const auto borderSize = Settings::borderSize;
//    const auto borderBounds = bounds.reduced(Settings::margin);
//    const auto innerBounds = borderBounds.reduced(borderSize);
//    const auto outerCornerSize = Settings::outerCornerSize;
//    const auto innerCornerSize = Settings::innerCornerSize;
//
//    if (Settings::drawOuterShadow) {
//      juce::Path outerShadowPath;
//      outerShadowPath.addEllipse(borderBounds);
//      outerShadow.drawOuterForPath(g, outerShadowPath);
//    }
//
//    g.setColour(Settings::foregroundColour);
//    g.fillEllipse(innerBounds);
//
//    if (Settings::drawInnerShadow) {
//      juce::Path innerShadowPath;
//      innerShadowPath.addEllipse(innerBounds);
//      innerShadow.drawInnerForPath(g, innerShadowPath);
//    }
//
//    auto graphPath = getPath(borderBounds.reduced(Settings::graphSize / 2.0f));
//    auto integralPath = graphPath;
//    integralPath.closeSubPath();
//
//    g.setColour(Settings::integralColour);
//    g.fillPath(integralPath);
//
//    g.setColour(Settings::graphColor);
//    g.strokePath(graphPath, juce::PathStrokeType(Settings::graphSize));
//
//    g.setColour(Settings::borderColour);
//    g.drawEllipse(borderBounds.reduced(Settings::borderSize / 2.0f),
//                  Settings::borderSize);
//  }
//
//  //============================================================================
//private:
//  dmt::Shadow outerShadow;
//  dmt::Shadow innerShadow;
//  dmt::Shadow lineShadow;
//
//  dmt::dsp::synth::AnalogOscillator osc;
//  juce::dsp::LookupTable<float> table;
//  dmt::ChainSettings chainSettings;
//  juce::AudioProcessorValueTreeState& apvts;
//
//  //==============================================================================
//  void timerCallback()
//  {
//    dmt::ChainSettings newChainSettings(apvts);
//    if (isParametersChanged(newChainSettings)) {
//      chainSettings = newChainSettings;
//      osc.setWaveformType(chainSettings.waveformType);
//      osc.setDrive(chainSettings.oscDrive);
//      osc.setBias(chainSettings.oscBias);
//      osc.setBend(chainSettings.oscBend);
//      osc.setPwm(chainSettings.oscPwm);
//      osc.setSync(chainSettings.oscSync);
//      this->buildTable();
//      this->repaint();
//    }
//  }
//
//  void updateDisplay(dmt::ChainSettings& newChainSettings)
//  {
//    chainSettings = newChainSettings;
//    osc.setWaveformType(chainSettings.waveformType);
//    osc.setBend(chainSettings.oscBend);
//    osc.setPwm(chainSettings.oscPwm);
//    osc.setSync(chainSettings.oscSync);
//    this->buildTable();
//    this->repaint();
//  }
//
//  void buildTable()
//  {
//    osc.setPhase(0.0f);
//    table.initialise([&](size_t index) { return (float)osc.getNextSample(); },
//                     resolution);
//  }
//
//  bool isParametersChanged(dmt::ChainSettings& newChainSettings)
//  {
//    bool waveformChanged =
//      chainSettings.waveformType != newChainSettings.waveformType;
//    bool driveChanged = chainSettings.oscDrive != newChainSettings.oscDrive;
//    bool biasChanged = chainSettings.oscBias != newChainSettings.oscBias;
//    bool bendChanged = chainSettings.oscBend != newChainSettings.oscBend;
//    bool pwmChanged = chainSettings.oscPwm != newChainSettings.oscPwm;
//    bool syncChanged = chainSettings.oscSync != newChainSettings.oscSync;
//    return waveformChanged || driveChanged || biasChanged || bendChanged ||
//           pwmChanged || syncChanged;
//  }
//
//  //==============================================================================
//  juce::Path getPath(juce::Rectangle<float> bounds)
//  {
//    bounds.setY(bounds.getY() + (bounds.getHeight() / 10.0f));
//    bounds.setHeight(bounds.getHeight() - (bounds.getHeight() / 5.0f));
//
//    auto outerBounds = bounds;
//    bounds = bounds.reduced(bounds.getWidth() / 6.0f);
//
//    juce::Path path;
//
//    auto startX = bounds.getX();
//    auto startY = bounds.getY() + (bounds.getHeight() / 2.0f);
//    juce::Point<float> start(startX, startY);
//    path.startNewSubPath(start);
//
//    auto width = bounds.getWidth();
//
//    for (int i = 0; i < width; i++) {
//      auto x = bounds.getX() + i;
//      auto y = bounds.getY() + (bounds.getHeight() / 2.0f) -
//               (table[i / width * resolution] * bounds.getHeight() / 2.0f);
//      juce::Point<float> p(x, y);
//      path.lineTo(p);
//    }
//
//    auto endX = bounds.getX() + bounds.getWidth();
//    auto endY = bounds.getY() + (bounds.getHeight() / 2.0f);
//    juce::Point<float> end(endX, endY);
//    path.lineTo(end);
//
//    return path;
//  }
//
//  //============================================================================
//};
//} // namespace dmt
//} // namespace gui
//} // namespace components