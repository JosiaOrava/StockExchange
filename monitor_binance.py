import os
import time
import psutil
import subprocess

# Path to your executable file
exe_path = "X:\\Dev\\Trading\\stock.exe"

# Name of the Binance process (replace with the actual process name if different)
binance_process_name = "Binance.exe"

# Helper function to check if Binance is running
def is_binance_running():
    for process in psutil.process_iter(['name']):
        if process.info['name'] == binance_process_name:
            return True
    return False

# Monitor Binance and manage the executable
def monitor_binance():
    exe_process = None

    while True:
        if is_binance_running():
            if exe_process is None:
                # Launch the executable if Binance starts
               
                exe_process = subprocess.Popen([exe_path])
        else:
            if exe_process is not None:
                # Terminate the executable if Binance closes
           
                exe_process.terminate()
                exe_process = None

        time.sleep(2)  # Check every 2 seconds

if __name__ == "__main__":
    monitor_binance()
