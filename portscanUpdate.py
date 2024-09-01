import socket

ports = [21, 22, 80, 8000, 443, 445, 3306, 25]

for port in ports:
    cliente = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    cliente.settimeout(0.1)
    codigo = cliente.connect_ex(('exemplo.com', port))
    if codigo == 0:
        print(f"Porta {port} aberta")
    else:
        print(f"Porta {port} fechada")
    cliente.close() 