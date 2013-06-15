#include <iostream>
#include <map>
#include <set>
#include <memory>
#include <string>
#include "smart_ptr.h"

using namespace smart_ptr;

// X.hpp:

class X
{
public:

    virtual void f() = 0;
    virtual void g() = 0;

    virtual ~X() {
        std::cout << "X::~X()" << std::endl;
    }
protected:

};

strong_ptr<X> createX();

//-- X.cpp:

class X_impl: public X
{
private:

    X_impl(X_impl const &);
    X_impl & operator=(X_impl const &);

public:
    X_impl()
    {
    }

    virtual ~X_impl()
    {
        std::cout << "X_impl::~X_impl()" << std::endl;
    }

    virtual void f()
    {
        std::cout << "X_impl::f()" << std::endl;
    }

    virtual void g()
    {
        std::cout << "X_impl::g()" << std::endl;
    }
};

strong_ptr<X> createX()
{
    strong_ptr<X> px(new X_impl());
    return px;
}

int main(void)
{
    std::string key = "key-string";
    strong_ptr<X> sp = createX();

    std::map<std::string, strong_ptr<X> > mapSp;
    mapSp[key] = sp;

    strong_ptr<X> &sp2 = mapSp.at(key);
    if (sp2) {
        sp2->f();
    }

    std::set<strong_ptr<X> > setSp;
    setSp.insert(sp);
    (*setSp.begin())->g();
}
