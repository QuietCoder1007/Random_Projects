#include <iostream>
#include <random>
#include <string>
#include <cstring>
#include <unordered_set>
#include <sstream>

#ifdef _WIN32
#include <Windows.h>
#endif

std::string generateSecurePassword(const std::unordered_set<char> &characters, int passwordLength, double entropy);
void parseArguments(int argc, char *argv[], std::unordered_set<char> &characters, int &passwordLength);
bool is_digits(const std::string &str);
int checkPasswordStrength(const std::string &password);
std::unordered_set<char> getExpandedCharacterSet();
std::unordered_set<char> getPrintableASCIICharacterSet();
std::unordered_set<char> getDigitCharacterSet();
std::unordered_set<char> getLowercaseCharacterSet();
std::unordered_set<char> getUppercaseCharacterSet();
std::unordered_set<char> getLetterCharacterSet();

void copyToClipboard(const std::string &str)
{
#ifdef _WIN32
    OpenClipboard(0);
    EmptyClipboard();
    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, str.size());
    if (!hg)
    {
        CloseClipboard();
        return;
    }
    memcpy(GlobalLock(hg), str.c_str(), str.size());
    GlobalUnlock(hg);
    SetClipboardData(CF_TEXT, hg);
    CloseClipboard();
    GlobalFree(hg);
#else
    // Copy the data to the clipboard using the xclip utility
    std::string command = "echo '" + str + "' | xclip -selection clipboard";
    if (system(command.c_str()) != 0)
    {
        std::cerr << "Error copying to clipboard" << std::endl;
    }
#endif
}

const std::unordered_set<char> characters = getExpandedCharacterSet();
int main(int argc, char *argv[])
{
    int passwordLength;
    std::unordered_set<char> characters;
    parseArguments(argc, argv, characters, passwordLength);
    std::string password = generateSecurePassword(characters, passwordLength, INT16_MAX);

    // Calculate password strength
    int strength = checkPasswordStrength(password);

    copyToClipboard(password);

    return 0;
}

// Parse command - line arguments and return the desired character set and password length
void parseArguments(int argc, char *argv[], std::unordered_set<char> &characters, int &passwordLength)
{
    int min = 8;
    passwordLength = min;
    characters = getPrintableASCIICharacterSet();

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "-l" || arg == "--length")
        {
            if (i + 1 < argc)
            {
                try
                {
                    std::string temp = argv[i + 1];
                    if (!is_digits(temp))
                    {
                        throw std::invalid_argument("");
                    }
                    passwordLength = std::stoi(argv[i + 1]);
                    if (passwordLength < min)
                    {
                        throw std::out_of_range("");
                    }
                }
                catch (const std::invalid_argument &)
                {
                    std::cerr << "Error: invalid password length" << std::endl;
                    exit(1);
                }
                catch (const std::out_of_range &)
                {
                    std::cerr << "Error: password length to short! MUST be at least " << min << " characters" << std::endl;
                    exit(1);
                }

                i++;
            }
        }
        else if (arg == "-c" || arg == "--characters")
        {
            if (i + 1 < argc)
            {
                std::string characterSet = argv[i + 1];
                characters.clear();
                for (char c : characterSet)
                {
                    if (isprint(c))
                    {
                        characters.insert(c);
                    }
                }
                i++;
            }
        }
    }
}

bool is_digits(const std::string &str)
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}

// Check the strength of the specified password
int checkPasswordStrength(const std::string &password)
{
    int strength = 0;
    for (char c : password)
    {
        if (isupper(c))
            strength += 2;
        else if (islower(c))
            strength += 1;
        else if (isdigit(c))
            strength += 3;
        else
            strength += 5;
    }
    strength = strength * password.length() / 10;

    // Normalize password strength to be from 0 to 10
    strength = std::max(0, std::min(10, strength));

    return strength;
}

std::string generateSecurePassword(const std::unordered_set<char> &characters, int passwordLength, double entropy)
{
    std::string password;
    std::random_device rd;
    std::uniform_int_distribution<int> distribution(0, characters.size() - 1);
    bool hasUppercase = false;
    bool hasLowercase = false;
    bool hasDigit = false;
    bool hasSymbols = false;
    while (!hasUppercase || !hasLowercase || !hasDigit || !hasSymbols)
    {
        password.clear();
        hasUppercase = false;
        hasLowercase = false;
        hasDigit = false;
        hasSymbols = false;
        double totalEntropy = 0;

        char lastChar = 0;
        char lastType = 0;

        for (int i = 0; i < passwordLength; i++)
        {
            char c;
            do
            {
                c = *std::next(characters.begin(), distribution(rd));
            } while ((std::isalpha(c) == std::isalpha(lastChar) == lastType == 'A') ||
                     (std::tolower(c) == std::tolower(lastChar) == lastType == 'a') ||
                     (std::isdigit(c) == std::isdigit(lastChar) == lastType == 'd') ||
                     (std::isgraph(c) == std::isgraph(lastChar) == lastType == 's'));

            password += c;
            lastChar = c;

            if (std::isalpha(c))
            {
                lastType = 'A';
            }
            else if (std::tolower(c))
            {
                lastType = 'a';
            }
            else if (std::isdigit(c))
            {
                lastType = 'd';
            }
            else if (std::isgraph(c))
            {
                lastType = 's';
            }
            hasUppercase |= isupper(c);
            hasLowercase |= islower(c);
            hasDigit |= isdigit(c);
            hasSymbols |= isgraph(c);
            totalEntropy += log2(characters.size());
        }

        if (totalEntropy >= entropy)
        {
            break;
        }
    }
    return password;
}

// Return a character set that includes all printable ASCII characters, emoji, unicode symbols, and non-Latin characters
std::unordered_set<char> getExpandedCharacterSet()
{
    std::unordered_set<char> characters;
    // Insert all printable ASCII characters
    for (int i = 32; i <= 126; i++)
    {
        characters.insert(static_cast<char>(i));
    }
    // Insert emoji and unicode symbols
    for (int i = 0x1F600; i <= 0x1F64F; i++)
    {
        characters.insert(static_cast<char>(i));
    }
    for (int i = 0x2700; i <= 0x27BF; i++)
    {
        characters.insert(static_cast<char>(i));
    }
    // Insert non-Latin characters
    for (int i = 0x0400; i <= 0x052F; i++)
    {
        characters.insert(static_cast<char>(i));
    }
    for (int i = 0x1F00; i <= 0x1FFF; i++)
    {
        characters.insert(static_cast<char>(i));
    }
    for (int i = 0x2500; i <= 0x257F; i++)
    {
        characters.insert(static_cast<char>(i));
    }
    for (int i = 0x2E80; i <= 0x4DBF; i++)
    {
        characters.insert(static_cast<char>(i));
    }
    for (int i = 0x4E00; i <= 0x9FFF; i++)
    {
        characters.insert(static_cast<char>(i));
    }
    for (int i = 0xAC00; i <= 0xD7AF; i++)
    {
        characters.insert(static_cast<char>(i));
    }
    for (int i = 0xF900; i <= 0xFAFF; i++)
    {
        characters.insert(static_cast<char>(i));
    }
    return characters;
}

// Returns a character set containing all printable ASCII characters
std::unordered_set<char> getPrintableASCIICharacterSet()
{
    const char *characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789~!@#$%^&*-_=+:;<>,./";
    return std::unordered_set<char>(characters, characters + strlen(characters));
}

// Returns a character set containing all ASCII digits
std::unordered_set<char> getDigitCharacterSet()
{
    std::unordered_set<char> characters = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    return characters;
}

// Returns a character set containing all ASCII lowercase letters
std::unordered_set<char> getLowercaseCharacterSet()
{
    std::unordered_set<char> characters;
    for (int i = 97; i < 123; i++)
    {
        characters.insert(char(i));
    }
    return characters;
}

// Returns a character set containing all ASCII uppercase letters
std::unordered_set<char> getUppercaseCharacterSet()
{
    std::unordered_set<char> characters;
    for (int i = 65; i < 91; i++)
    {
        characters.insert(char(i));
    }
    return characters;
}

// Returns a character set containing all ASCII letters (uppercase and lowercase)
std::unordered_set<char> getLetterCharacterSet()
{
    std::unordered_set<char> characters = getLowercaseCharacterSet();
    auto uppercaseCharacters = getUppercaseCharacterSet();
    characters.insert(uppercaseCharacters.begin(), uppercaseCharacters.end());
    return characters;
}