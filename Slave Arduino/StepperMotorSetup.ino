#include <SparkFunAutoDriver.h>

void dSPINConfig(void)
{
  boardA.SPIPortConnect(&SPI);      // Before doing anything else, we need to
                                    //  tell the object which SPI port to use.
                                    //  Some devices may have more than one.
  boardA.configSyncPin(BUSY_PIN, 0);// BUSY pin low during operations;
                                    //  second paramter ignored.
  boardA.configStepMode(STEP_FS_4);   // 0 microsteps per step
  boardA.setMaxSpeed(5000);        // 10000 steps/s max
  boardA.setFullSpeed(5000);       // microstep below 10000 steps/s
  boardA.setAcc(500);            // accelerate at 10000 steps/s/s
  boardA.setDec(500);
  boardA.setSlewRate(SR_530V_us);   // Upping the edge speed increases torque.
  boardA.setOCThreshold(OC_2250mA);  // OC threshold 750mA
  boardA.setPWMFreq(PWM_DIV_1, PWM_MUL_2); // 31.25kHz PWM freq
  boardA.setOCShutdown(OC_SD_DISABLE); // don't shutdown on OC
  boardA.setVoltageComp(VS_COMP_DISABLE); // don't compensate for motor V
  boardA.setSwitchMode(SW_HARD_STOP);    // Switch is not hard stop
  boardA.setOscMode(INT_16MHZ_OSCOUT_16MHZ); // for boardA, we want 16MHz
                                    //  internal osc, 16MHz out.
  boardA.setAccKVAL(50);           // We'll tinker with these later, if needed.
  boardA.setDecKVAL(50);
  boardA.setRunKVAL(50);
  boardA.setHoldKVAL(10);           // This controls the holding current; keep it low.
  
  
}
