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
  //=============================================================p================
  static inline auto sendRequest(const juce::String apiEndpoint)
  {
    auto url = createURL(apiEndpoint); // createURL(apiEndpoint);

    // Do some logging
    std::cout << "Sending request to: " << url.toString(true) << std::endl;

    // Send a GET request to the specified URL
    auto responseString = url.readEntireTextStream(false);

    // Test if the response is empty
    if (responseString.isEmpty()) {
      std::cerr << "Failed to fetch data from: " << url.toString(true)
                << std::endl;
      return juce::String();
    }

    // Everything is fine, log the response
    std::cout << "Response: " << responseString << std::endl;

    return responseString;
  }
  //==============================================================================
};
} // namespace version
} // namespace dmt