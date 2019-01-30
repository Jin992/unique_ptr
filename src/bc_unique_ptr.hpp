#ifndef BC_UNIQUE_PTR_HPP
#define BC_UNIQUE_PTR_HPP

namespace gl {
namespace cpp_basecamp {

template <class T>
struct DefaultDeleter {
	void operator()(T* ptr) const {
		delete ptr;
	}	
};

template <class T, class D = DefaultDeleter<T> >
class unique_ptr {
public:
	unique_ptr();
	explicit unique_ptr(T* val);
	unique_ptr(unique_ptr && rhs);
	unique_ptr(const unique_ptr & rhs) = delete;
	~unique_ptr();

	unique_ptr & operator=(const unique_ptr & rhs) = delete;
	unique_ptr & operator=(unique_ptr && rhs);
	T & operator*() const;
	T * operator->() const;
	bool operator==(const unique_ptr & rhs) const;

	T* get() const;
	T* release();

private:
	 T* ptr_;
};

template <class T, class D>
unique_ptr<T, D>::unique_ptr(): ptr_(nullptr)
{}

template <class T, class D>
T* unique_ptr<T, D>::get() const {
	return ptr_;
}

template <class T, class D>
unique_ptr<T, D>::unique_ptr(T* val): ptr_(val) 
{}

template <class T, class D>
unique_ptr<T, D>::unique_ptr(unique_ptr<T, D> && rhs) {
	ptr_ = rhs.ptr_;
	rhs.ptr_ = nullptr;
}

template <class T, class D>
	unique_ptr<T, D> & unique_ptr<T, D>::operator=(unique_ptr<T, D> && rhs) {
	ptr_ = rhs.ptr_;
	rhs.ptr_ = nullptr;
	return *this;
}

template <class T, class D>
unique_ptr<T, D>::~unique_ptr(){
	D obj;
	obj(ptr_);
}

template <class T, class D>
T* unique_ptr<T, D>::release() {
	T* tmp = ptr_;
	ptr_ = nullptr;
	return tmp;
}

template <class T, class D>
T & unique_ptr<T, D>::operator*() const {
	return *get();
}

template <class T, class D>
T * unique_ptr<T, D>::operator->() const{
	return get();
}

template <class T, class D>
bool unique_ptr<T, D>::operator==(const unique_ptr<T, D> &rhs) const {
	return get() == rhs.get();
}
} // namespace cxx_basecamp
} // namdespace gl

#endif // BC_UNIQUE_PTR_HPP
