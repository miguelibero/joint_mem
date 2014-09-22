joint_mem
=========

Inspired by [this video](https://www.youtube.com/watch?v=TH9VCN6UkyQ)
of Jonathan Blow talking about a new programming language based on C
but with additional memory handling features, I wrote a class in
C++11 that will alloc and dealloc memory for a list of pointers in
the way he was doing in [this slide](http://www.youtube.com/watch?v=TH9VCN6UkyQ&t=65m51s), but using the RAII idiom which he doesn't like (sorry, I know it's not
the point of the video).

To see how it works, read the tests.
Using this class, the slide would look like this:

```c++
#include <joint_mem/joint_mem.h>

struct Mesh {
	joint_mem memory_block;

	Vector3* positions = NULL;
	int* indices = NULL;
	Vector2* uvs = NULL;

	int num_indices = 0;
	int num_vertices = 0;
}

Mesh mesh;
mesh.memory_block = joint_mem{
	{ mesh.positions, mesh.num_vertices },
	{ mesh.indices, mesh.num_indices },
	{ mesh.uvs, mesh.num_vertices }
};
```
