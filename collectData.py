import os

info = {
    "os": os.name,
    "platform": os.uname(),
    "cwd": os.getcwd(),
    "env": os.environ
}

print(info)
