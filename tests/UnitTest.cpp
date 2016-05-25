#ifdef _MSC_VER //Microsoft Visual Studio only

#include "gtest/gtest.h"

TEST(Sanity, sanity1) {
	EXPECT_EQ(1, 1);
}

TEST(Sanity, sanity2) {
	EXPECT_EQ(1, 2);
}


int main (int argc, char** argv) {

    testing::InitGoogleTest(&argc, argv);

    int returnValue;

    returnValue =  RUN_ALL_TESTS();

    return returnValue;
}

#endif
