//============================================================================

#pragma once

//==============================================================================

#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace version {

//==============================================================================
struct Utility
{
  using VersionArray = std::array<int, 3>;

  //============================================================================
  static inline juce::String parseResponseToVersionString(
    const juce::String& response) noexcept
  {
    int start = response.indexOf("version") + 8;
    auto secondHalf = response.substring(start);
    auto tokens = juce::StringArray::fromTokens(secondHalf, "\"", "");
    auto versionString = tokens[1];
    std::cout << "Version String: " << versionString << std::endl;
    return versionString;
  }
  //============================================================================
  static inline VersionArray parseVersionStringToArray(
    const juce::String& versionString) noexcept
  {
    auto versionArray = juce::StringArray::fromTokens(versionString, ".", "");
    VersionArray result;
    for (int i = 0; i < 3; i++) {
      result[static_cast<std::size_t>(i)] = versionArray[i].getIntValue();
    }
    std::cout << "- Major:" << result[0] << std::endl;
    std::cout << "- Minor:" << result[1] << std::endl;
    std::cout << "- Patch:" << result[2] << std::endl;
    return result;
  }
  //===========================================================================
  static inline int compareVersions(const VersionArray& version1,
                                    const VersionArray& version2)
  {
    for (std::size_t i = 0; i < version1.size(); ++i) {
      if (version1[i] < version2[i])
        return 2; // version2 is greater than version1
      else if (version1[i] > version2[i])
        return 1; // version1 is greater than version2
    }
    return 0; // Both versions are equal
  }
};
} // namespace version
} // namespace dmt