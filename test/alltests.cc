#include <gtest/gtest.h>

int main(int argc, char* argv[])
{ int sts;
  ::testing::InitGoogleTest(&argc,argv);
  sts = RUN_ALL_TESTS();
  //printf("Press <Enter> to continue\n");
  //getchar();
  return sts;
}
