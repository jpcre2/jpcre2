#include <iostream>

class A{
    int a;
    
    public:
    
    virtual void setA(int x){
        a = x;
    }
    virtual int getA(){
        return a;
    }
    virtual ~A(){}
};

class B: virtual public A{
    int a;
};

int main(){
    
    B b;
    b.setA(7);
    std::cout<<b.getA();
    return 0;
}
