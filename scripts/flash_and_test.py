import serial
import time
import sys
from pyocd.core.helpers import ConnectHelper
from pyocd.flash.file_programmer import FileProgrammer

# Configuration
SERIAL_PORT = "/dev/ttyACM0"
BAUD_RATE = 115200

# Use the ELF output from your firmware build
ELF_PATH = "firmware/build/firmware.elf"


def flash_elf_firmware() -> bool:
    """
    Flash the ELF firmware to the STM32 using pyOCD + ST-LINK, then reset the target.
    Returns True on success, False on failure.
    """
    print(f"--- Flashing ELF: {ELF_PATH} ---")
    try:
        # Auto-detect and connect to the debug probe (ST-LINK).
        # target_override ensures pyOCD uses the correct MCU target.
        session = ConnectHelper.session_with_chosen_probe(target_override="stm32f446re")
        with session:
            # Program the ELF (includes sections and addresses from the ELF metadata).
            FileProgrammer(session).program(ELF_PATH)

            # Reset the board after flashing so the new firmware starts running.
            session.target.reset()

        print("--- ELF flash successful! ---")
        return True
    except Exception as e:
        print(f"ELF flash error: {e}")
        return False


def monitor_uart() -> bool:
    """
    Read UART output for a fixed window and declare PASS if a temperature line appears.
    Returns True if the expected output is observed, otherwise False.
    """
    print(f"--- Monitoring UART on {SERIAL_PORT} ---")
    try:
        with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=5) as ser:
            start_time = time.time()
            # Overall test window (seconds)
            while time.time() - start_time < 15:
                line = ser.readline().decode("utf-8", errors="ignore").strip()
                if not line:
                    continue

                print(f"[STM32]: {line}")

                # Success condition: firmware prints temperature output
                if "Temperature:" in line:
                    print("--- HIL TEST: PASSED ---")
                    return True

        print("--- HIL TEST: FAILED (Timeout waiting for expected output) ---")
        return False
    except Exception as e:
        print(f"UART error: {e}")
        return False


if __name__ == "__main__":
    if flash_elf_firmware():
        # Small delay to allow USB CDC / UART to enumerate and firmware to boot.
        time.sleep(1)
        if monitor_uart():
            sys.exit(0)

    sys.exit(1)
