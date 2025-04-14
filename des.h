#include <string>
#include <vector>

// các hàm chuyển đổi
std::string chuyen_hex_sang_binary(const std::string& hex_str);
std::string chuyen_binary_sang_hex(const std::string& binary_str);
std::string chuyen_string_sang_hex(const std::string& str);
std::string chuyen_hex_sang_string(const std::string& hex_str);

// Hàm xử lý bit
std::string hoan_vi(const std::string& key, const std::vector<int>& table);
std::string dich_trai_circular(const std::string& key, int shift);
std::string xor_binary(const std::string& a, const std::string& b);


// Các hàm chính trong thuật toán DES
std::string tao_khoa_con(const std::string& key, int round);
std::string tao_16_khoa_con(const std::string& key, std::vector<std::string>& subkeys);
std::string tinh_f(const std::string& R, const std::string& key);
std::string ma_hoa_des(const std::string& plain_text, const std::string& key);
std::string giai_ma_des(const std::string& cipher_text, const std::string& key);