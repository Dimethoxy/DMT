// #pragma once
// //==============================================================================
// #include <JuceHeader.h>
// #include <configuration/Options.h>
// #include <configuration/Parameters.h>
// //==============================================================================
// namespace dmt {
// namespace configuration {
// //==============================================================================
// class Properties
// {
// public:
//   //============================================================================
//   Properties() = delete;
//   //============================================================================
//   Properties(const Properties& obj) = delete;
//   //============================================================================
//   ~Properties()
//   {
//     file.saveIfNeeded();
//     file.closeFiles();
//   }
//   //============================================================================
//   static std::shared_ptr<Properties> getInstance()
//   {
//     if (instance == nullptr) {
//       instance = std::make_shared<Properties>();
//     }
//     return instance;
//   }
//   //============================================================================
//   void initialize()
//   {
//     auto options = dmt::configuration::getOptions();
//     file.setStorageParameters(options);

//     auto settings = file.getUserSettings();
//     fallbackPropertySet = dmt::configuration::getPropertySet();
//     settings->setFallbackPropertySet(&fallbackPropertySet);
//   }
//   //============================================================================
// private:
//   static std::shared_ptr<Properties> instance;
//   juce::ApplicationProperties file;
//   juce::PropertySet fallbackPropertySet;
// };
// //==============================================================================
// } // namespace configuration
// } // namespace dmt