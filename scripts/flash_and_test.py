import serial
import time
import sys
from pyocd.core.helpers import ConnectHelper
from pyocd.flash.file_programmer import FileProgrammer

# Cấu hình
SERIAL_PORT = '/dev/ttyACM0' 
BAUD_RATE = 115200
# Đường dẫn file ELF thay vì BIN
ELF_PATH = 'firmware/build/firmware.elf' 

def flash_elf_firmware():
    print(f"--- Đang nạp file ELF: {ELF_PATH} ---")
    try:
        # Tự động tìm kiếm và kết nối với ST-LINK
        session = ConnectHelper.session_with_chosen_probe(target_override="stm32f446re")
        with session:
            # Nạp file ELF
            FileProgrammer(session).program(ELF_PATH)
            # Reset board sau khi nạp
            session.target.reset()
            print("--- Nạp ELF thành công! ---")
            return True
    except Exception as e:
        print(f"Lỗi khi nạp ELF: {e}")
        return False

def monitor_uart():
    print(f"--- Đang kiểm tra UART tại {SERIAL_PORT} ---")
    try:
        with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=5) as ser:
            start_time = time.time()
            while time.time() - start_time < 15: # Tăng timeout lên một chút
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                if line:
                    print(f"[STM32]: {line}")
                    if "Temperature:" in line:
                        print("--- HIL TEST: PASSED ---")
                        return True
        print("--- HIL TEST: FAILED (Timeout) ---")
        return False
    except Exception as e:
        print(f"Lỗi UART: {e}")
        return False

if __name__ == "__main__":
    if flash_elf_firmware():
        time.sleep(1) # Chờ board ổn định
        if monitor_uart():
            sys.exit(0)
    sys.exit(1)
