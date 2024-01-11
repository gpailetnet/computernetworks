#include <iostream>
#include <string>
#include <unordered_set>

constexpr int NUM_LETTERS{26};

bool is_letter(char c) {
    return (c <= 90 && c >= 65) || (97 <= c && c <= 122);
}

char shift_char(char c, int shift) {
    if (is_letter(c)) {
        // need to add because of potential negative modulo
        if (c <= 90 && c >= 65) {
            return ((c - 65 + shift + NUM_LETTERS) % NUM_LETTERS) + 65;
        }
        else {
            return ((c - 97 +shift + NUM_LETTERS) % NUM_LETTERS) + 97;
        }
    }
    return c;
}

char cipher_char(char c, int key) {
    return shift_char(c, key);
}

char plaintext_char(char c, int key) {
    return shift_char(c, -key);
}

void encrypt(std::string& plaintext, int key) {
    for (char& c: plaintext) {
        c = cipher_char(c, key);
    }
}

void decrypt(std::string& ciphertext, int key) {
    for (char& c: ciphertext) {
        c = plaintext_char(c, key);
    }
}

// the caesar cipher is an example of a symmetric key system
int main() {
    std::string plaintext{"bob, i love you. Alice"};
    int key{3};
    std::cout << "Plaintext message is: " << plaintext << std::endl;
    encrypt(plaintext, key);
    std::cout << "Ciphertext message is: " << plaintext << std::endl;
    decrypt(plaintext, key);
    std::cout << "Decrypted ciphertext is: " << plaintext << std::endl;


    return 0;
}