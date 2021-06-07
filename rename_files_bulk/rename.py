import sys
import os
import random
import string


def main():
    path = './test/png/'
    with os.scandir(path) as files:
        for count, file in enumerate(files):
            if file.is_file():

                # printing letters
                letters = string.ascii_letters
                
                file_list = file.name.split(".")
                file_list[0] = ''.join(random.choice(letters) for i in range(10))
                

                dst = file_list[0] + "_" + str(count+1) +  "." + file_list[1]
                src = path + file.name
                dst = path + dst
                
                # rename() function will
                # rename all the files
                os.rename(src, dst)
# Driver Code
if __name__ == '__main__':
      
    # Calling main() function
    main()