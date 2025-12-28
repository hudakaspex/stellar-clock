#include <Arduino.h>
#include "display/display_utils.h"

DisplayUtils displayUtils;

void setup()
{
  Serial.begin(115200);
  Serial.println("Stellar Clock Starting...");
  displayUtils.initDisplay();
}

void loop()
{
  displayUtils.renderDisplay();
}