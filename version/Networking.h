#pragma once
//==============================================================================
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace version {
//==============================================================================
constexpr auto SERVER = "https://api.dimethoxy.com/";
//==============================================================================
class Networking
{
public:
  //==============================================================================
  static inline auto createURL(const juce::String apiEndpoint)
  {
    return juce::URL(SERVER + apiEndpoint);
  }
  //==============================================================================
  static inline auto sendRequest(const juce::String apiEndpoint)
  {
    auto url = createURL(apiEndpoint);

    // Do some logging
    std::cout << "Sending request to: " << url.toString(true) << std::endl;

    // Send a GET request to the specified URL
    juce::String responseString = url.readEntireTextStream();

    // Test if the response is empty
    if (responseString.isEmpty()) {
      std::cerr << "Error: No response string received." << std::endl;
    }

    return responseString;
  }
  //==============================================================================
};
} // namespace version
} // namespace dmt