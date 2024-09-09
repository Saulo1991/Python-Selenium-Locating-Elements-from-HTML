import dns.resolver


res = dns.resolver.Resolver()
file = open(r'C:\Users\Notebook\python\wordlist.txt', 'r')
subdomains = file.read().splitlines()

target = 'panelinhadaaninha.com'

for subdomain in subdomains:
    try:
        sub_target = subdomain + '.' + target
        result = res.resolve(sub_target, 'A')
        for ip in result:
            print(f'Target -> {sub_target} ::: IP -> {ip}')
    except:
        pass