xss_payload = '<script>alert("XSS")</script>'
print(f"<input value='{xss_payload}' />")
