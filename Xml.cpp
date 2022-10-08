#include"Xml.h"
#include"Parser.h"
using namespace yyz::xml;

#include<iostream>
#include<sstream>
#include<fstream>
#include<algorithm>
using namespace std;
//Value
Value::Value()
{
}

Value::Value(bool value)
{
    *this = value;
}

Value::Value(int value)
{
    *this = value;
}

Value::Value(double value)
{
    *this = value;
}

Value::Value(const char * value) : m_value(value)
{
}

Value::Value(const string & value) : m_value(value)
{
}

Value::~Value()
{
}

Value & Value::operator = (bool value)
{
    m_value = value ? "true" : "false";
    return *this;
}

Value & Value::operator = (int value)
{
    stringstream ss;
    ss << value;
    m_value = ss.str();
    return *this;
}

Value & Value::operator = (double value)
{
    stringstream ss;
    ss << value;
    m_value = ss.str();
    return *this;
}

Value & Value::operator = (const char * value)
{
    m_value = value;
    return *this;
}

Value & Value::operator = (const string & value)
{
    m_value = value;
    return *this;
}

Value & Value::operator = (const Value & value)
{
    m_value = value.m_value;
    return *this;
}

bool Value::operator == (const Value & other)
{
    return m_value == other.m_value;
}

bool Value::operator != (const Value & other)
{
    return !(m_value == other.m_value);
}

Value::operator bool()
{
    if (m_value == "true")
        return true;
    else if (m_value == "false")
        return false;
    return false;
}

Value::operator int()
{
    return std::atoi(m_value.c_str());
}

Value::operator double()
{
    return std::atof(m_value.c_str());
}

Value::operator string()
{
    return m_value;
}

//Xml
Xml::Xml():m_name(nullptr),
        m_text(nullptr),
        m_attr(nullptr),
        m_childs(nullptr){
    ;
}

Xml::Xml(const std::string& name):m_name(nullptr),
                                m_text(nullptr),
                                m_attr(nullptr),
                                m_childs(nullptr){
    m_name=new string(name);
}

Xml::Xml(const char* name):m_name(nullptr),
                        m_text(nullptr),
                        m_attr(nullptr),
                        m_childs(nullptr){
    m_name=new string(name);
}
Xml::Xml(const Xml& xml){
    if(this==&xml){
        return ;
    }
    m_name=xml.m_name;
    m_text=xml.m_text;
    m_attr=xml.m_attr;
    m_childs=xml.m_childs;

}
void Xml::clear(){
    if(m_name!=nullptr){
        delete m_name;
        m_name=nullptr;
    }
    if(m_text!=nullptr){
        delete m_text;
        m_text=nullptr;
    }
    if(m_attr!=nullptr){
        delete m_attr;
        m_attr=nullptr;
    }
    if(m_childs!=nullptr){
        for(auto it : *m_childs){
            it.clear();
        }
        delete m_childs;
        m_childs=nullptr;
    }
}
Xml::ListIt Xml::findChildXml(const char* name) const{
    return this->findChildXml(string(name));
}
Xml::ListIt Xml::findChildXml(const string& name) const{
    for(auto it=m_childs->begin();it!=m_childs->end();++it){
        if(it->getName()==name){
            return it;
        }
    }
    return m_childs->end();
}
void Xml::setName(const std::string& name){
    if(m_name!=nullptr){
        delete m_name;
        m_name=nullptr;
    }
    m_name=new string(name);
}
std::string Xml::getName() const{
    if(m_name==nullptr){
        return "";
    }
    return *m_name;
}
void Xml::setText(const std::string& text){
    if(m_text!=nullptr){
        delete m_text;
        m_text=nullptr;
    }
    m_text=new string(text);
}
std::string Xml::getText() const{
    if(m_text==nullptr){
        return "";
    }
    return *m_text;
}
void Xml::setAttr(const std::string& key,const Value& value){
    if(m_attr==nullptr){
        m_attr=new map<string,Value>();
    }
    (*m_attr)[key]=value;

}
Value& Xml::getAttr(const std::string& key){
    if(m_attr==nullptr){
        m_attr=new map<string,Value>();
    }
    auto it=m_attr->find(key);
    if(it==m_attr->end()){
        throw new logic_error("Can't find attr!");
    }
    return (*m_attr)[key];
}

void Xml::append(const Xml& xml){
    if(m_childs==nullptr){
        m_childs=new list<Xml>();
    }
    m_childs->push_back(xml);
}

Xml& Xml::operator[](const string& name){
    if(m_childs==nullptr){
        m_childs=new list<Xml>();
    }
    auto it=this->findChildXml(name);
    if(it!=m_childs->end()){
        return *it;
    }
    m_childs->push_back(Xml(name));
    return m_childs->back();
}
const Xml& Xml::operator[](const string& name)const {
    return const_cast<const Xml&>((*this)[name]);
}

Xml& Xml::operator[](const char* name){
    return (*this)[string(name)];
}
const Xml& Xml::operator[](const char* name) const{
    return (*this)[string(name)];
}
Xml& Xml::operator = (const Xml& xml){
    if(this==&xml){
        return *this;
    }
    this->clear();
    m_name=xml.m_name;
    m_text=xml.m_text;
    m_attr=xml.m_attr;
    m_childs=xml.m_childs;
    return *this;
}

string Xml::serialize(){
    if(m_name==nullptr){
        throw new logic_error("element name is empty");
    }
    stringstream ss;
    ss << "<";
    ss << this->getName();
    //add attributes
    if(m_attr!=nullptr){
        for(auto it=m_attr->begin();it!=m_attr->end();++it){
            ss << " "<<it->first <<"=\"" << (string)it->second <<"\"";
        }
    }
    ss<<">";
    if(m_childs!=nullptr){
        for(auto& it :*m_childs){
            ss<<it.serialize();
        }
    }

    if(m_text!=nullptr){
        ss<<this->getText();
    }
    ss<<"</"<<this->getName()<<">";
    return ss.str();
}

bool Xml::removeChildXml(const char* name){
    return this->removeChildXml(string(name));
}
bool Xml::removeChildXml(const string& name){
    if(m_childs==nullptr){
        return false;
    }
    bool flag=false;
    for(auto it=m_childs->begin();it!=m_childs->end();){
        if(it->getName()==name){
            flag=true;
            it->clear();
            it=m_childs->erase(it);
        }
        else{
            it++;
        }
    }
    return flag;

}

bool Xml::load(const std::string& filename){
    Parser p;
    if (!p.loadFile(filename)){
        return false;
    }
    *this = p.parse();
    return true;
}
bool Xml::save(const std::string& filename){
    ofstream fout(filename);
    if (fout.fail()){
        return false;
    }
    fout << this->serialize();
    fout.close();
    return true;
}
bool Xml::parse(const std::string& str){
    Parser p;
    if (!p.loadString(str))
    {
        return false;
    }
    *this = p.parse();
    return true;
}