#include <iostream>
#include "biginteger.h"

#include <gtest/gtest.h>

TEST(BigInteger_Initialisation, _from_EMPTY)
{
	BigInteger bi;

	ASSERT_EQ("0", bi.toString());
}

TEST(BigInteger_Initialisation, _from_int)
{
	BigInteger bi_0(0);
	ASSERT_EQ("0", bi_0.toString());

	BigInteger bi_1(1);
	ASSERT_EQ("1", bi_1.toString());

	BigInteger bi_2(-5);
	ASSERT_EQ("-5", bi_2.toString());
	
	BigInteger bi_3(12345);
	ASSERT_EQ("12345", bi_3.toString());

	BigInteger bi_4(-5432);
	ASSERT_EQ("-5432", bi_4.toString());
}

TEST(BigInteger_Initialisation, _from_BigInteger)
{
	BigInteger bi_0(0);
	BigInteger bi_0_(bi_0);
	ASSERT_EQ("0", bi_0_.toString());

	BigInteger bi_1(1);
	BigInteger bi_1_(bi_1);
	ASSERT_EQ("1", bi_1_.toString());

	BigInteger bi_2(-5);
	BigInteger bi_2_(bi_2);
	ASSERT_EQ("-5", bi_2_.toString());
	
	BigInteger bi_3(12345);
	BigInteger bi_3_(bi_3);
	ASSERT_EQ("12345", bi_3_.toString());

	BigInteger bi_4(-5432);
	BigInteger bi_4_(bi_4);
	ASSERT_EQ("-5432", bi_4_.toString());
}

TEST(BigInteger_Initialisation, _from_assigment)
{
	BigInteger bi_0(0);
	BigInteger bi_0_;
	bi_0_ = bi_0;
	ASSERT_EQ("0", bi_0_.toString());

	BigInteger bi_1(1);
	BigInteger bi_1_;
   	bi_1_ = bi_1;
	ASSERT_EQ("1", bi_1_.toString());

	BigInteger bi_2(-5);
	BigInteger bi_2_;
	bi_2_ = bi_2;
	ASSERT_EQ("-5", bi_2_.toString());
	
	BigInteger bi_3(12345);
	BigInteger bi_3_;
	bi_3_ = bi_3;
	ASSERT_EQ("12345", bi_3_.toString());

	BigInteger bi_4(-5432);
	BigInteger bi_4_;
	bi_4_ = bi_4;
	ASSERT_EQ("-5432", bi_4_.toString());
}

/* ###################################################### */

TEST(BigInteger_Comparison, _is_less)
{
	ASSERT_EQ(true, BigInteger(0) < BigInteger(1));
	ASSERT_EQ(true, BigInteger(-1) < BigInteger(0));
	ASSERT_EQ(false, BigInteger(0) < BigInteger(0));
	ASSERT_EQ(false, BigInteger(-1) < BigInteger(-1));

	for(int l=-200; l<=200; ++l)
		for(int r=-200; r<=200; ++r)
			ASSERT_EQ(l < r, BigInteger(l) < BigInteger(r));
}

TEST(BigInteger_Comparison, _others)
{
	for(int l=-20; l<=20; ++l)
		for(int r=-20; r<=20; ++r)
		{
			BigInteger bi_l(l), bi_r(r);
			ASSERT_EQ(l > r, bi_l > bi_r);
			ASSERT_EQ(l == r, bi_l == bi_r);
			ASSERT_EQ(l != r, bi_l != bi_r);
			ASSERT_EQ(l <= r, bi_l <= bi_r);
			ASSERT_EQ(l >= r, bi_l >= bi_r);
		}
}

/* ###################################################### */

TEST(BigInteger_Output, _to_string)
{
	for(int i=-20; i<=20; ++i)
		ASSERT_EQ(std::to_string(i), BigInteger(i).toString());
}

TEST(BigInteger_Output, _cout)
{
	for(int i=-20; i<=20; ++i)
	{
		testing::internal::CaptureStdout();
		std::cout << BigInteger(i);
		std::string output = testing::internal::GetCapturedStdout();
		ASSERT_EQ(std::to_string(i), output);
	}
}

/* ###################################################### */

TEST(BigInteger_Arithmetic, _unari_minus)
{
	for(int i=-20; i<=20; ++i)
		ASSERT_EQ(BigInteger(-i), -BigInteger(i));
}

TEST(BigInteger_Arithmetic_elementary, _addition_same_sign)
{
	for(int i=1; i<100; ++i)
		for(int j=0; j<i; ++j)
		{
			BigInteger bi(i);
			bi += BigInteger(j);
			ASSERT_EQ(BigInteger(i+j), bi);
		}

	for(int i=-100; i<0; ++i)
		for(int j=i; j<0; ++j)
		{
			BigInteger bi(i);
			bi += BigInteger(j);
			ASSERT_EQ(BigInteger(i+j), bi);
		}
}

TEST(BigInteger_Arithmetic_elementary, _addition_diff_sign)
{
	for(int i=1; i<100; ++i)
		for(int j=0; j<i; ++j)
		{
			BigInteger bi(i);
			bi += BigInteger(-j);
			ASSERT_EQ(BigInteger(i-j), bi);
		}
	
	for(int i=-100; i<0; ++i)
		for(int j=i; j<0; ++j)
		{
			BigInteger bi(i);
			bi += BigInteger(-j);
			ASSERT_EQ(BigInteger(i-j), bi);
		}
}

TEST(BigInteger_Arithmetic_elementary, _random_math)
{

	for(int i=-200; i<=200; ++i)
		for(int j=-200; j<=200; ++j)
		{
			ASSERT_EQ(BigInteger(i+j), BigInteger(i) + BigInteger(j));
			ASSERT_EQ(BigInteger(i-j), BigInteger(i) - BigInteger(j));
		}
}

TEST(BigInteger_Arithmetic_elementary, _inc_dec)
{
	int i;
	for(BigInteger bi=-100, i=-100; bi<=100; ++bi, ++i)
		ASSERT_EQ(BigInteger(i), bi);

	for(BigInteger bi=100, i=100; bi>=-100; --bi, --i)
		ASSERT_EQ(BigInteger(i), bi);
}

/* ###################################################### */

/*
TEST(BigInteger_Arithmetic_intermediate, _mul_int)
{
	for(int i=-200; i<=200; ++i)
		for(int j=0; j<10; ++j)
			ASSERT_EQ(BigInteger(i*j), BigInteger(i).mul(j));
}
*/

TEST(BigInteger_Arithmetic_intermediate, _mul)
{
	for(int i=-200; i<=1000; ++i)
		for(int j=-200; j<=200; ++j)
		{
			BigInteger bi(i);
			bi *= j;

			ASSERT_EQ(BigInteger(i*j), bi);
		}
}

/* ###################################################### */

TEST(BigInteger_Arithmetic_apper, _div)
{
	for(int i=-200; i<=200; ++i)
		for(int j=1; j<40; ++j)
			ASSERT_EQ(BigInteger(i/j), BigInteger(i) / BigInteger(j));

	for(int i=-200; i<=200; ++i)
		for(int j=1; j<40; ++j)
			ASSERT_EQ(BigInteger(i%j), BigInteger(i) % BigInteger(j));
}

/* ###################################################### */
/* ###################################################### */
/* ###################################################### */
/* ###################################################### */

int main(int argc, char **argv)
{

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
