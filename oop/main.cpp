#include <iostream>
#include "biginteger.cpp"

#include <gtest/gtest.h>

TEST(BigInteger_Initialisation, _from_EMPTY)
{
	BigInteger bi;

	EXPECT_EQ("0", bi.toString());
}

TEST(BigInteger_Initialisation, _from_int)
{
	BigInteger bi_0(0);
	EXPECT_EQ("0", bi_0.toString());

	BigInteger bi_1(1);
	EXPECT_EQ("1", bi_1.toString());

	BigInteger bi_2(-5);
	EXPECT_EQ("-5", bi_2.toString());
	
	BigInteger bi_3(12345);
	EXPECT_EQ("12345", bi_3.toString());

	BigInteger bi_4(-5432);
	EXPECT_EQ("-5432", bi_4.toString());
}

TEST(BigInteger_Initialisation, _from_BigInteger)
{
	BigInteger bi_0(0);
	BigInteger bi_0_(bi_0);
	EXPECT_EQ("0", bi_0_.toString());

	BigInteger bi_1(1);
	BigInteger bi_1_(bi_1);
	EXPECT_EQ("1", bi_1_.toString());

	BigInteger bi_2(-5);
	BigInteger bi_2_(bi_2);
	EXPECT_EQ("-5", bi_2_.toString());
	
	BigInteger bi_3(12345);
	BigInteger bi_3_(bi_3);
	EXPECT_EQ("12345", bi_3_.toString());

	BigInteger bi_4(-5432);
	BigInteger bi_4_(bi_4);
	EXPECT_EQ("-5432", bi_4_.toString());
}

TEST(BigInteger_Initialisation, _from_assigment)
{
	BigInteger bi_0(0);
	BigInteger bi_0_;
	bi_0_ = bi_0;
	EXPECT_EQ("0", bi_0_.toString());

	BigInteger bi_1(1);
	BigInteger bi_1_;
   	bi_1_ = bi_1;
	EXPECT_EQ("1", bi_1_.toString());

	BigInteger bi_2(-5);
	BigInteger bi_2_;
	bi_2_ = bi_2;
	EXPECT_EQ("-5", bi_2_.toString());
	
	BigInteger bi_3(12345);
	BigInteger bi_3_;
	bi_3_ = bi_3;
	EXPECT_EQ("12345", bi_3_.toString());

	BigInteger bi_4(-5432);
	BigInteger bi_4_;
	bi_4_ = bi_4;
	EXPECT_EQ("-5432", bi_4_.toString());
}

TEST(BigInteger_Comparison, _is_less)
{
	EXPECT_EQ(true, BigInteger(0) < BigInteger(1));
	EXPECT_EQ(true, BigInteger(-1) < BigInteger(0));
	EXPECT_EQ(false, BigInteger(0) < BigInteger(0));
	EXPECT_EQ(false, BigInteger(-1) < BigInteger(-1));
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
