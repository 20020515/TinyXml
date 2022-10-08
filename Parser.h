#ifndef _PARSER_
#define _PARSER_
#include "Xml.h"
#include <string>

namespace yyz {
namespace xml {

class Parser
{
public:
    Parser();
    bool loadFile(const std::string & file);
    bool loadString(const std::string & str);
    Xml parse();

private:
    void skipSpace();
    bool parseDeclaration();
    bool parseComment();
    Xml parseElement();
    std::string parseElementName();
    std::string parseElementText();
    std::string parseElementKey();
    std::string parseElementVal();

private:
    std::string m_str;
    int m_idx;
};

}
}
#endif