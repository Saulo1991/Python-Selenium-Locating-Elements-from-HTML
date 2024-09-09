count = 0

while True:
    number = input('Type a number: ')
    count += 1
    
    if number == '':
        print('Empty field')
        continue
    
    intNumber = int(number)
    
    if intNumber < 0:
        print('End of program')
        break
    
print("Total of numbers: ", count)


