#ifdef UNIT_TEST_ENABLE

#include "application.h"
#include "unitTest.h"

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// Tests
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

test(sanity)
{
  int x=3;
  int y=3;
  assertEqual(x,y);
}

test(insanity)
{
  int x=3;
  int y=3;
  assertNotEqual(x,y);
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// Call Start to Init Tests
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  bool flag;

  int start(String input)
  {
    flag = true;
    return 1;
  }

  void setup()
  {
    Particle.function("RunUnitTests", start);
    flag = false;
    Serial.begin(9600);
  }

  void loop()
  {
    if (flag)
      Test::run();
  }

#endif
