#include"Parser.h"
using namespace yyz::xml;

#include<fstream>
#include<sstream>
using namespace std;
Parser::Parser():m_str(""),m_idx(0){

}
bool Parser::loadFile(const string & file){
    ifstream fin(file);
    if(fin.fail()){
        return false;
    }
    stringstream ss;
    ss<<fin.rdbuf();
    m_str=ss.str();
    m_idx=0;
    return true;
}
bool Parser::loadString(const string & str){
    m_str=str;
    m_idx=0;
    return true;
}
Xml Parser::parse(){
    skipSpace();
    if(m_str.compare(m_idx,5,"<?xml")==0){
        if(!parseDeclaration()){
            throw logic_error("Xml declaration error!");
        }
    }
    skipSpace();
    while(m_str.compare(m_idx, 4, "<!--")==0){
        if(!parseComment()){
            throw logic_error("Xml comment error!");
        }
        skipSpace();
    }
    if(m_str[m_idx]=='<'){
        Xml test=parseElement();
        return test;
    }
    throw std::logic_error("parse element error");
}
void Parser::skipSpace(){
    while(m_str[m_idx]==' '
    ||m_str[m_idx]=='\r'
    ||m_str[m_idx]=='\n'
    ||m_str[m_idx] == '\t'){
        m_idx++;
    }
}
bool Parser::parseDeclaration(){
    m_idx+=5;
    size_t pos=m_str.find("?>",m_idx);
    if(pos==string::npos){
        return false;
    }
    m_idx=pos+2;
    return true;
}
bool Parser::parseComment(){
    m_idx+=4;
    size_t pos=m_str.find("-->",m_idx);
    if(pos==string::npos){
        return false;
    }
    m_idx=pos+3;
    return true;
}
Xml Parser::parseElement(){
    Xml element;
    //from '<' to next 
    m_idx++;
    const string& name=parseElementName();
    element.setName(name);
    while (m_str[m_idx]!='\0')
    {
        skipSpace();
        if(m_str[m_idx]=='/'){
            if(m_str[m_idx]=='>'){
                m_idx+=2;
                break;
            }
            else{
                throw logic_error("Xml empty is error!");
            }
        }
        else if(m_str[m_idx]=='>'){
            m_idx++;
            const string& text=parseElementText();
            if(text!=""){
                element.setText(text);
            }
        }
        else if(m_str[m_idx]=='<'){
            //if is Root element endTag
            if(m_str[m_idx+1]=='/'){
                string endTag="</"+name+">";
                size_t pos=m_str.find(endTag,m_idx);
                if(pos==string::npos){
                    throw logic_error("Xml endTag format is error!");
                }
                m_idx=pos+endTag.size();
                break;
            }
            else if(m_str.compare(m_idx,4,"<!--")==0){
                if(!parseComment()){
                    throw logic_error("Xml Comment error!");
                }
            }
            else{
                auto a=parseElement();
                element.append(a);
                continue;
            }
        }
        else{
            //attribute
            // parse elem's attr
            string key = parseElementKey();
            skipSpace();
            if (m_str[m_idx] != '=')
            {
                throw std::logic_error("xml element attr is error" + key);
            }
            m_idx++;
            skipSpace();
            string val = parseElementVal();
            element.setAttr(key, val);
        }
    }
    return element;

}
string Parser::parseElementName(){
    int start=m_idx;
    while (isalnum(m_str[m_idx]) 
    || (m_str[m_idx] == '_') 
    || (m_str[m_idx] == '-') 
    || (m_str[m_idx] == ':') 
    || (m_str[m_idx] == '.'))
    {
        m_idx++;
    }
    return m_str.substr(start,m_idx-start);
}
string Parser::parseElementText(){
    int start =m_idx;
    while(m_str[m_idx]!='<'){
        m_idx++;
    }
    return m_str.substr(start,m_idx-start);
}
string Parser::parseElementKey(){
    int pos = m_idx;
    if (isalpha(m_str[m_idx]) || (m_str[m_idx] == '_'))
    {
        m_idx++;
        while (isalnum(m_str[m_idx]) || (m_str[m_idx] == '_') || (m_str[m_idx] == '-') || (m_str[m_idx] == ':'))
        {
            m_idx++;
        }
    }
    return m_str.substr(pos, m_idx - pos);
}
string Parser::parseElementVal(){
    if (m_str[m_idx] != '"')
    {
        throw std::logic_error("Xml Element Value should be in double quotes");
    }
    m_idx++;
    int pos = m_idx;
    while (m_str[m_idx] != '"')
    {
        m_idx++;
    }
    m_idx++;
    return m_str.substr(pos, m_idx - pos - 1);
}