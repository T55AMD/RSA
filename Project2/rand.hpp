#include "gmp.h"
#include<chrono>
#include <time.h>
#include<set>
// ��������
const int TERMS = 80; //80�ֵ�Miller-Rabin��������Ӧ���൱��ȫ
const int MAX = 1024;
// ������ȡģ����
bool pre(const mpz_t& num) {
    const int prime[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
    mpz_t q;
    mpz_init(q);

    for (int i = 0; i < 10; i++) {
        int it = prime[i];
        mpz_mod_ui(q, num, it);
        if (mpz_cmp_ui(q, 0) == 0) {
            mpz_clear(q);  // ȷ������ǰ����ʱ���� q
            return false;
        }
    }

    mpz_clear(q);  // �����˳�ʱҲ���� q
    return true;
}

void quickpow(mpz_t& ans, const mpz_t& a, const mpz_t& b, const mpz_t& MOD) {
    // ��ʼ�����Ϊ 1
    mpz_set_si(ans, 1);

    // ��ʼ����ʱ����
    mpz_t zero, one, r, temp_b, temp_a;
    mpz_init(zero);
    mpz_init(one);
    mpz_init(r);
    mpz_init(temp_b);
    mpz_init(temp_a);

    // ���ó���ֵ
    mpz_set_si(zero, 0);
    mpz_set_si(one, 1);

    // ���� b �� a �ĸ����������޸�ԭʼ����
    mpz_set(temp_b, b);
    mpz_set(temp_a, a);

    // ������ȡģ�㷨
    while (mpz_cmp(temp_b, zero) > 0) {
        // ��� temp_b �����λ�Ƿ�Ϊ 1
        mpz_tdiv_r_2exp(r, temp_b, 1);
        if (mpz_cmp(r, zero) > 0) {
            mpz_mul(ans, ans, temp_a);
            mpz_mod(ans, ans, MOD);
        }

        // �� temp_b ����һλ
        mpz_tdiv_q_2exp(temp_b, temp_b, 1);

        // ƽ�� temp_a����ȡģ
        mpz_mul(temp_a, temp_a, temp_a);
        mpz_mod(temp_a, temp_a, MOD);
    }

    // ������ʱ����
    mpz_clear(zero);
    mpz_clear(one);
    mpz_clear(r);
    mpz_clear(temp_b);
    mpz_clear(temp_a);
}

// Miller-Rabin ���Բ���
bool Miller_Rabin(const mpz_t& num) {
    if (mpz_cmp_ui(num, 2) <= 0) {
        return mpz_cmp_ui(num, 2) == 0;
    }

    // ��ʼ�������������
 unsigned long int time_seed =
        std::chrono::high_resolution_clock::now()
        .time_since_epoch()
        .count();
    gmp_randstate_t state;
    gmp_randinit_default(state);
   
    gmp_randseed_ui(state, time_seed);

    // ��ʼ����ʱ����
    mpz_t p, u, s, m1, v, A, cnt;
    mpz_init(p);
    mpz_init(u);
    mpz_init(s);
    mpz_init(m1);
    mpz_init(v);
    mpz_init(A);
    mpz_init(cnt);

    // ���� u = num - 1
    mpz_set_ui(s, 0);
    mpz_set(u, num);
    mpz_sub_ui(u, u, 1);
    mpz_set(m1, u);
  //  gmp_printf("%zd", &u);
    // �ֽ� u = 2^s * d
    while (mpz_congruent_ui_p(u, 0, 2) != 0) {
        mpz_cdiv_q_2exp(u, u, 1);
        mpz_add_ui(s, s, 1);
    }

    // ���� t �β���
    int t = TERMS;
    bool ans = true;
    while (t--) {
        // ����һ������� p
        mpz_urandomm(p, state, num);
        if (mpz_cmp_si(p, 0) == 0) {
            continue;
        }
        // ���� A = p^d % num
        quickpow(A, p, u, num);

        if (mpz_cmp_ui(A, 1) == 0) {
            continue;
        }

        // ��� A �Ƿ���� num-1 ���� A^2 % num �Ƿ���� num-1
        mpz_set_ui(cnt, 0);  // ���� cnt
        for (; mpz_cmp (cnt, s) != 0;) {
          //  gmp_printf("%Zd %Zd %Zd %Zd %Zd \n ", &cnt, &s,&p,&A,&num);
            
            if (mpz_cmp(A, m1) == 0) {
                break;
            }
            mpz_add_ui(cnt, cnt, 1);
            
            mpz_mul(A, A, A);
            mpz_mod(A, A, num);
        }
        
        if (mpz_cmp(cnt, s) == 0) {
            ans = false;
            break;
        }
    }

    // ����������ʱ����
    mpz_clear(p);
    mpz_clear(u);
    mpz_clear(s);
    mpz_clear(m1);
    mpz_clear(v);
    mpz_clear(A);
    mpz_clear(cnt);

    // ���������������״̬
    gmp_randclear(state);
    /*if (ans) {
        gmp_printf("%Zd is Prime\n",&num);
    }
    else {
        gmp_printf("%Zd isnot Prime\n",&num);
    }*/
    return ans;
}
inline bool is_prime(const mpz_t& tar) {
	return pre(tar)&&Miller_Rabin(tar);
}

void rand_prime(mpz_t& tar) {
    // ��ʼ�������������
  
    unsigned long int time_seed =
        std::chrono::high_resolution_clock::now()
        .time_since_epoch()
        .count();
    gmp_randstate_t state;
   
    gmp_randinit_default(state);  // ��ʽ��ʼ�������������
    gmp_randseed_ui(state, time_seed);
    // ��ʼ����ʱ����
    mpz_t x;
    mpz_init(x);

    // ȷ������
    do {
        mpz_rrandomb(x, state, MAX);  // ����һ�������
    //    gmp_printf("%Zd\n", &x);
    } while (!is_prime(x));

    // �����ɵ�������ֵ�� tar
    mpz_set(tar, x);

    // ������ʱ����
    mpz_clear(x);

    // ���������������״̬
    gmp_randclear(state);
}