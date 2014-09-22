
#ifndef _joint_mem_hpp_
#define _joint_mem_hpp_

#include <memory>
#include <vector>
#include <cstdint>

typedef uint8_t joint_mem_unit;

template<typename Element>
struct joint_mem_array_def
{
	Element*& ptr;
	size_t size;

	/*
	joint_mem_array_def(Element*& ptr, size_t size=1):
	ptr(ptr), size(size)
	{
	}
	*/
};

template<typename Alloc = std::allocator<joint_mem_unit>>
class joint_mem_alloc
{
public:
	template <typename Element>
	using array_def = joint_mem_array_def<Element>;
    typedef joint_mem_unit unit;
private:
	typedef std::vector<unit> vector;
    typedef std::unique_ptr<vector> vector_ptr;
    vector_ptr _data;

    template<typename Element, typename... Elements>
    size_t size_of(const array_def<Element>& def,
        const array_def<Elements>&... defs)
    {
        return size_of(def)+size_of(defs...);
    }

    template<typename Element>
    size_t size_of(const array_def<Element>& def)
    {
        return def.size*sizeof(Element);
    }

    template<typename Element, typename... Elements>
    void assign(unit* ptr, const array_def<Element>& def,
        const array_def<Elements>&... defs)
    {
        assign(ptr, def);
        assign(ptr+size_of(def), defs...);
    }

    template<typename Element>
    void assign(unit* ptr, const array_def<Element>& def)
    {
        def.ptr = reinterpret_cast<Element*>(ptr);
    }

    template<typename... Elements>
    void reserve(const array_def<Elements>&... defs)
    {
        _data->resize(size_of(defs...));
        assign(_data->data(), defs...);
    }

    template<typename Element, typename... Elements>
    void reserve(Element*& elm, Elements*&... elms)
    {
        reserve(array_def<Element>{elm, 1},
            array_def<Element>{elms, 1}...);
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

    template<typename Element, typename... Elements>
    joint_mem_alloc(const array_def<Element>& elm,
        const array_def<Elements>&... elms):
    _data(new vector(Alloc()))
    {
        reserve(elm, elms...);
    }

    template<typename Element, typename... Elements>
    joint_mem_alloc(const Alloc& alloc,
        const array_def<Element>& elm,
        const array_def<Elements>&... elms):
    _data(new vector(alloc))
    {
        reserve(elm, elms...);
    }

    template<typename Element, typename... Elements>
    joint_mem_alloc(Element*& elm, Elements*&... elms):
    _data(new vector(Alloc()))
    {
        reserve(elm, elms...);
    }

    template<typename Element, typename... Elements>
    joint_mem_alloc(const Alloc& alloc,
    	Element*& elm, Elements*&... elms):
    _data(new vector(alloc))
    {
        reserve(elm, elms...);
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

    template<typename Element, typename... Elements>
    joint_mem(const array_def<Element>& elm,
        const array_def<Elements>&... elms):
    joint_mem_alloc(elm, elms...)
    {
    }

    template<typename Element, typename... Elements>
    joint_mem(Element*& elm, Elements*&... elms):
    joint_mem_alloc(elm, elms...)
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