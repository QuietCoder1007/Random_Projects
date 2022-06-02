#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <iostream>
#include <regex>
#include <string>
#include "getopt.h"
#include <ctime>
#include <random>

int randomNumber(int max) {
    return rand() % max;
}

std::string generatePassword(unsigned long int length, bool alpha_uppers, bool alpha_lowers, bool numbers, bool symbols)
{
    std::string alphanums = "";
    std::string uppers = "";
    std::string lowers = "";
    std::string nums = "";
    std::string _symbols = "";

    if (alpha_uppers)
    {
        alphanums += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        // uppers += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }

    if (alpha_lowers)
    {
        alphanums += "abcdefghijklmnopqrstuvwxyz";
        // lowers += "abcdefghijklmnopqrstuvwxyz";
    }

    if (numbers)
    {
        alphanums += "0123456789";
        // nums += "0123456789";
    }

    if (symbols)
    {
        alphanums += "~`!@#$%^&*()_-+={}[]|\\:;\"\'<>,.?/";
        // _symbols += "~`!@#$%^&*()_-+=<>,./?;:'{}[]|\"";
    }

    //std::cout << "alphanums: " << alphanums << std::endl;

    std::string password = "";
    srand(time(NULL));

    //std::cout << "Options: " << alphanums << std::endl;

    for (unsigned long int i = 0; i < length; i++)
    {

        int choice = randomNumber(alphanums.length());

        //std::cout << "Choice: " << choice << std::endl;

        while (!password.empty() && i - 1 > 0 && password[i - 1] == alphanums[choice]) {
            choice = randomNumber(alphanums.length());
        }

        password += alphanums[choice];
    }

    return password;
}

void toClipboard(const std::string& s) {
    OpenClipboard(0);
    EmptyClipboard();
    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size());
    if (!hg) {
        CloseClipboard();
        return;
    }
    memcpy(GlobalLock(hg), s.c_str(), s.size());
    GlobalUnlock(hg);
    SetClipboardData(CF_TEXT, hg);
    CloseClipboard();
    GlobalFree(hg);
}

int main(int argc, char** argv) {
    srand(time(NULL));
    
    bool alpha_uppers = false;
    bool alpha_lowers = false;
    bool numbers = false;
    bool symbols = false;
    unsigned long int length = 6;
    int opt;

    while ((opt = getopt(argc, argv, "L:AaNS")) > 0) {
        //std::cout << "Opt: " << char(opt) << std::endl;
        
        if (opt == '?') {
            /*std::cerr << " -L: Password length [>=6] \r\n -U: Alphabet upper [A-Z] \r\n -a: Alphabet Lower [a-z] \r\n -N: Numbers [0-9] \r\n -S: Letters [~`!@#$%^&*]" << std::endl;*/
            perror(" -L: Password length [>=6] \r\n -A: Alphabet upper [A-Z] \r\n -a: Alphabet Lower [a-z] \r\n -N: Numbers [0-9] \r\n -S: Letters [~`!@#$%^&*]");
            exit(1);
        }
        else if (argc == 3 && opt == 'L') {
            int temp = atoi(optarg);
            if (6 <= temp) {
                length = temp;
                alpha_uppers = true;
                alpha_lowers = true;
                numbers = true;
                symbols = true;
            }
            else {
                std::cout << "Please choose a password size GREATER than or EQUAL to 6! NOT " << temp << std::endl;
                exit(1);
            }
            break;
        }
        else if (argc >= 3) {
            switch (opt) {
                case 'L': {
                    int temp = atoi(optarg);
                    if (6 <= temp) {
                        length = temp;
                    }
                    else {
                        std::cout << "Please choose a password size GREATER than or EQUAL to 6! NOT "<< temp << std::endl;
                        exit(1);
                    }
                    break;
                }
                case 'A': {
                    alpha_uppers = true;
                    break;
                }
                case 'a': {
                    alpha_lowers = true;
                    break;
                }
                case 'N': {
                    numbers = true;
                    break;
                }
                case 'S': {
                    symbols = true;
                    break;
                }
            }
        }
    }
    //std::uniform_int_distribution<int> distribution(0, a - 1);
    std::string generated_password = generatePassword(length, alpha_uppers, alpha_lowers, numbers, symbols);

    toClipboard(generated_password);

    return 0;
}


