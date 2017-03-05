#include <iostream>

class B{
    int a;
    public:
    virtual ~B(){}
    virtual B& setA(int x){
        a = x;
        return *this;
    }
};

class D: virtual public B{
    int d;
    public:
    D& setD(int x){
        d = x;
        return *this;
    }
};

int main(){

    return 0;
}
