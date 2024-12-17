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
    mpz_t e, d, m,tmp;
    mpz_inits(e, d, m,tmp, NULL);
    RSA(e, d, m);
    {
        char *k1= mpz_get_str(NULL,10, e);
        std::ofstream fout("Public_Key.txt");
        fout << k1<<" ";
        char* k2 = mpz_get_str(NULL, 10, m);
        fout << k2;
        fout.close();
        fout.open("Private_Key.txt");
        char* k3 = mpz_get_str(NULL, 10, d);
        fout << k3 << " ";
        char* k4 = mpz_get_str(NULL, 10, m);
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
    std::ofstream fout1;
    filename = "encryptoed.txt";
    fout1.open(filename);
    std::cout << content << std::endl;
    for (int i = 0; i < content.length() / rsaLength;i++) {
        std::string str = "";
        for (int j = i * rsaLength; j < (i + 1) * rsaLength; j++) {
        ;
            str += Table[content[j]];
        }
        std::cout << str << std::endl;
        mpz_t tmp,ans;
        mpz_inits(tmp,ans,NULL);
        mpz_set_str(tmp,str.c_str(), 2);
        quickpow(ans, tmp,e,m);
        char *k1=mpz_get_str(NULL, 2, ans);
        fout1 << k1<<" ";
        std::cout << k1 << std::endl;
      //  free(k1);
        mpz_clears(tmp, ans,NULL);
    }
    std::cout << std::endl;
    fout1.close();
    mpz_clears(e, d, m, NULL);
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
        while (str1.length() < rsaLength * 5) {
            str1 = "0" + str1;
        }
      //  std::cout << str1 << std::endl;
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