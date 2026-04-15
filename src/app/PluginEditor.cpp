#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
PluginEditor::PluginEditor(PluginProcessor& p)
  : dmt::app::AbstractPluginEditor(
      p,
      "Neutrino",
      500,
      270,
      [&p](dmt::gui::window::Layout& layout) {
        layout.addPanel<dmt::gui::panel::NeutrinoPanel<float>>(
          0, 0, 1, 1, p.apvts, p.oscilloscopeBuffer);
      })
{
}

//==============================================================================
PluginEditor::~PluginEditor() {}
