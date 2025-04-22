#pragma once
//==============================================================================
namespace dmt {
namespace version {
//==============================================================================
struct Info
{
  using VersionArray = std::array<int, 3>;

  static inline std::unique_ptr<VersionArray> current;
  static inline std::unique_ptr<VersionArray> latest;
  static inline std::unique_ptr<bool> isLatest;
  static inline std::unique_ptr<juce::String> downloadLink;
  static inline bool wasPopoverShown = false;
};
} // namespace version
} // namespace dmt