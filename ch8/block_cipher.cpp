

#include <cassert>
#include <iostream>
#include <random>

const int NUM_TABLE_ENTRIES{256};
const int NUM_TABLES{8};

class Rand {
private:
    std::default_random_engine eng;
    std::uniform_real_distribution<double> dist{0, 1};
public:
    double operator()() {
        return dist(eng);
    }
    size_t range(size_t lower, size_t upper) {
        // std::cout << "Lower: " << lower << ", Upper: " << upper << std::endl;
        // size_t val{}
        return (size_t)(lower + ((upper - lower) * dist(eng)));
    }
};

void swap(uint8_t arr[], size_t index1, size_t index2) {
    // std::cout << "Swapping indices: " << index1 << " and " << index2 << std::endl;
    uint8_t tmp{arr[index1]};
    arr[index1] = arr[index2];
    arr[index2] = tmp;
}

void permute(uint8_t arr[], size_t len, Rand& r) {
    for (size_t index = 0; index < len; index++) {
        size_t swap_index = r.range(index, len);
        swap(arr, index, swap_index);
    }
}

class Table {
private:
    uint8_t mapping[NUM_TABLE_ENTRIES];
public:
    // generate a random table
    Table() {
        for (int i = 0; i < NUM_TABLE_ENTRIES; i++) {
            mapping[i] = i;
        }
    }
    void permute_table(Rand& rand) {
        permute(mapping, NUM_TABLE_ENTRIES, rand);
    }
    uint8_t operator[](uint8_t index) {
        return mapping[index];
    }
};

// a scrambler takes in a 64-bit number and scrambles it so each bit ends up
// in the same place
class Scrambler {
private:
    uint8_t mapping[64];
public:
    Scrambler() {
        for (int i = 0; i < 64; i++) {
            mapping[i] = i;
        }
    }
    void permute_table(Rand& rand) {
        permute(mapping, 64, rand);
    }
    uint64_t operator()(uint64_t input) {
        // scrambled bits to regular form
        uint64_t output{0};
        for (int i = 0; i < 64; i++) {
            output |= (((input >> i) & 1) << mapping[i]);
        }
        return output;
    }
};


// The big idea: cipher that encodes groups of 64 bits into 64 bits
// This uses 8 8-bit tables, a scrambler, and a specification for the
// Number of rounds within

class BlockCipher {
private:
    // help to generate the permutations for the table
    Rand rand;
    // table that maps 8 bits to 8 bits; 256 bytes total
    Table table[NUM_TABLES];

    Scrambler scrambler;
    int num_rounds;

    uint64_t round(uint64_t input) {
        uint8_t* plaintext_ptr{(uint8_t*) &input};

        uint64_t ciphertext;
        uint8_t* ciphertext_ptr{(uint8_t*) &ciphertext};
        // go through all 8 tables for each 'section' of the plaintext
        for (int i = 0; i < NUM_TABLES; i++) {
            ciphertext_ptr[i] = table[i][plaintext_ptr[i]];
        }
        // return ciphertext;
        return scrambler(ciphertext);
    }
    
public:
    BlockCipher(int num_rounds): num_rounds{num_rounds} {
        assert(num_rounds > 0);

        for (int i = 0; i < NUM_TABLES; i++) {
            table[i].permute_table(rand);
        }
        scrambler.permute_table(rand);
    }
    // encode using all 8 tables
    uint64_t operator()(uint64_t input) {
        for (int i = 0; i < num_rounds; i++) {
            input = round(input); 
        }

        return input;
    }
};


int main() {  
    Rand rand;
    BlockCipher cipher{10};

    std::string s{"HTTP/1.0 abcdefg"};
    std::string s2{"abcdefghHTTP/1.0"};

    uint64_t input = *(uint64_t*) s.data();
    uint64_t output = cipher(input);
    for (int i = 0; i < NUM_TABLES; i++) {
        std::cout << ((int)((uint8_t*)&output)[i]) << std::endl;
    }

    std::cout << std::endl;

    uint64_t input2 = *((uint64_t*) &(s2.data()[8]));
    uint64_t output2 = cipher(input2);
    for (int i = 0; i < NUM_TABLES; i++) {
        std::cout << ((int)((uint8_t*)&output2)[i]) << std::endl;
    }

    assert(output == output2);

    return 0;
}