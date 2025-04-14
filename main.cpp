#include <iostream>
#include <fstream>
#include <string>
#include "des.h"

int main() {
    std::string key = "KHONGTIMTHAYKHOA"; // Khóa mặc định DES
    std::string plain_text;
    
    // Đọc dữ liệu từ input.txt
    std::ifstream input_file("input.txt");
    if (input_file.is_open()) {
        std::getline(input_file, plain_text);
        input_file.close();
    } else {
        std::cerr << "Khong the mo input.txt" << std::endl;
        return 1;
    }
    
    // Chuyển đổi plain text sang hex
    std::string hex_plain = chuyen_string_sang_hex(plain_text);
    
    // Mã hóa
    std::string encrypted = "";
    // Xử lý từng khối 64bit (8 byte)
    for (size_t i = 0; i < hex_plain.length(); i += 16) {
        std::string block = hex_plain.substr(i, 16);
        // Nếu block không đủ 16 ký tự (8 byte), thêm "0"
        while (block.length() < 16) {
            block += "0";
        }
        encrypted += ma_hoa_des(block, key);
    }
    
    // Ghi kết quả mã hóa vào encrypted.txt
    std::ofstream encrypted_file("encrypted.txt");
    if (encrypted_file.is_open()) {
        encrypted_file << encrypted;
        encrypted_file.close();
    } else {
        std::cerr << "Khong the mo encrypted.txt" << std::endl;
        return 1;
    }
    
    // Giải mã
    std::string decrypted_hex = "";
    // Xử lý từng khối 64bit
    for (size_t i = 0; i < encrypted.length(); i += 16) {
        std::string block = encrypted.substr(i, 16);
        decrypted_hex += giai_ma_des(block, key);
    }
    
    // Chuyển đổi hex về string
    std::string decrypted = chuyen_hex_sang_string(decrypted_hex);

    // Loại bỏ các ký tự null ở cuối
    while (!decrypted.empty() && decrypted.back() == '\0') {
        decrypted.pop_back();
    }
    
    // Ghi kết quả giải mã vào decrypted.txt
    std::ofstream decrypted_file("decrypted.txt");
    if (decrypted_file.is_open()) {
        decrypted_file << decrypted;
        decrypted_file.close();
    } else {
        std::cerr << "Khong the mo decrypted.txt" << std::endl;
        return 1;
    }
    
    std::cout << "Da ma hoa input.txt ---> encrypted.txt ---giai ma---> decrypted.txt THANH CONG!" << std::endl;
    return 0;
}