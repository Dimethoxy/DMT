#pragma once

#include "dmt/gui/panel/AbstractPanel.h"

namespace dmt {
namespace gui {
namespace window {

using GridSeparatorLayout = std::vector<float>;

// Layout is now a JUCE Component
class Layout : public juce::Component
{
  using AbstractPanel = dmt::gui::panel::AbstractPanel;
  using PanelPtr = std::shared_ptr<dmt::gui::panel::AbstractPanel>;
  using PanelList = std::vector<PanelPtr>;
  using PanelPosition = std::pair<size_t, size_t>;
  using PanelPositionList = std::vector<PanelPosition>;

public:
  // Constructor takes column and row separators
  Layout(const GridSeparatorLayout& _columnSeparators,
         const GridSeparatorLayout& _rowSeparators)
    : columnSeparators(_columnSeparators)
    , rowSeparators(_rowSeparators)
  {
  }

  virtual ~Layout() = default;

  // JUCE paint
  void paint(juce::Graphics& g) override
  {
    g.fillAll(juce::Colours::darkgrey);
    // Optionally draw grid lines here if desired
  }

  // JUCE resized
  void resized() override
  {
    auto bounds = getLocalBounds().toFloat();
    // Always treat 0.0 and 1.0 as implicit grid borders
    std::vector<float> cols = { 0.0f };
    cols.insert(cols.end(), columnSeparators.begin(), columnSeparators.end());
    cols.push_back(1.0f);

    std::vector<float> rows = { 0.0f };
    rows.insert(rows.end(), rowSeparators.begin(), rowSeparators.end());
    rows.push_back(1.0f);

    for (size_t i = 0; i < panels.size(); ++i) {
      auto [colIdx, rowIdx] = panelPositions[i];
      float x0 = cols[colIdx] * bounds.getWidth();
      float x1 = cols[colIdx + 1] * bounds.getWidth();
      float y0 = rows[rowIdx] * bounds.getHeight();
      float y1 = rows[rowIdx + 1] * bounds.getHeight();
      juce::Rectangle<int> panelBounds(static_cast<int>(x0),
                                       static_cast<int>(y0),
                                       static_cast<int>(x1 - x0),
                                       static_cast<int>(y1 - y0));
      panels[i]->setBounds(panelBounds);
    }
  }

  // Template method to add a panel of type T at (columnIndex, rowIndex)
  template<typename PanelType, typename... Args>
  void addPanel(size_t _columnIndex, size_t _rowIndex, Args&&... _args)
  {
    static_assert(std::is_base_of<AbstractPanel, PanelType>::value,
                  "PanelType must derive from Panel");
    auto panel = std::make_shared<PanelType>(std::forward<Args>(_args)...);
    panels.push_back(panel);
    panelPositions.emplace_back(_columnIndex, _rowIndex);
    // Add to JUCE component tree
    addAndMakeVisible(panel.get());
  }

protected:
  PanelList panels;
  // Stores (columnIndex, rowIndex) for each panel
  PanelPositionList panelPositions;
  GridSeparatorLayout columnSeparators;
  GridSeparatorLayout rowSeparators;
};