#include <iostream>
#include "RSA.hpp"
#include<fstream>
#include<map>
#include<bitset>
#include<sstream>
#pragma comment(lib, "libgmpxx.dll.a")
#pragma comment(lib, "libgmp.dll.a")
int rsaLength = 0;
std::map<char, std::string> Table;
std::map<std::string, char> Table1;
void init() {
    // 将空格映射为 "00000"
    Table[' '] = "00000";
    Table1["00000"] = ' ';
    // 将字母 'A' 到 'Z' 映射为 1 到 26 的 5 位二进制形式
    for (char ch = 'A'; ch <= 'Z'; ++ch) {
        int num = ch - 'A' + 1;  // 计算字母对应的数字 (A -> 1, B -> 2, ..., Z -> 26)
        std::bitset<5> binary(num);  // 将数字转换为 5 位二进制
        Table[ch] = binary.to_string();  // 将二进制字符串存入表中
        Table1[binary.to_string()] = ch;
     //   std::cout << Table[ch] << std::endl;
    }
}
void encrypt() {
    std::cout << "执行加密操作..." << std::endl;
    mpz_t e, d, n,tmp;
    mpz_inits(e, d, n,tmp, NULL);
    RSA(e, d,n);
    {
        char *k1= mpz_get_str(NULL,10, e);
        std::ofstream fout("Public_Key.txt");
        fout << k1<<" ";
        char* k2 = mpz_get_str(NULL, 10, n);
        fout << k2;
        fout.close();
        fout.open("Private_Key.txt");
        char* k3 = mpz_get_str(NULL, 10, d);
        fout << k3 << " ";
        char* k4 = mpz_get_str(NULL, 10, n);
        fout << k4;
       // free(k1); free(k2); free(k3); free(k4);
            fout.close();
        std::cout << "请在当前文件夹下查看公钥私钥" << std::endl;
    }
    std::string filename;
    std::cout << "请输入文件名称，不要带空格" << std::endl;
    std::cout << "演示用文件的名词为nuaa.txt" << std::endl;
    
    std::cout << "请输入文件名" << std:: endl;
    std::cin >> filename;
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cerr << "无法打开文件" << std::endl;
        exit(0);
    }
    std::stringstream buffer;
    buffer << fin.rdbuf(); // 将文件流的内容复制到字符串流中
    std::string content = buffer.str(); // 将字符串流转换为字符串
    fin.close();
    int offset = content.length() % rsaLength;
    for (int i = 0; i < (rsaLength-offset)%rsaLength; i++) {
        content +=" ";
    }
   // std::cout << content.length() << std::endl;
    std::ofstream fout1;
    filename = "encryptoed.txt";
    fout1.open(filename);
    std::cout << "文件内容为：" << std::endl;
    std::cout << content << std::endl;
    for (int i = 0; i < content.length() / rsaLength;i++) {
        std::string str = "";
        for (int j = i * rsaLength; j < (i + 1) * rsaLength; j++) {
            str += Table[content[j]];
         //   std::cout << (int)(content[j]) << "\n";
        }
        mpz_t tmp,ans;
        mpz_inits(tmp,ans,NULL);
        mpz_set_str(tmp,str.c_str(), 2);
        quickpow(ans, tmp,e,n);
        char *k1=mpz_get_str(NULL, 2, ans);
      //  free(k1);
        fout1<<k1<<std::endl;
        mpz_clears(tmp, ans,NULL);
    }
    std::cout << std::endl;
    fout1.close();
    mpz_clears(e, d, n, NULL);
    std::cout << "请再次打开本程序以完成解密操作"<<std::endl;;
    return;
}

void decrypt() {
    std::cout << "请输入文件名称" << std::endl;
    std::cout << "默认文件名为encryptoed.txt" << std::endl;
    std::string str;
    std::cin >> str;
    std::fstream fin(str);
    if (!fin.is_open()) {
        std::cout << "文件打开失败！" << std::endl;
        exit(0);
    }
    std::cout << "正在读取本机上的密钥" << std::endl;
    std::cout << "执行解密操作..." << std::endl;
    std::fstream fin1("Private_Key.txt");
    if (fin1.fail()) {
        std::cerr << "还未进行加密的操作" << std::endl;
        exit(0);
    }
    char d[1024] , m[1024];
    fin1 >> d >> m;
    mpz_t dt, mt,ans,c;
    mpz_inits(dt, mt,ans,c, NULL);
    mpz_set_str(dt, d, 10);
    mpz_set_str(mt, m, 10);
    while (fin >> str) {
    //    std::cout << str << std::endl;
        mpz_set_str(c, str.c_str(), 2);
        quickpow(ans,c ,dt, mt);
    //    gmp_printf("ans==%Zd\n %Zd %Zd" , &ans,&dt,&mt);
        char *tmp1 = mpz_get_str(NULL,2,ans);
        std::string str1 = (tmp1);
     //   std::cout << str1 << std::endl;
        while (str1.length() < rsaLength * 5) {
            str1 = "0" + str1;
        }
     //   std::cout << str1 << std::endl;
        for (int i = 0; i < rsaLength; i++) {
            int p = i * 5;
            std::string sub = str1.substr(p, 5);
         //   std::cout << sub << std::endl;
            std::cout << Table1[sub];
        }
    }
    std::cout << std::endl;
    fin.close(); fin1.close();
    mpz_clears(dt, mt,c, ans,NULL);
}

int main() {
    init();
    char choice = '\0';
    // 询问RSA分组位数
    std::cout << "本程序对单个字母进行5位定长编码，使用1024位素数进行加密" << std::endl;
    
    // 询问是加密还是解密
    std::cout << "请选择操作：(E) 加密 或 (D) 解密: ";
    std::cin >> choice;
    std::cout << "请输入按多少个字母进行分组: ";
    std::cin >> rsaLength;
    while (rsaLength >200||rsaLength<1) {
        if(rsaLength > 200)std::cout << "长度太长，请重新输入";
        if (rsaLength < 1)std::cout << "长度太短，请重新输入";
        std::cout << std::endl;
        std::cin >> rsaLength;
    }
    
    
    // 根据用户的选择执行相应的操作
    if (choice == 'E' || choice == 'e') {
        // 如果选择了加密
        std::cout << "您选择了加密。" << std::endl;
        encrypt(); // 调用加密函数
    }
    else if (choice == 'D' || choice == 'd') {
        // 如果选择了解密
        std::cout << "您选择了解密。" << std::endl;
        decrypt(); // 调用解密函数
    }
    else {
        std::cerr << "无效的选择，请输入 E 或 D。" << std::endl;
        return 1; // 返回错误码
    }

    std::cout << "操作完成。" << std::endl;
    return 0;
}
/*
10101010011111011100100101111111111000001110000000111111111000110100000010011001111000010011011111111011111000101010011001000100011100001010001001011010110101001100000111010100000100000000100101010100000110011100001100101010010000000000100001111001111010110010001010101011011111111111001001100100101011110001100110000010011010110001110110100001100000011010111000111000101011010001111001000001001100010110101100111110110010111010001101000000111010001111000000000110011110000010000100000000100001111110011101101010100000110100111010100011111010011001001011000011101111111101000100000111010001011001001100110101101101101011110001001101011100111011111000101001001000011100011010111011111101100111110111101000101001000111101100001011000110110101110001000100001110000111001011011010111101111010011101011100110000011101101001001111100001010101001010111110100100010010010101001100111000001111011101111001001100011111001000011010000110110011011101111001000000111000111111001010010000110100001011001010001101001111110010000101111001010001110001001000111101100010110101100011100111010100101010111010010011000001001011011101001011000100000010000101100001101100110010000010011110111011110001111000010111010010011011000011010111001000000101000110001110110111001000101010001001111101111001110000001111001001001100000101101100100010111001011100111010011011010000011010111110010010010010100001001100001010011100011100010110111111010001010011001010001111000111011000110001100110110001011101000000101001010101110010011010111010110100000111111110101110001011111111000101010000110101111011001100000010010010111000000100011011011110110111000110100111011101011010011001001001110010011111110101100111100001100100110010101101101101101100011111011001100100001011000100111001110000111001011111011011000111110001010111011100010101011100010111011010001100001110001010001001101001111000000110110011000110100100100110111011011100011100000111000011010110001101101001101001011101110101111100000010101011110110111100101111001001001101010101100001000011110010110101011110101000111101
*/