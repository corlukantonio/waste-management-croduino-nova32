#include "components/installer/public/installer.h"

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