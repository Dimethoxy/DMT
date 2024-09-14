// #pragma once

// #include <JuceHeader.h>

// namespace dmt {
// namespace version {
// class Manager : public juce::Thread
// {
// public:
//   //============================================================================
//   Manager() = delete;
//   //============================================================================
//   Manager(const Manager& obj) = delete;
//   //============================================================================
//   ~Manager() { stopThread(1000); }
//   //============================================================================
//   static void initialize()
//   {
//     if (instance != nullptr) {
//       return;
//     }
//     instance = std::make_unique<Manager>();
//     instance->startThread();
//   }

// protected:
//   //============================================================================
//   void run() override
//   {
//     while (!threadShouldExit()) {
//       if (fetchLatestVersion()) {
//         break;
//       }
//       wait(1000);
//     }
//   }
//   //============================================================================
//   bool fetchLatestVersion()
//   {
//     // TODO: Implement this method and return true if successful
//     return false;
//   }
//   //============================================================================
// private:
//   static std::unique_ptr<Manager> instance;
// };
// } // namespace version
// } // namespace dmt