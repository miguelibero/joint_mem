
#include <joint_mem/joint_mem.h>
#include "gtest/gtest.h"

struct test_member
{
    int a;
};

struct test_struct1 : public joint_mem
{
    test_member* member1;
    test_member* member2;

    test_struct1():
    joint_mem(member1, member2)
    {
    }
};

struct test_struct2
{
    test_member* member1;
    test_member* member2;
    joint_mem mem;

    test_struct2()
    {
    }
};

TEST(joint_mem, basic1) {

    test_struct1 struc;
    test_member* ptr = struc.data<test_member>();
    ASSERT_EQ(2*sizeof(test_member), struc.size());
    ASSERT_EQ(struc.member1, ptr);
    ASSERT_EQ(struc.member2, ptr+1);
}

TEST(joint_mem, basic2) {

    test_struct2 struc;
    struc.mem = joint_mem(struc.member1, struc.member2);
    test_member* ptr = struc.mem.data<test_member>();
    ASSERT_EQ(2*sizeof(test_member), struc.mem.size());
    ASSERT_EQ(struc.member1, ptr);
    ASSERT_EQ(struc.member2, ptr+1);
}

TEST(joint_mem, multiple) {

	test_struct2 struc;
    struc.mem = joint_mem<test_member, test_member>(
    	{ struc.member1, 2 },
    	{ struc.member2, 10 });

    test_member* ptr = struc.mem.data<test_member>();
    ASSERT_EQ(12*sizeof(test_member), struc.mem.size());
    ASSERT_EQ(struc.member1, ptr);
    ASSERT_EQ(struc.member2, ptr+2);

}