
#include <joint_mem/joint_mem.h>
#include "gtest/gtest.h"

struct test_member1
{
    int a;
};

struct test_member2
{
    double b;
};

struct test_struct1 : public joint_mem
{
    test_member1* member1;
    test_member2* member2;

    test_struct1():
    joint_mem{member1, member2}
    {
    }
};

struct test_struct2
{
    test_member1* member1;
    test_member2* member2;
    joint_mem mem;
};

TEST(joint_mem, basic1) {

    test_struct1 struc;
    auto ptr = struc.data<test_member1>();
    ASSERT_EQ(sizeof(test_member1)+sizeof(test_member2), struc.size());
    ASSERT_EQ(struc.member1, ptr);
    ASSERT_EQ(struc.member2, reinterpret_cast<test_member2*>(ptr+1));
}

TEST(joint_mem, basic2) {

    test_struct2 struc;
    struc.mem = joint_mem{struc.member1, struc.member2};
    auto ptr = struc.mem.data<test_member1>();
    ASSERT_EQ(sizeof(test_member1)+sizeof(test_member2), struc.mem.size());
    ASSERT_EQ(struc.member1, ptr);
    ASSERT_EQ(struc.member2, reinterpret_cast<test_member2*>(ptr+1));
}

TEST(joint_mem, multiple) {

	test_struct2 struc;
    struc.mem = joint_mem{
    	{ struc.member1, 2 },
    	{ struc.member2, 10 }
    };

    auto ptr = struc.mem.data<test_member1>();
    ASSERT_EQ(2*sizeof(test_member1)+10*sizeof(test_member2), struc.mem.size());
    ASSERT_EQ(struc.member1, ptr);
    ASSERT_EQ(struc.member2, reinterpret_cast<test_member2*>(ptr+2));

}