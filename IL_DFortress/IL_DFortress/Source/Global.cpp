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
	//Make xml file
	pugi::xml_document doc;
	//Read XML file from path
	pugi::xml_parse_result result = doc.load_file(InPath);

	//Loop children and return attribute value for first node it finds
	for (pugi::xml_node tool : doc.child("Main").child("Data").children(NodeName))
	{
		//Return first node attribute value
		return  tool.attribute(Attribute).value();
	}
	return "";
}

std::string ReadXML(const char InPath[], const char NodeName[], const char NodeChildren[], const char Attribute[])
{
	//Make xml file
	pugi::xml_document doc;
	//Read XML file from path
	pugi::xml_parse_result result = doc.load_file(InPath);

	//Loop children and return attribute value for first node it finds
	for (pugi::xml_node tool : doc.child("NodeName").children(NodeChildren))
	{
		//Return first node attribute value
		return  tool.attribute(Attribute).value();
	}
	return "";
}

const std::vector<std::string> ExplodeString(const std::string& s, const char& c)
{
	//String buffer
	std::string buff{ "" };
	//vector to collect strings in
	vector<std::string> v;

	//Loop string and add to vector
	for (auto n : s)
	{
		if (n != c) buff += n; else
			if (n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if (buff != "") v.push_back(buff);

	return v;
}
