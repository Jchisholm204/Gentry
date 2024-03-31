import sys
BOOTLOADER_SIZE = 0x4000
BOOTLOADER_FILE = sys.argv[1]

with open(BOOTLOADER_FILE, "rb") as f:
    bootloader = f.read()
bytestopad = BOOTLOADER_SIZE - len(bootloader)

print(f"Padding {BOOTLOADER_FILE} from {len(bootloader)} to {BOOTLOADER_SIZE} bytes")

padding = bytes(0xff for _ in range(bytestopad))

print(len(padding))


with open(BOOTLOADER_FILE, "ab") as f:
    f.write(padding)