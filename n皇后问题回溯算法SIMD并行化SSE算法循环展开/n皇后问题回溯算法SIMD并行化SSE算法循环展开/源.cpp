#include<iostream>
#include<cstring>
#include<Windows.h>
#include <emmintrin.h>
using namespace std;
class TimerCounter
{
public:
    TimerCounter(void);//构造函数
    ~TimerCounter(void);//析构函数
private:
    LARGE_INTEGER startCount;//记录开始时间

    LARGE_INTEGER endCount;//记录结束时间

    LARGE_INTEGER freq;//本机CPU时钟频率
public:
    double dbTime;//程序运行的时间保存在这里

public:
    void Start();//被测程序开始点处开始计时
    void Stop();//被测程序结束点处结束计时
};
TimerCounter::TimerCounter(void)
{
    QueryPerformanceFrequency(&freq);//获取主机CPU时钟频率
}
TimerCounter::~TimerCounter(void)
{
}
void TimerCounter::Start()
{
    QueryPerformanceCounter(&startCount);//开始计时
}
void TimerCounter::Stop()
{
    QueryPerformanceCounter(&endCount);//停止计时

    dbTime = ((double)endCount.QuadPart - (double)startCount.QuadPart) / (double)freq.QuadPart;//获取时间差

}

int n; // 棋盘大小
int cnt = 0; // 解的数量
__declspec(align(16)) int row[20]; // 存放每一行皇后的位置

// 判断在第x行放置皇后是否合法
bool check(int x) {
    //*row = (int*)_mm_alignr_epi16((const __m128i*)row, (__m128i*)row, 0);
    int* row1 = (int*)(((unsigned long)row + 15) & ~15);
    int num = (x - 1) / 4;
    __m128i xmm1, xmm2, xmm3, xmm4;
    xmm2 = _mm_set1_epi32(row1[x]);
    xmm4 = _mm_set1_epi32(x);
    for (int j = 0; j < num; j++) {
        xmm1 = _mm_loadu_si128((__m128i*) & row1[1 + 4 * j]);
        xmm3 = _mm_setr_epi32(1 + 4 * j, 2 + 4 * j, 3 + 4 * j, 4 + 4 * j);
        __m128i xmm5 = _mm_cmpeq_epi32(xmm1, xmm2);
        int mask = _mm_movemask_epi8(xmm5);
        if (mask != 0)
            return false;
        xmm3 = _mm_sub_epi32(xmm4, xmm3);
        xmm1 = _mm_sub_epi32(xmm2, xmm1);
        //xmm1 = _mm_abs_epi32(xmm1);
        //__m128i sign_mask = _mm_set1_epi32(0x80000000);
        //xmm1 = _mm_xor_si128(xmm1, sign_mask);
        int* ptr = (int*)&xmm1;
        *ptr = abs(*ptr);
        *(ptr + 1) = abs(*(ptr + 1));
        *(ptr + 2) = abs(*(ptr + 2));
        *(ptr + 3) = abs(*(ptr + 3));
        xmm1 = _mm_cmpeq_epi32(xmm1, xmm3);
        int mask1 = _mm_movemask_epi8(xmm1);
        if (mask1 != 0)
            return false;
    }
    for (int i = 1 + 4 * num; i < x; i++) {
        if (row[i] == row[x] || abs(row[x] - row[i]) == abs(x - i))
            return false;
    }
    return true;

    /*if (x <= 4) {
        for (int i = 1; i < x; i++) {
            if (row[i] == row[x] || abs(row[x] - row[i]) == abs(x - i))
                return false;
        }
        return true;
    }
    else {
        __m128i xmm1, xmm2, xmm3, xmm4;
        xmm2 = _mm_set1_epi32(row[x]);
        xmm4 = _mm_set1_epi32(x);
        xmm1 = _mm_loadu_si128((__m128i*) & row[1]);
        xmm3 = _mm_setr_epi32(1, 2, 3, 4);
        __m128i xmm5 = _mm_cmpeq_epi32(xmm1, xmm2);
        int mask = _mm_movemask_epi8(xmm5);
        if (mask != 0)
            return false;
        xmm3 = _mm_sub_epi32(xmm4, xmm3);
        xmm1 = _mm_sub_epi32(xmm2, xmm1);
        //xmm1 = _mm_abs_epi32(xmm1);
        //__m128i sign_mask = _mm_set1_epi32(0x80000000);
        //xmm1 = _mm_xor_si128(xmm1, sign_mask);
        int* ptr = (int*)&xmm1;
        *ptr = abs(*ptr);
        *(ptr + 1) = abs(*(ptr + 1));
        *(ptr + 2) = abs(*(ptr + 2));
        *(ptr + 3) = abs(*(ptr + 3));
        xmm1 = _mm_cmpeq_epi32(xmm1, xmm3);
        int mask1 = _mm_movemask_epi8(xmm1);
        if (mask1 != 0)
            return false;

        for (int i = 5; i < x; i++) {
            if (row[i] == row[x] || abs(row[x] - row[i]) == abs(x - i))
                return false;
        }
        return true;
    }*/
}


// 递归函数，放置第x行的皇后
void dfs(int x) {
    if (x > n) {
        cnt++;
        return;
    }
    for (int i = 1; i < 9; i += 4) {
        row[x] = i;
        if (check(x)) {
            dfs(x + 1);
        }
        row[x] = i + 1;
        if (check(x)) {
            dfs(x + 1);
        }
        row[x] = i + 2;
        if (check(x)) {
            dfs(x + 1);
        }
        row[x] = i + 3;
        if (check(x)) {
            dfs(x + 1);
        }
    }
    for (int i = 9; i <= n; i++) {
        row[x] = i;
        if (check(x)) {
            dfs(x + 1);
        }
    }
}

int main() {
    cin >> n;
    TimerCounter tc;
    tc.Start();
    dfs(1);
    tc.Stop();
    cout << cnt << endl;
    cout << tc.dbTime;
    return 0;
}
