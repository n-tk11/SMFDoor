import usb
from usb_setup import find_mcu_boards, McuBoard, PeriBoard
from time import sleep

devs = find_mcu_boards()

if len(devs) == 0:
    print("*** No practicum board found.")
    exit(1)

mcu = McuBoard(devs[0])
print("*** Practicum board found")
print("*** Manufacturer: %s" % \
        mcu.handle.getString(mcu.device.iManufacturer, 256))
print("*** Product: %s" % \
        mcu.handle.getString(mcu.device.iProduct, 256))
peri = PeriBoard(mcu)

while True:
    sw = peri.get_switch()
    print(sw);
    sleep(0.5);
