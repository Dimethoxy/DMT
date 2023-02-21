#include "../MyProject/AhdEnvelope.h"
#include "CppUnitTest.h"
#include "Test.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MyProjectTests {
TEST_CLASS(AhdEnvelopeTests){ public :
                                TEST_METHOD(TestGetValue){ AhdEnvelope env;
AhdEnvelope::Parameters params;
params.attack = 0.5f;
params.hold = 0.2f;
params.decay = 0.3f;
params.attackScew = 1;
params.decayScew = -1;
env.setParameters(params);
env.setSampleRate(44100);
env.noteOn();
for (int i = 0; i < 44100; i++) {
  float value = env.getNextSample();
  Assert::IsTrue(value >= 0.0f && value <= 1.0f);
}
}
}
;
}
