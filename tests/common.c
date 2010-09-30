#include "../types.h"

TEST(BasicTypes,ByteSizes)
{ 
  EXPECT_TRUE(sizeof(u8 )==1 );
  EXPECT_TRUE(sizeof(u16)==2 );
  EXPECT_TRUE(sizeof(u32)==4 );
  EXPECT_TRUE(sizeof(u64)==8 );
  EXPECT_TRUE(sizeof(i8 )==1 );
  EXPECT_TRUE(sizeof(i16)==2 );
  EXPECT_TRUE(sizeof(i32)==4 );
  EXPECT_TRUE(sizeof(i64)==8 );
  EXPECT_TRUE(sizeof(f32)==4 );
  EXPECT_TRUE(sizeof(f64)==8 );
}
