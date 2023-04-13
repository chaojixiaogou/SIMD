#include<iostream>
#include<cstring>
#include<Windows.h>
#include <emmintrin.h>
using namespace std;
class TimerCounter
{
public:
    TimerCounter(void);//���캯��
    ~TimerCounter(void);//��������
private:
    LARGE_INTEGER startCount;//��¼��ʼʱ��

    LARGE_INTEGER endCount;//��¼����ʱ��

    LARGE_INTEGER freq;//����CPUʱ��Ƶ��
public:
    double dbTime;//�������е�ʱ�䱣��������

public:
    void Start();//�������ʼ�㴦��ʼ��ʱ
    void Stop();//�����������㴦������ʱ
};
TimerCounter::TimerCounter(void)
{
    QueryPerformanceFrequency(&freq);//��ȡ����CPUʱ��Ƶ��
}
TimerCounter::~TimerCounter(void)
{
}
void TimerCounter::Start()
{
    QueryPerformanceCounter(&startCount);//��ʼ��ʱ
}
void TimerCounter::Stop()
{
    QueryPerformanceCounter(&endCount);//ֹͣ��ʱ

    dbTime = ((double)endCount.QuadPart - (double)startCount.QuadPart) / (double)freq.QuadPart;//��ȡʱ���

}

int n; // ���̴�С
int cnt = 0; // �������
__declspec(align(16)) int row[20]; // ���ÿһ�лʺ��λ��

// �ж��ڵ�x�з��ûʺ��Ƿ�Ϸ�
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


// �ݹ麯�������õ�x�еĻʺ�
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
