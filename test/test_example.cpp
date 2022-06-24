#include <Arduino.h>
#include <unity.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void test_example(void)
{
  bool trueBool = true;

  TEST_ASSERT_EQUAL(3, 3);
}

void test_example2(void)
{
  bool trueBool = true;

  TEST_ASSERT_EQUAL(3, 3);
}

int runUnityTests(void)
{
  UNITY_BEGIN();

  RUN_TEST(test_example);
  RUN_TEST(test_example2);

  return UNITY_END();
}

int main(int argc, char **argv)
{
  return runUnityTests();
}

void setup()
{
  delay(2000);

  runUnityTests();
}

void loop()
{
}