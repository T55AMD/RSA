#pragma once
#include"rand.hpp"

bool exgcd(mpz_t& a, mpz_t& b, mpz_t& x, mpz_t& y) {
    /*
     * 如果 b 为 0
     */
    if (mpz_cmp_ui(b, 0) == 0) {
        /*
         * 如果 a 为 1
         */
        if (mpz_cmp_ui(a, 1) == 0) {
            mpz_set_ui(x, 1);  // 设置 x 为 1
            mpz_set_ui(y, 0);  // 设置 y 为 0
            return true;  // 返回 true
        }
        else {
            return false;  // 否则，返回 false
        }
    }

    /*
     * 初始化递归调用的临时变量
     */
    mpz_t a1, b1, q, r, tmp, z;
    mpz_inits(a1, b1, q, r, tmp, z, NULL);

    /*
     * 为下一次递归做准备
     */
    mpz_set(a1, b);  // a1 = b
    mpz_tdiv_r(r, a, b);  // r = a % b
    mpz_set(b1, r);  // b1 = a % b

    /*
     * 递归调用
     */
    bool flag = exgcd(a1, b1, x, y);
  //  gmp_printf("x=%Zd y=%Zd\n", &x, &y);

    /*
     * 如果递归调用成功
     */
    if (flag) {
        /*
         * 根据递归结果更新 x 和 y
         */
        mpz_set(z, x);  // z = x
        mpz_set(x, y);  // x = y
        mpz_tdiv_q(q, a, b);  // q = a / b
        mpz_mul(tmp, q, x);  // tmp = q * x
        mpz_sub(y, z, tmp);  // y = z - tmp
    }

    /*
     * 清除临时变量以防止内存泄漏
     */
    mpz_clears(a1, b1, q, r, tmp, z, NULL);

    return flag;  // 返回递归调用的结果
}
void RSA(mpz_t& e, mpz_t& d, mpz_t& n) {
    /*
     * 使用当前时间作为随机数生成器的种子，确保每次生成的随机数不同。
     */
    unsigned long int time_seed =
        std::chrono::high_resolution_clock::now()
        .time_since_epoch()
        .count();

    /*
     * 初始化 GMP 随机数生成器的状态。
     * 这是生成随机数所必须的步骤。
     */
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time_seed);

    /*
     * 使用 mpz_inits 同时初始化多个 mpz_t 变量，并将它们设为0。
     * 这里初始化 p, q, n1, p1, q1, phi, ran, x, y。
     */
    mpz_t p, q, n1, p1, q1, phi, ran, x, y;
    mpz_inits(p, q, n1, p1, q1, phi, ran, x, y, NULL);

    /*
     * 生成两个不同的随机素数 p 和 q。
     * 如果生成的 p 和 q 相同，则重新生成 q，直到它们不同。
     */
    rand_prime(p);
    do {
        rand_prime(q);
    } while (mpz_cmp(p, q) == 0);

    /*
     * 计算 p - 1 和 q - 1，用于欧拉函数的计算。
     */
    mpz_set(p1, p);
    mpz_sub_ui(p1, p1, 1);
    mpz_set(q1, q);
    mpz_sub_ui(q1, q1, 1);

    /*
     * 计算 n = p * q 和 φ(n) = (p-1)(q-1)。
     * 这两个值在 RSA 算法中是基础性的。
     */
    mpz_mul(n1, p, q);
    mpz_mul(phi, p1, q1);

    /*
     * 生成一个随机数 ran，确保它与 φ(n) 互质。
     * 这个 ran 将作为公钥指数 e。
     * 使用循环确保 ran 和 φ(n) 互质。
     */
    do {
        mpz_urandomm(ran, state, phi);  // 生成 ran，使得 ran < φ(n)
    } while (!exgcd(ran, phi, x, y));  // 检查 ran 和 φ(n) 是否互质
    /*
     * 设置返回值，将生成的密钥赋值给引用参数。
     * e 为公钥指数，d 为私钥指数，n 为模数。
     * 注意：先赋值 n，再释放 n1。
     */

    mpz_fdiv_r(ran,ran,n1);
    mpz_fdiv_r(x, x, n1);
    
    mpz_set(n, n1);
    mpz_set(e, ran);
    mpz_set(d, x);
 //   gmp_printf("%Zd %Zd %Zd\n", &n1, &x,&d);
    mpz_clears(p, q, p1, q1, phi, ran, x, y, n1, NULL);

    /*
     * 清除随机数生成器的状态，释放相关资源。
     */
    gmp_randclear(state);
}