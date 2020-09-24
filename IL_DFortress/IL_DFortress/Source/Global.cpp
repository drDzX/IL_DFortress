#include "Global.h"


std::string ReadJson(const char InPath[], const char elemKey[])
{

	std::ifstream file(InPath);
	json J;
	file >> J;
	std::string rt = J.at(elemKey);
	return rt;

}

std::string ReadXML(const char InPath[], const char NodeName[], const char Attribute[])
{
    //Debuging segment
    //std::ifstream t("E:\\Dezex\\Projects\\IL_DFortress\\IL_DFortress\\IL_DFortress\\Config\\world.xml");
    //std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    //std::cout << str;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(InPath);
  /*  if (!result)
        return "";*/

    for (pugi::xml_node tool : doc.child("Main").child("Data").children(NodeName))
    {
        //Return first node attribute value
        return  tool.attribute(Attribute).value();
    }
    return "";
}
