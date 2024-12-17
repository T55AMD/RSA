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
    // ���ո�ӳ��Ϊ "00000"
    Table[' '] = "00000";
    Table1["00000"] = ' ';
    // ����ĸ 'A' �� 'Z' ӳ��Ϊ 1 �� 26 �� 5 λ��������ʽ
    for (char ch = 'A'; ch <= 'Z'; ++ch) {
        int num = ch - 'A' + 1;  // ������ĸ��Ӧ������ (A -> 1, B -> 2, ..., Z -> 26)
        std::bitset<5> binary(num);  // ������ת��Ϊ 5 λ������
        Table[ch] = binary.to_string();  // ���������ַ����������
        Table1[binary.to_string()] = ch;
     //   std::cout << Table[ch] << std::endl;
    }
}
void encrypt() {
    std::cout << "ִ�м��ܲ���..." << std::endl;
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
        std::cout << "���ڵ�ǰ�ļ����²鿴��Կ˽Կ" << std::endl;
    }
    std::string filename;
    std::cout << "�������ļ����ƣ���Ҫ���ո�" << std::endl;
    std::cout << "��ʾ���ļ�������Ϊnuaa.txt" << std::endl;
    
    std::cout << "�������ļ���" << std:: endl;
    std::cin >> filename;
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cerr << "�޷����ļ�" << std::endl;
        exit(0);
    }
    std::stringstream buffer;
    buffer << fin.rdbuf(); // ���ļ��������ݸ��Ƶ��ַ�������
    std::string content = buffer.str(); // ���ַ�����ת��Ϊ�ַ���
    fin.close();
    int offset = content.length() % rsaLength;
    for (int i = 0; i < (rsaLength-offset)%rsaLength; i++) {
        content +=" ";
    }
   // std::cout << content.length() << std::endl;
    std::ofstream fout1;
    filename = "encryptoed.txt";
    fout1.open(filename);
    std::cout << "�ļ�����Ϊ��" << std::endl;
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
    std::cout << "���ٴδ򿪱���������ɽ��ܲ���"<<std::endl;;
    return;
}

void decrypt() {
    std::cout << "�������ļ�����" << std::endl;
    std::cout << "Ĭ���ļ���Ϊencryptoed.txt" << std::endl;
    std::string str;
    std::cin >> str;
    std::fstream fin(str);
    if (!fin.is_open()) {
        std::cout << "�ļ���ʧ�ܣ�" << std::endl;
        exit(0);
    }
    std::cout << "���ڶ�ȡ�����ϵ���Կ" << std::endl;
    std::cout << "ִ�н��ܲ���..." << std::endl;
    std::fstream fin1("Private_Key.txt");
    if (fin1.fail()) {
        std::cerr << "��δ���м��ܵĲ���" << std::endl;
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
    // ѯ��RSA����λ��
    std::cout << "������Ե�����ĸ����5λ�������룬ʹ��1024λ�������м���" << std::endl;
    
    // ѯ���Ǽ��ܻ��ǽ���
    std::cout << "��ѡ�������(E) ���� �� (D) ����: ";
    std::cin >> choice;
    std::cout << "�����밴���ٸ���ĸ���з���: ";
    std::cin >> rsaLength;
    while (rsaLength >200||rsaLength<1) {
        if(rsaLength > 200)std::cout << "����̫��������������";
        if (rsaLength < 1)std::cout << "����̫�̣�����������";
        std::cout << std::endl;
        std::cin >> rsaLength;
    }
    
    
    // �����û���ѡ��ִ����Ӧ�Ĳ���
    if (choice == 'E' || choice == 'e') {
        // ���ѡ���˼���
        std::cout << "��ѡ���˼��ܡ�" << std::endl;
        encrypt(); // ���ü��ܺ���
    }
    else if (choice == 'D' || choice == 'd') {
        // ���ѡ���˽���
        std::cout << "��ѡ���˽��ܡ�" << std::endl;
        decrypt(); // ���ý��ܺ���
    }
    else {
        std::cerr << "��Ч��ѡ�������� E �� D��" << std::endl;
        return 1; // ���ش�����
    }

    std::cout << "������ɡ�" << std::endl;
    return 0;
}
/*
10101010011111011100100101111111111000001110000000111111111000110100000010011001111000010011011111111011111000101010011001000100011100001010001001011010110101001100000111010100000100000000100101010100000110011100001100101010010000000000100001111001111010110010001010101011011111111111001001100100101011110001100110000010011010110001110110100001100000011010111000111000101011010001111001000001001100010110101100111110110010111010001101000000111010001111000000000110011110000010000100000000100001111110011101101010100000110100111010100011111010011001001011000011101111111101000100000111010001011001001100110101101101101011110001001101011100111011111000101001001000011100011010111011111101100111110111101000101001000111101100001011000110110101110001000100001110000111001011011010111101111010011101011100110000011101101001001111100001010101001010111110100100010010010101001100111000001111011101111001001100011111001000011010000110110011011101111001000000111000111111001010010000110100001011001010001101001111110010000101111001010001110001001000111101100010110101100011100111010100101010111010010011000001001011011101001011000100000010000101100001101100110010000010011110111011110001111000010111010010011011000011010111001000000101000110001110110111001000101010001001111101111001110000001111001001001100000101101100100010111001011100111010011011010000011010111110010010010010100001001100001010011100011100010110111111010001010011001010001111000111011000110001100110110001011101000000101001010101110010011010111010110100000111111110101110001011111111000101010000110101111011001100000010010010111000000100011011011110110111000110100111011101011010011001001001110010011111110101100111100001100100110010101101101101101100011111011001100100001011000100111001110000111001011111011011000111110001010111011100010101011100010111011010001100001110001010001001101001111000000110110011000110100100100110111011011100011100000111000011010110001101101001101001011101110101111100000010101011110110111100101111001001001101010101100001000011110010110101011110101000111101
*/