//\====================================================================================
//\ Author: Christopher.Allport
//\ About : XMLHandler.h - 
//\ 
//\ Defines the XMLHandler Singleton Class. This class is used to handle the loading of 
//\ XML Files and given values back to other classes
//\====================================================================================
#ifndef _XML_H_
#define _XML_H_

#include <string>
#include <list>
#include "TinyXML.h"




class XMLHandler
{
/////////////////////////////////////////////////////////
public:
/////////////////////////////////////////////////////////
	
	static XMLHandler* GetInstance();
	
	//===============================================
	//			Constructors & Destructor
	//===============================================
	XMLHandler(std::string XML_RootFolderLocation);
	~XMLHandler();



	//===============================================
	//			Public Functions
	//===============================================
	void OnLoad();
	void OnUnload();
	
	/*bool LoadNavMeshDocument( FBXObject* FBXobj, std::string PathAndFilename );
	bool LoadNavMeshDocument( FBXObject* FBXobj );

	void WriteNavMeshDocument( FBXObject* FBXobj );*/



	//===============================================
	//			Getter Functions
	//===============================================
	bool Get_DoesNavMeshXMLExist( std::string Filename );


	//===============================================
	//			Setter Functions
	//===============================================
	void SetXMLDocumentLocation( std::string Location );



/////////////////////////////////////////////////////////
private:
/////////////////////////////////////////////////////////


	//===============================================
	//			Private Declarations
	//===============================================
	static XMLHandler* m_Instance;
	
	std::string m_sLocation;
	std::string m_sFilename;	// Filename changes with each XML being made/read, this is for debug purposes in which you can check if something isn't working correctly
	
	std::string m_sNavMeshSubFolder;
	

	//===============================================
	//			Private Functions
	//===============================================
	void SetClassVariablesToNil();

	//bool ReadAndWriteNavMeshDocumentToFBXObject( FBXObject* FBXobjToWriteTo, const char* DocumentFilename );

	template< class T >
	const char* ConvertAnythingToAString(T AnyValue);
};

#endif