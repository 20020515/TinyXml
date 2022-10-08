#ifndef _XML_
#define _XML_
#include<string>
#include<map>
#include<list>
namespace yyz
{
namespace xml
{
class Value
{
public:
    Value();
    Value(bool value);
    Value(int value);
    Value(double value);
    Value(const char * value);
    Value(const std::string & value);
    ~Value();

    Value & operator = (bool value);
    Value & operator = (int value);
    Value & operator = (double value);
    Value & operator = (const char * value);
    Value & operator = (const std::string & value);
    Value & operator = (const Value & value);

    bool operator == (const Value & other);
    bool operator != (const Value & other);
    
    operator bool();
    operator int();
    operator double();
    operator std::string();

private:
    std::string m_value;
};

class Xml{
    typedef std::list<Xml>::iterator ListIt;
public:
    Xml();
    Xml(const std::string& name);
    Xml(const char* name);
    Xml(const Xml& xml);
    ~Xml(){;}
    //bean
    void setName(const std::string& name);
    std::string getName() const;
    
    void setText(const std::string& text);
    std::string getText() const;

    void setAttr(const std::string& key,const Value& value);
    Value& getAttr(const std::string& key);

    void append(const Xml& child);
    
    //serialize
    std::string serialize();

    //operator
    Xml& operator[](const std::string& name);
    const Xml& operator[](const std::string& name) const;
    Xml& operator[](const char* name);
    const Xml& operator[](const char* name) const;
    Xml& operator = (const Xml& xml);

    //delete child node
    bool removeChildXml(const char* name);
    bool removeChildXml(const std::string& name);

    //find child node
    ///must ensure the m_childs is not nullptr
    ListIt findChildXml(const char* name) const;
    ListIt findChildXml(const std::string& name) const;
    
    //delete the dynamic memory
    void clear();

    //iterator begin and end
    ListIt begin(){
        return m_childs->begin();
    }
    ListIt end(){
        return m_childs->end();
    }
    int size(){
        return m_childs->size();
    }
    //load
    bool load(const std::string& filename);
    bool save(const std::string& filename);
    bool parse(const std::string& str);

private:
    std::string* m_name;
    std::string* m_text;
    std::map<std::string,Value>* m_attr;
    std::list<Xml>* m_childs;

};
} // namespace xml
} // namespace yyz

#endif