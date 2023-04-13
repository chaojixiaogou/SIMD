#include <immintrin.h>
#include <iostream>
#include <cstring>
#include<Windows.h>
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

int n;
int cnt = 0;
int row[20];

bool check(int x) {
    if (x <= 8) {
        for (int i = 1; i < x; i++) {
            if (row[i] == row[x] || abs(row[x] - row[i]) == abs(x - i))
                return false;
        }
        return true;
    }
    else {
        __m256i xmm1, xmm2, xmm3, xmm4;
        xmm2 = _mm256_set1_epi32(row[x]);
        xmm4 = _mm256_set1_epi32(x);
        xmm1 = _mm256_loadu_si256((__m256i*) & row[1]);
        xmm3 = _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8);
        __m256i xmm5 = _mm256_cmpeq_epi32(xmm1, xmm2);
        int test1 = _mm256_testz_si256(xmm5, xmm5);
        if (test1 == 0)
            return false;
        xmm3 = _mm256_sub_epi32(xmm4, xmm3);
        xmm1 = _mm256_sub_epi32(xmm2, xmm1);
        xmm1 = _mm256_abs_epi32(xmm1);
        xmm1 = _mm256_cmpeq_epi32(xmm1, xmm3);
        int test = _mm256_testz_si256(xmm1, xmm1);
        if (test == 0)
            return false;
        for (int i = 9; i < x; i++) {
            if (row[i] == row[x] || abs(row[x] - row[i]) == abs(x - i))
                return false;
        }
        return true;
    }
}

void dfs(int x) {
    if (x > n) {
        cnt++;
        return;
    }
    for (int i = 1; i <= n; i++) {
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
