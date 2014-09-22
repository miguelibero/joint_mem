joint_mem
=========

Inspired by [this video](https://www.youtube.com/watch?v=TH9VCN6UkyQ)
of Jonathan Blow talking about a new programming language based on C
but with additional memory handling features, I wrote a class in
C++11 that will alloc and dealloc memory for a list of pointers in
the way he was doing in [this slide](http://www.youtube.com/watch?v=TH9VCN6UkyQ&t=65m51s), but using the RAII idiom which he doesn't like (sorry, I know it's not
the point of the video).

To see how it works, read the tests, but it's more or less this:

```c++
#include <joint_mem/joint_mem.h>

struct test_member1 {
    int a;
};

struct test_member2 {
    double b;
};

struct test_struct2 {
    test_member1* member1;
    test_member2* member2;
    joint_mem mem;
};

test_struct2 struc;
// this will alloc 2*member1
// and 10*member2 consecutive memory
// and dealloc it when struc is destroyed
struc.mem = joint_mem{
	{ struc.member1, 2 },
	{ struc.member2, 10 }
};
```
