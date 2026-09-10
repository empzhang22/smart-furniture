import socket
import tkinter as tk
from tkinter import messagebox


ESP_IP = "192.168.1.233"   # Change to your ESP32's IP
ESP_PORT = 4210


def send_command():
    try:
        time_on = int(time_on_entry.get())
        time_off = int(time_off_entry.get())

        if time_on <= 0 or time_off <= 0:
            raise ValueError

    except ValueError:
        messagebox.showerror(
            "Invalid Input",
            "time_on and time_off must be positive integers."
        )
        return

    message = f"{time_on},{time_off}"

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    sock.sendto(
        message.encode(),
        (ESP_IP, ESP_PORT)
    )

    sock.close()

    status_label.config(
        text=f"Sent: ON={time_on} ms, OFF={time_off} ms"
    )


root = tk.Tk()

root.title("ESP32 LED Controller")
root.geometry("350x220")


title_label = tk.Label(
    root,
    text="ESP32 LED Controller",
    font=("Arial", 16)
)
title_label.pack(pady=10)


time_on_frame = tk.Frame(root)
time_on_frame.pack(pady=5)

tk.Label(
    time_on_frame,
    text="Time ON (ms):",
    width=15
).pack(side=tk.LEFT)

time_on_entry = tk.Entry(time_on_frame)
time_on_entry.insert(0, "1000")
time_on_entry.pack(side=tk.LEFT)


time_off_frame = tk.Frame(root)
time_off_frame.pack(pady=5)

tk.Label(
    time_off_frame,
    text="Time OFF (ms):",
    width=15
).pack(side=tk.LEFT)

time_off_entry = tk.Entry(time_off_frame)
time_off_entry.insert(0, "1000")
time_off_entry.pack(side=tk.LEFT)


send_button = tk.Button(
    root,
    text="Send to ESP32",
    command=send_command
)
send_button.pack(pady=15)


status_label = tk.Label(
    root,
    text="No command sent yet."
)
status_label.pack()


root.mainloop()