#include "components/installer/public/installer.h"

#ifdef TARGET_ESP32DEV
#if TEST_MAIN == 0
void setup()
{
  Installer::GetInstance()->Setup();
}

void loop()
{
  Installer::GetInstance()->Loop();
}
#endif
#elif defined(TARGET_NATIVE)
#if TEST_MAIN == 0
int main(int argc, char **argv)
{
  return 0;
}
#endif
#endif