sql_payload = "' OR 1=1 --"
print(f"SELECT * FROM users WHERE username='{sql_payload}' AND password='password';")
