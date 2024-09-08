#ifndef UNIQUE_PTR
#define UNIQUE_PTR

#include <utility>

template<typename T>
class UniquePtr
{
public:
    UniquePtr():_p(nullptr){};
    explicit UniquePtr(T* p) : _p(p){};
    ~UniquePtr(){
        delete _p;
        _p = nullptr;
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr(UniquePtr && p){
        _p = std::move(p);
        p = nullptr;
    }

    UniquePtr& operator=(const UniquePtr&) = delete;
    UniquePtr& operator=(UniquePtr && p)
    {
        if(this != &p){
            _p = std::move(p);
            p = nullptr;
        }
        return *this;
    }

    T* get(){ return _p;}
    T& operator*(){ return *_p;}
    T* operator->(){ return _p;}

    void reset(){
        delete _p;
        _p = nullptr;
    }

    void reset(T* p){
        reset();
        _p = std::move(p);
        p = nullptr;
    }

    T* release(){
        T* p = _p;
        _p = nullptr;
        return p;
    }

    explicit operator bool() const { return _p != nullptr; }

private:
    T* _p;

};

template<typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args)
{
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}







#endif //UNIQUE_PTR