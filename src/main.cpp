#include "components/installer/public/installer.h"

#if TEST_MAIN == 0
#ifdef TARGET_ESP32DEV
void setup()
{
  Installer::GetInstance()->Setup();
}

void loop()
{
  Installer::GetInstance()->Loop();
}
#elif defined(TARGET_NATIVE)
int main(int argc, char **argv)
{
  return 0;
}
#endif
#endif