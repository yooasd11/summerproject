#include "XMLParser.h"
#include <queue>

tinyxml2::XMLElement* XMLParser::findElement(const char* nodename){
	tinyxml2::XMLElement* first = this->doc.FirstChildElement();
	if (first == nullptr) return nullptr;
	std::queue<tinyxml2::XMLElement*> q;
	q.push(first);
	while (!q.empty()){
		tinyxml2::XMLElement* here = q.front();
		q.pop();
		if (here->NextSiblingElement() != nullptr) q.push(here->NextSiblingElement());
		if (here->FirstChildElement() != nullptr) q.push(here->FirstChildElement());
		if (!std::strcmp(here->Name(), nodename)) return here;
	}
	return nullptr;
}

tinyxml2::XMLElement* XMLParser::getChildByID(const char* nodename, const char* ID){
	tinyxml2::XMLElement* eRes = findElement(nodename);
	if (eRes == nullptr) return nullptr;
	eRes = eRes->FirstChildElement();
	while (eRes != nullptr){
		if (!std::strcmp(ID, eRes->Attribute("id")))
			return eRes;
		eRes = eRes->NextSiblingElement();
	}
	return nullptr;
}

tinyxml2::XMLElement* XMLParser::getFirstChildByName(const char* nodename){
	tinyxml2::XMLElement* eRes = findElement(nodename);
	if (eRes == nullptr) return nullptr;
	return eRes->FirstChildElement();
}