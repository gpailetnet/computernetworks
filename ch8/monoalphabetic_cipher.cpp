#include <cmath>
#include <iostream>
#include <random>
#include <stddef.h>
#include <string>

constexpr int NUM_LETTERS{26};

bool is_letter(char c) {
    return (c <= 90 && c >= 65) || (97 <= c && c <= 122);
}

class Rand {
private:
    std::default_random_engine generator;
    std::uniform_real_distribution<> dist{0, 1};
public:
    Rand() {};
    double operator()() {
        return dist(generator);
    }
    // return number in uniform range[lower, upper)
    size_t range(size_t lower, size_t upper) {
        return (size_t) std::floor(((upper - lower) * dist(generator)) + lower);
    }
};

void swap(char* arr, size_t index1, size_t index2) {
    char temp = arr[index1];
    arr[index1] = arr[index2];
    arr[index2] = temp;
}


// a cipher in this case is most concisely expressed as a permutation of 0-25
// this can be done in an 26-char array
void permute(char* arr, size_t len, Rand& r) {
    for (size_t left_index = 0; left_index < len; left_index ++) {
        size_t swap_index{r.range(left_index, len)};
        swap(arr, left_index, swap_index);
    }
}

// TODO: this assumes we're using the regular alphabetic case, but what if we did unicode?
// What if we did a mapping from numbers to a charset?  That would be insightful...
class MonoAlphabeticCipher {
private:
    char key[NUM_LETTERS]; // map plaintext to ciphertext
    char reverse_key[NUM_LETTERS]; // map ciphertext to plaintext
    Rand r;
public:
    MonoAlphabeticCipher() {
        for (int i = 0; i < NUM_LETTERS; i++) {
            key[i] = i;
        }
        permute(key, NUM_LETTERS, r);
        for (int plain_index = 0; plain_index < NUM_LETTERS; plain_index++) {
            int cipher_index = key[plain_index];
            reverse_key[cipher_index] = plain_index;
        }
    }
    void encrypt(std::string& plaintext) {
        for (char& c: plaintext) {
            if (is_letter(c)) {
                if (c < 97) {
                    c = key[c - 65] + 65;
                }
                else {
                    c = key[c - 97] + 97;
                }
            }
        }
    }

    void decrypt(std::string& ciphertext) {
        for (char& c: ciphertext) {
            if (is_letter(c)) {
                if (c < 97) {
                    c = reverse_key[c - 65] + 65;
                }
                else {
                    c = reverse_key[c - 97] + 97;
                }
            }
        }
    }

    char* get_key() {
        return key;
    }
};


int main() {
    MonoAlphabeticCipher cipher;
    std::string s{"bob, i love you. Alice"};
    std::cout << "Plaintext is: " << s << std::endl;
    cipher.encrypt(s);
    std::cout << "Ciphertext is: " << s << std::endl;
    cipher.decrypt(s);
    std::cout << "Decrypted Ciphertext is: " << s << std::endl;

    return 0;
}