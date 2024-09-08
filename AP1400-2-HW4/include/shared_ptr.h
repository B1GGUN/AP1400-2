#ifndef SHARED_PTR
#define SHARED_PTR

template <typename T>
class SharedPtr
{
public:
    SharedPtr() : _p(nullptr), cnt(new size_t(0)) {};
    SharedPtr(T *p) : _p(p), cnt(new size_t(1)) {};
    ~SharedPtr()
    {
        if ((*cnt)-- == 1)
        {
            delete _p;
        }
        _p = nullptr;
    }

    SharedPtr(const SharedPtr &p)
    {
        _p = p._p;
        cnt = p.cnt;
        (*cnt)++;
    }

    SharedPtr(SharedPtr &&p) : _p(p._p), cnt(p.cnt)
    {
        p.cnt = nullptr;
        p = nullptr;
    }

    SharedPtr &operator=(const SharedPtr &p)
    {
        if (this != &p)
        {
            _p = p._p;
            cnt = p.cnt;
            (*cnt)++;
        }
        return *this;
    }

    SharedPtr &operator=(SharedPtr &&p)
    {
        if (p != &this)
        {
            _p = p;
            cnt = p.cnt;
            p.cnt = nullptr;
            p = nullptr;
        }
        return *this;
    }

    size_t use_count()
    {
        return *cnt;
    }

    T *get()
    {
        return _p;
    }

    T &operator*()
    {
        return *_p;
    }

    T *operator->()
    {
        return _p;
    }

    void reset()
    {
        delete _p;
        *cnt = 0;
        _p = nullptr;
    }

    void reset(T* p){
        delete _p;
        _p = std::move(p);
        *cnt = 1;
    }

    explicit operator bool() const { return _p != nullptr; }

private:
    T *_p;
    size_t *cnt;
};

template <typename T, typename... Args>
SharedPtr<T> make_shared(Args &&...args)
{
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

#endif // SHARED_PTR