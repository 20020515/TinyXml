#include"Xml.h"
#include<iostream>
using namespace std;
using namespace yyz::xml;
int main(void){
    Xml root;
    root.load("./test.xml");
    cout<<root.serialize();
    root.removeChildXml("company");
    cout<<root.serialize();
    root.save("./test.xml");
}