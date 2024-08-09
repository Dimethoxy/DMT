#pragma once
//==============================================================================
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace version {
//==============================================================================
static const juce::String SERVER_URL = "https://api.dimethoxy.com/";
//==============================================================================
class Networking
{
public:
  //==============================================================================
  static juce::URL createURL(const juce::String& apiEndpoint)
  {
    return juce::URL("https://api.dimethoxy.com/" + apiEndpoint);
  }
  //==============================================================================
  juce::String sendRequest(const juce::String& apiEndpoint)
  {
    juce::URL url = createURL(apiEndpoint);

    // Send a GET request to the specified URL
    juce::String responseString = url.readEntireTextStream();

    return responseString;
  }
  //==============================================================================
};
} // namespace version
} // namespace dmt