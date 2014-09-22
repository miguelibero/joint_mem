
#ifndef _joint_mem_hpp_
#define _joint_mem_hpp_

#include <memory>
#include <vector>
#include <cstdint>
#include <initializer_list>

typedef uint8_t joint_mem_unit;

struct joint_mem_array_def
{
    void*& ptr;
    size_t size;
    size_t num;

    template<typename Element>
    joint_mem_array_def(Element*& ptr, size_t num=1):
    ptr(reinterpret_cast<void*&>(ptr)),
    size(sizeof(Element)), num(num)
    {
    }
};

template<typename Alloc = std::allocator<joint_mem_unit>>
class joint_mem_alloc
{
public:
    typedef joint_mem_array_def array_def;
    typedef std::initializer_list<array_def> init_array_defs;
    typedef joint_mem_unit unit;
private:
    typedef std::vector<unit> vector;
    typedef std::unique_ptr<vector> vector_ptr;
    vector_ptr _data;

    static size_t size_of(init_array_defs defs)
    {
        size_t size = 0;
        for(const array_def& def : defs)
        {
            size += def.size*def.num;
        }
        return size;
    }

    static void assign(unit* ptr, init_array_defs defs)
    {
        for(const array_def& def : defs)
        {
            def.ptr = ptr;
            ptr += def.size*def.num;
        }
    }

    void reserve(init_array_defs defs)
    {
        _data->resize(size_of(defs));
        assign(_data->data(), defs);
    }

    joint_mem_alloc(const joint_mem_alloc& that) = delete;

public:
    joint_mem_alloc():
    _data(new vector(Alloc()))
    {
    }

    joint_mem_alloc(const Alloc& alloc):
    _data(new vector(alloc))
    {
    }

    joint_mem_alloc(init_array_defs defs) :
    _data(new vector(Alloc()))
    {
        reserve(defs);
    }

    joint_mem_alloc(const Alloc& alloc, init_array_defs defs) :
    _data(new vector(alloc))
    {
        reserve(defs);
    }

    joint_mem_alloc(joint_mem_alloc&& that):
    _data(std::move(that._data))
    {
    }

    joint_mem_alloc& operator=(joint_mem_alloc&& that)
    {
        _data = std::move(that._data);
        return *this;
    }

    size_t size() const
    {
        return _data->size();
    }

    const void* data() const
    {
        return _data->data();
    }

    void* data()
    {
        return _data->data();
    }

    template<typename Element>
    const Element* data() const
    {
        return static_cast<const Element*>(data());
    }

    template<typename Element>
    Element* data()
    {
        return static_cast<Element*>(data());
    }
};

class joint_mem : public joint_mem_alloc<>
{
public:
    joint_mem()
    {
    }

    joint_mem(init_array_defs defs) :
    joint_mem_alloc(defs)
    {
    }

    joint_mem(joint_mem&& that):
    joint_mem_alloc(std::move(that))
    {
    }

    joint_mem& operator=(joint_mem&& that)
    {
        joint_mem_alloc::operator=(std::move(that));
        return *this;
    }

};

#endif