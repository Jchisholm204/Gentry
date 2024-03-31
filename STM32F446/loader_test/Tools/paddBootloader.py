BOOTLOADER_SIZE = 0x4000
BOOTLOADER_FILE = "../build/firmware.bin"

with open(BOOTLOADER_FILE, "rb") as f:
    bootloader = f.read()
bytestopad = BOOTLOADER_SIZE - len(bootloader)
padding = bytes(0xff for _ in range(bytestopad))
with open(BOOTLOADER_FILE, "ab") as f:
    f.write(bootloader + padding)
print(f"Padded bootloader to {BOOTLOADER_SIZE} bytes")