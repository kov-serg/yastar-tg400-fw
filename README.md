# yastar-tg400-fw

utils to restore [TG400](https://www.yeastar.ru/products/yeastar-tg400/) after fatal update on new version
  https://www.yeastar.com/firmware-download/
> ... before using the TG V3 firmware 91.2.0.4 to upgrade, please make sure
> your TG gateway’s firmware version is 91.X.X.X or its SN starts with 331; using 
> this firmware on a non-V3 gateway will result in system crash...

unpack.sh
  downloads [firmware for TG400](https://www.yeastar.com/download/TG200_TG400/53.18.0.46.zip) unpack and prepare image S34ML01G2-TG400-firmware-53.18.0.46.bin
  for [writing](https://ru.aliexpress.com/item/XGECU-100-TL866II-ICSP-FLASH-EEPROM-MCU-NAND-22/32857147523.html) into [S34ML01G2 flash chip](https://www.cypress.com/file/218306/download)

