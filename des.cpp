#include "des.h"
#include <sstream>
#include <iomanip>

// Các bảng hoán vị và thay thế sử dụng trong DES
const std::vector<int> IP = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

const std::vector<int> IP_INV = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

const std::vector<int> PC1 = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

const std::vector<int> PC2 = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

const std::vector<int> E = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
};

const std::vector<int> P = {
    16, 7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
};

const std::vector<std::vector<std::vector<int>>> S_BOXES = {
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    },
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    },
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
    },
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
    },
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
    },
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
    },
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
    },
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
    }
};

const std::vector<int> key_shift = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

// Hàm chuyển đổi
std::string chuyen_hex_sang_binary(const std::string& hex_str) {
    std::string binary = "";
    for (char c : hex_str) {
        int val;
        if (c >= '0' && c <= '9') val = c - '0';
        else if (c >= 'A' && c <= 'F') val = c - 'A' + 10;
        else if (c >= 'a' && c <= 'f') val = c - 'a' + 10;
        else continue;

        for (int i = 3; i >= 0; i--) {
            binary += ((val >> i) & 1) ? "1" : "0";
        }
    }
    return binary;
}

std::string chuyen_binary_sang_hex(const std::string& binary_str) {
    std::string hex = "";
    for (size_t i = 0; i < binary_str.length(); i += 4) {
        std::string chunk = binary_str.substr(i, 4);
        int val = 0;
        for (int j = 0; j < 4; j++) {
            if (chunk[j] == '1') val += (1 << (3 - j));
        }
        if (val < 10) hex += ('0' + val);
        else hex += ('A' + val - 10);
    }
    return hex;
}

std::string chuyen_string_sang_hex(const std::string& str) {
    std::stringstream ss;
    for (char c : str) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)c;
    }
    return ss.str();
}

std::string chuyen_hex_sang_string(const std::string& hex_str) {
    std::string result;
    for (size_t i = 0; i < hex_str.length(); i += 2) {
        std::string byte = hex_str.substr(i, 2);
        char c = (char)strtol(byte.c_str(), nullptr, 16);
        result += c;
    }
    return result;
}

// Hàm xử lý bit
std::string hoan_vi(const std::string& key, const std::vector<int>& table) {
    std::string result = "";
    for (int pos : table) {
        result += key[pos - 1];
    }
    return result;
}

std::string dich_trai_circular(const std::string& key, int shift) {
    std::string result = key;
    for (int i = 0; i < shift; i++) {
        char first = result[0];
        result = result.substr(1) + first;
    }
    return result;
}

std::string xor_binary(const std::string& a, const std::string& b) {
    std::string result = "";
    for (size_t i = 0; i < a.length(); i++) {
        result += (a[i] == b[i]) ? "0" : "1";
    }
    return result;
}

// Các hàm chính trong thuật toán DES
std::string tao_khoa_con(const std::string& key, int round) {
    // Áp dụng PC1
    std::string perm_key = hoan_vi(key, PC1);
    
    // Chia thành 2 phần
    std::string left = perm_key.substr(0, 28);
    std::string right = perm_key.substr(28, 28);
    
    // Thực hiện dịch trái theo quy tắc
    for (int i = 0; i < round; i++) {
        left = dich_trai_circular(left, key_shift[i]);
        right = dich_trai_circular(right, key_shift[i]);
    }
    
    // Ghép lại và áp dụng PC2
    std::string combined = left + right;
    std::string subkey = hoan_vi(combined, PC2);
    
    return subkey;
}

std::string tao_16_khoa_con(const std::string& key, std::vector<std::string>& subkeys) {
    std::string binary_key = chuyen_hex_sang_binary(key);
    
    // Nếu key không đủ 64 bit thì thêm "0"
    while (binary_key.length() < 64) {
        binary_key = "0" + binary_key;
    }
    
    subkeys.clear();
    for (int round = 0; round < 16; round++) {
        subkeys.push_back(tao_khoa_con(binary_key, round + 1));
    }
    
    return binary_key;
}

std::string tinh_f(const std::string& R, const std::string& key) {
    // Mở rộng R từ 32 bit thành 48 bit
    std::string expanded_r = hoan_vi(R, E);
    
    // XOR với khóa
    std::string xored = xor_binary(expanded_r, key);
    
    // Áp dụng Sbox
    std::string s_box_output = "";
    for (int i = 0; i < 8; i++) {
        std::string chunk = xored.substr(i * 6, 6);
        int row = (chunk[0] - '0') * 2 + (chunk[5] - '0');
        int col = (chunk[1] - '0') * 8 + (chunk[2] - '0') * 4 + (chunk[3] - '0') * 2 + (chunk[4] - '0');
        int val = S_BOXES[i][row][col];
        
        // Chuyển đổi thành binary 4 bit
        for (int j = 3; j >= 0; j--) {
            s_box_output += ((val >> j) & 1) ? "1" : "0";
        }
    }
    
    // Áp dụng hoán vị P
    std::string result = hoan_vi(s_box_output, P);
    
    return result;
}

std::string ma_hoa_des(const std::string& plain_text, const std::string& key) {
    // Chuyển đổi plain text sang binary
    std::string binary_text = chuyen_hex_sang_binary(plain_text);
    
    // Đảm bảo text có đủ 64 bit (nếu không đủ thì thêm "0")
    while (binary_text.length() < 64) {
        binary_text = "0" + binary_text;
    }
    
    // Áp dụng hoán vị đầu IP
    std::string permuted = hoan_vi(binary_text, IP);
    
    // Chia thành 2 phần L và R
    std::string L = permuted.substr(0, 32);
    std::string R = permuted.substr(32, 32);
    
    // Tạo 16 khóa con
    std::vector<std::string> subkeys;
    tao_16_khoa_con(key, subkeys);
    
    // 16 vòng lặp
    for (int i = 0; i < 16; i++) {
        std::string temp = R;
        std::string f_result = tinh_f(R, subkeys[i]);
        R = xor_binary(L, f_result);
        L = temp;
    }
    
    // Ghép R và L (chú ý là đổi thứ tự)
    std::string combined = R + L;
    
    // Áp dụng hoán vị cuối IP^-1
    std::string cipher = hoan_vi(combined, IP_INV);
    
    // Chuyển về hex
    return chuyen_binary_sang_hex(cipher);
}

std::string giai_ma_des(const std::string& cipher_text, const std::string& key) {
    // Chuyển đổi cipher text sang binary
    std::string binary_text = chuyen_hex_sang_binary(cipher_text);
    
    // Đảm bảo text có đủ 64 bit
    while (binary_text.length() < 64) {
        binary_text = "0" + binary_text;
    }
    
    // Áp dụng hoán vị đầu IP
    std::string permuted = hoan_vi(binary_text, IP);
    
    // Chia thành 2 phần L và R
    std::string L = permuted.substr(0, 32);
    std::string R = permuted.substr(32, 32);
    
    // Tạo 16 khóa con (để giải mã, chúng ta sẽ sử dụng theo thứ tự ngược lại)
    std::vector<std::string> subkeys;
    tao_16_khoa_con(key, subkeys);
    
    // 16 vòng lặp với thứ tự khóa ngược lại
    for (int i = 0; i < 16; i++) {
        std::string temp = R;
        std::string f_result = tinh_f(R, subkeys[15 - i]);
        R = xor_binary(L, f_result);
        L = temp;
    }
    
    // Ghép R và L
    std::string combined = R + L;
    
    // Áp dụng hoán vị cuối IP^-1
    std::string plain = hoan_vi(combined, IP_INV);
    
    // Chuyển về hex
    return chuyen_binary_sang_hex(plain);
}