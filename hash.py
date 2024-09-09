import hashlib

def convert_text_to_sha1(text):
    digest = hashlib.sha1(text.encode()).hexdigest()
    
    return digest

def main():
    user_sha1 = input('Enter the SHA1 to assess: ')
    cleaned_user_sha1 = user_sha1.strip().lower()
    
    try:
        with open('./senhas.txt') as pw:
            for line in pw:
                password = line.strip()
                converted_sha1 = convert_text_to_sha1(
                    password
                    )
                
                if cleaned_user_sha1 == converted_sha1:
                    print(f'Password Found: {password}')
                    return
    except FileNotFoundError:
        print("O arquivo './senhas.txt' não foi encontrado.")
    except IOError as e:
        print(f"Ocorreu um erro ao ler o arquivo: {e}")
    except Exception as e:
        print(f'General error: {e}')
            
    print('Could not find the password')

if __name__ == '__main__':
    main()