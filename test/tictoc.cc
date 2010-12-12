#include <tictoc.h>

#ifdef HAVE_GTEST
#include <gtest/gtest.h>
TEST(TicTocTimerTest,Basic)
{
  TicTocTimer t = tic();
  ASSERT_TRUE(toc(&t)>0.0);
}
TEST(TicTocTimerTest,TicTocNULL)
{
  tic();
  ASSERT_TRUE(toc(NULL)>0.0);
}
TEST(TicTocTimerTest,TocNULL)
{
  ASSERT_TRUE(toc(NULL)>0.0);
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
#endif
