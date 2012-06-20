#include <gtest/gtest.h>
#include <tictoc.h>

#ifdef _MSC_VER
#include <windows.h>
void usleep(int us) {
    __int64 time1 = 0, time2 = 0, freq = 0, w;

    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
    w=us*freq/1000000;
    QueryPerformanceCounter((LARGE_INTEGER *) &time1);

    do {
        QueryPerformanceCounter((LARGE_INTEGER *) &time2);
    } while((time2-time1) < w);
}
#endif

TEST(TicTocTimerTest,Basic)
{
  TicTocTimer t = tic();
  EXPECT_GE(toc(&t),0.0);
}
TEST(TicTocTimerTest,TicTocNULL)
{
  tic();
  EXPECT_GE(toc(NULL),0.0);
}
TEST(TicTocTimerTest,TocNULL)
{ EXPECT_GE(toc(NULL),0.0);
}
TEST(TicTocTimerTest,TicToc10MSecPrecision)
{
  tic();
  usleep(10000);
  ASSERT_NEAR(toc(NULL),0.01,0.005);
}
TEST(TicTocTimerTest,TicTocMSecPrecision)
{
  tic();
  usleep(1000);
  ASSERT_NEAR(toc(NULL),0.001,0.0005);
}
TEST(TicTocTimerTest,TicToc100USecPrecision)
{
  tic();
  usleep(100);
  ASSERT_NEAR(toc(NULL),0.0001,0.00005);
}
TEST(TicTocTimerTest,TicToc10USecPrecision)
{
  tic();
  usleep(10);
  ASSERT_NEAR(toc(NULL),0.00001,0.000005);
}
TEST(TicTocTimerTest,TicToc1USecPrecision)
{
  tic();
  usleep(1);
  ASSERT_NEAR(toc(NULL),0.000001,0.0000005);
}
