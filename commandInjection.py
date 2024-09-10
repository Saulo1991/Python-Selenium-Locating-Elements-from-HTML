import os
import subprocess

# Função para executar comandos e capturar a saída
def run_command(command):
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output, error = process.communicate()
    
    if process.returncode == 0:
        return output.decode('utf-8')
    else:
        return error.decode('utf-8')

# Executando vários comandos no sistema
commands = [
    "uname -a",               # Informações do kernel e do sistema operacional
    "whoami",                 # Usuário atual
    "id",                     # Informações do usuário (UID, GID, grupos)
    "hostname",               # Nome do host
    "cat /etc/passwd",        # Conteúdo do arquivo passwd (informações de usuários)
    "df -h",                  # Uso de disco
    "ifconfig || ip addr",    # Configuração de rede
    "netstat -tuln",          # Portas de rede abertas e escutando
    "ps aux",                 # Processos em execução
    "env",                    # Variáveis de ambiente
]

# Executando cada comando e mostrando a saída
for cmd in commands:
    print(f"Executing: {cmd}")
    output = run_command(cmd)
    print(output)
    print("=" * 50)

# Verificando se o usuário tem privilégios de root
print("Verificando privilégios de root...")
is_root = run_command("whoami").strip() == "root"

if is_root:
    print("O usuário tem privilégios de root!")
else:
    print("O usuário NÃO tem privilégios de root.")

# Tentando ler o arquivo de shadow (se possível)
if is_root:
    shadow_output = run_command("cat /etc/shadow")
    print("Conteúdo do arquivo /etc/shadow:")
    print(shadow_output)
else:
    print("Permissão negada para acessar /etc/shadow.")
