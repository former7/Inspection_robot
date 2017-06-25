#include <string>
#include <iostream>
#include "tinyxml.h"
#include <stdlib.h>
#include "RouteArrayManager.h"
void printXml();
int readXml();
void writeXml();
//extern RouteArrayManager route;
extern RouteArrayManager route;
using namespace std;
int XMLinit() {
	//printXml();
	readXml();//解析规划路径XML
	//writeXml();
	return 0;
}

void printXml() {
	using namespace std;
	TiXmlDocument doc;  
	const char * xmlFile = "./conf/receive.xml";	
	if (doc.LoadFile(xmlFile)) {  	
		doc.Print();  
	} else {
		cout << "can not parse xml conf/school.xml" << endl;
	}
}

int readXml() {
	using namespace std;
	const char * xmlFile = "./conf/receive.xml";	
	TiXmlDocument doc;  
	if (doc.LoadFile(xmlFile)) {  	
		doc.Print();  
	} else {
		cout << "can not parse xml conf/receive.xml" << endl;
		return 0;
	}
	TiXmlElement* rootElement = doc.RootElement();  //root元素  
	TiXmlElement* routeElement = rootElement->FirstChildElement();  //路径点  
	int temppoint[10];
	//RoutePoint *p;
	for (int j=0; routeElement != NULL; routeElement = routeElement->NextSiblingElement() ,	j++) {  
		TiXmlElement* informationElement = routeElement->FirstChildElement();//
		for (int i=0; informationElement != NULL; informationElement = informationElement->NextSiblingElement(),i++ ) {
			//string contactType = informationElement->Value();
			string contactValue = informationElement->GetText();
			temppoint[i]=atoi(contactValue.c_str());
		}
		if(route.addRoutePointAtTail(temppoint[0],temppoint[1],temppoint[2],temppoint[3],temppoint[4],temppoint[5])!=0)return 0;
	    //cout<<route->getRoutePointIndex(j)->x<<" "<<route->getRoutePointIndex(j)->y<<endl;
	}  
	return 1;
}


void writeXml() {
	using namespace std;
	const char * xmlFile = "./conf/school-write.xml";	
	TiXmlDocument doc;  
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "", "");  
	TiXmlElement * schoolElement = new TiXmlElement( "School" );  
	TiXmlElement * classElement = new TiXmlElement( "Class" );  
	classElement->SetAttribute("name", "C++");

	TiXmlElement * stu1Element = new TiXmlElement("Student");
	stu1Element->SetAttribute("name", "tinyxml");
	stu1Element->SetAttribute("number", "123");
	TiXmlElement * stu1EmailElement = new TiXmlElement("email");
	stu1EmailElement->LinkEndChild(new TiXmlText("tinyxml@163.com") );
	TiXmlElement * stu1AddressElement = new TiXmlElement("address");
	stu1AddressElement->LinkEndChild(new TiXmlText("中国"));
	stu1Element->LinkEndChild(stu1EmailElement);
	stu1Element->LinkEndChild(stu1AddressElement);

	TiXmlElement * stu2Element = new TiXmlElement("Student");
	stu2Element->SetAttribute("name", "jsoncpp");
	stu2Element->SetAttribute("number", "456");
	TiXmlElement * stu2EmailElement = new TiXmlElement("email");
	stu2EmailElement->LinkEndChild(new TiXmlText("jsoncpp@163.com"));
	TiXmlElement * stu2AddressElement = new TiXmlElement("address");
	stu2AddressElement->LinkEndChild(new TiXmlText("美国"));
	stu2Element->LinkEndChild(stu2EmailElement);
	stu2Element->LinkEndChild(stu2AddressElement);

	classElement->LinkEndChild(stu1Element);  
	classElement->LinkEndChild(stu2Element);  
	schoolElement->LinkEndChild(classElement);  
	
	doc.LinkEndChild(decl);  
	doc.LinkEndChild(schoolElement); 
	doc.SaveFile(xmlFile);  
}
