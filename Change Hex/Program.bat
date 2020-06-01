START "C:\Users\David\Desktop\test wake_me\Change Hex" ChangeHex.exe
cd C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility
ST-LINK_CLI.exe -c UR SWD -HardRst
ST-LINK_CLI.exe -c UR SWD -P "C:\Users\David\Desktop\test wake_me\Change Hex\AAA_VA.hex"
ST-LINK_CLI.exe -c UR SWD -HardRst
