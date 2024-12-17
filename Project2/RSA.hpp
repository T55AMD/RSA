#pragma once
#include"rand.hpp"

bool exgcd(mpz_t& a, mpz_t& b, mpz_t& x, mpz_t& y) {
    /*
     * ��� b Ϊ 0
     */
    if (mpz_cmp_ui(b, 0) == 0) {
        /*
         * ��� a Ϊ 1
         */
        if (mpz_cmp_ui(a, 1) == 0) {
            mpz_set_ui(x, 1);  // ���� x Ϊ 1
            mpz_set_ui(y, 0);  // ���� y Ϊ 0
            return true;  // ���� true
        }
        else {
            return false;  // ���򣬷��� false
        }
    }

    /*
     * ��ʼ���ݹ���õ���ʱ����
     */
    mpz_t a1, b1, q, r, tmp, z;
    mpz_inits(a1, b1, q, r, tmp, z, NULL);

    /*
     * Ϊ��һ�εݹ���׼��
     */
    mpz_set(a1, b);  // a1 = b
    mpz_tdiv_r(r, a, b);  // r = a % b
    mpz_set(b1, r);  // b1 = a % b

    /*
     * �ݹ����
     */
    bool flag = exgcd(a1, b1, x, y);
  //  gmp_printf("x=%Zd y=%Zd\n", &x, &y);

    /*
     * ����ݹ���óɹ�
     */
    if (flag) {
        /*
         * ���ݵݹ������� x �� y
         */
        mpz_set(z, x);  // z = x
        mpz_set(x, y);  // x = y
        mpz_tdiv_q(q, a, b);  // q = a / b
        mpz_mul(tmp, q, x);  // tmp = q * x
        mpz_sub(y, z, tmp);  // y = z - tmp
    }

    /*
     * �����ʱ�����Է�ֹ�ڴ�й©
     */
    mpz_clears(a1, b1, q, r, tmp, z, NULL);

    return flag;  // ���صݹ���õĽ��
}
void RSA(mpz_t& e, mpz_t& d, mpz_t& n) {
    /*
     * ʹ�õ�ǰʱ����Ϊ����������������ӣ�ȷ��ÿ�����ɵ��������ͬ��
     */
    unsigned long int time_seed =
        std::chrono::high_resolution_clock::now()
        .time_since_epoch()
        .count();

    /*
     * ��ʼ�� GMP �������������״̬��
     * �������������������Ĳ��衣
     */
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time_seed);

    /*
     * ʹ�� mpz_inits ͬʱ��ʼ����� mpz_t ����������������Ϊ0��
     * �����ʼ�� p, q, n1, p1, q1, phi, ran, x, y��
     */
    mpz_t p, q, n1, p1, q1, phi, ran, x, y;
    mpz_inits(p, q, n1, p1, q1, phi, ran, x, y, NULL);

    /*
     * ����������ͬ��������� p �� q��
     * ������ɵ� p �� q ��ͬ������������ q��ֱ�����ǲ�ͬ��
     */
    rand_prime(p);
    do {
        rand_prime(q);
    } while (mpz_cmp(p, q) == 0);

    /*
     * ���� p - 1 �� q - 1������ŷ�������ļ��㡣
     */
    mpz_set(p1, p);
    mpz_sub_ui(p1, p1, 1);
    mpz_set(q1, q);
    mpz_sub_ui(q1, q1, 1);

    /*
     * ���� n = p * q �� ��(n) = (p-1)(q-1)��
     * ������ֵ�� RSA �㷨���ǻ����Եġ�
     */
    mpz_mul(n1, p, q);
    mpz_mul(phi, p1, q1);

    /*
     * ����һ������� ran��ȷ������ ��(n) ���ʡ�
     * ��� ran ����Ϊ��Կָ�� e��
     * ʹ��ѭ��ȷ�� ran �� ��(n) ���ʡ�
     */
    do {
        mpz_urandomm(ran, state, phi);  // ���� ran��ʹ�� ran < ��(n)
    } while (!exgcd(ran, phi, x, y));  // ��� ran �� ��(n) �Ƿ���
    /*
     * ���÷���ֵ�������ɵ���Կ��ֵ�����ò�����
     * e Ϊ��Կָ����d Ϊ˽Կָ����n Ϊģ����
     * ע�⣺�ȸ�ֵ n�����ͷ� n1��
     */

    mpz_fdiv_r(ran,ran,n1);
    mpz_fdiv_r(x, x, n1);
    
    mpz_set(n, n1);
    mpz_set(e, ran);
    mpz_set(d, x);
 //   gmp_printf("%Zd %Zd %Zd\n", &n1, &x,&d);
    mpz_clears(p, q, p1, q1, phi, ran, x, y, n1, NULL);

    /*
     * ����������������״̬���ͷ������Դ��
     */
    gmp_randclear(state);
}