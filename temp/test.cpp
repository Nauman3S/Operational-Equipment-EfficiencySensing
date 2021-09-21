
#include<iostream>
#include <math.h>
using namespace std;
int roundint(double r) {
  return (int)((r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5));
}

int main()
{   
    float f=12.612;
    cout<<f<<endl;;
    int a=(int)roundint(f);
    cout<<a<<endl;
    cout<<"Hello Compiler, I am C++";
    cout<<endl;
    return 0;
}