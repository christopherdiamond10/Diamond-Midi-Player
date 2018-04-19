#include "XMLHandler.h"
#include <assert.h>
#include <sstream>

#include "HelperFunctions.h"

XMLHandler* XMLHandler::m_Instance = nullptr;
//==============================================
//			Get XMLHandler Instance
//==============================================
XMLHandler* XMLHandler::GetInstance()
{
	return m_Instance;
}


//==============================================
//		   Constructor & Destructor
//==============================================
XMLHandler::XMLHandler(std::string a_sXML_RootFolderLocation)
{
	if( m_Instance == nullptr)
	{ 
		m_Instance = this; 

		m_sLocation = a_sXML_RootFolderLocation;
		m_sFilename = " ";
	} 
	else 
	{ 
		assert(0); 
	}
}

XMLHandler::~XMLHandler()
{
	m_Instance = nullptr;
}



//==============================================
//			OnLoad
//==============================================
void XMLHandler::OnLoad()
{
	m_sNavMeshSubFolder = "NavMesh/";

	SetClassVariablesToNil();
}

//==============================================
//			OnUnload
//==============================================
void XMLHandler::OnUnload()
{

}










//==============================================
//			Does NavMesh XML Exist?
//==============================================
bool XMLHandler::Get_DoesNavMeshXMLExist( std::string a_sFilename )
{
	std::string sXML = m_sLocation + m_sNavMeshSubFolder + a_sFilename;


	TiXmlDocument XMLDoc( sXML.c_str() );
	return XMLDoc.LoadFile();
}







//==============================================
//			Set Document Location
//==============================================
void XMLHandler::SetXMLDocumentLocation( std::string sLocation )
{
	m_sLocation = sLocation;
}



//==============================================
//		  Set Class Variables To Nil
//==============================================
void XMLHandler::SetClassVariablesToNil()
{
	
}




//==============================================
//		   Write NavMesh Document
//==============================================
//void XMLHandler::WriteNavMeshDocument()
//{
	//// Assign Local Filename
	//m_sFilename = a_pFBXobj->m_sSceneName;

	//// Make XML Variables
	//TiXmlDocument* XMLDoc = new TiXmlDocument();
 //   TiXmlDeclaration* XMLDeclaration = new TiXmlDeclaration( "1.0", "utf-8", "");
 //   
	//// Add Declaration
	//XMLDoc->LinkEndChild( XMLDeclaration );

	//// Create Vertex Information for Vertices
 //   TiXmlElement* RootElement = new TiXmlElement( "NavMeshInfo" );  
	//	TiXmlElement* VerticesInformationElement = new TiXmlElement( "VerticesInformation" );
	//	VerticesInformationElement->SetAttribute( "TotalVertices", (int)a_pFBXobj->m_NavMeshInfo.aNavMeshVertices.size() );
	//		for each(auto VertexInfo in a_pFBXobj->m_NavMeshInfo.aNavMeshVertices)
	//		{
	//			TiXmlElement* VertexInfoElement = new TiXmlElement( "VertexInfo" );

	//				TiXmlElement* VertexIDElement = new TiXmlElement( "VertexID" );
	//					VertexIDElement->SetAttribute( "ID", (int)VertexInfo.uiVertexID );
	//				VertexInfoElement->LinkEndChild( VertexIDElement );

	//				TiXmlElement* VertexCostElement = new TiXmlElement( "VertexCost" );
	//					VertexCostElement->SetAttribute( "Cost", (int)VertexInfo.uiCost );
	//				VertexInfoElement->LinkEndChild( VertexCostElement );

	//				TiXmlElement* VertexPassableElement = new TiXmlElement( "VertexPassable" );
	//					VertexPassableElement->SetAttribute( "Passable", VertexInfo.bPassable ? "1" : "0" );
	//				VertexInfoElement->LinkEndChild( VertexPassableElement );

	//				TiXmlElement* VertexIndicesElement = new TiXmlElement( "VertexIndices" );
	//					TiXmlElement* VertexIndicesOneElement = new TiXmlElement( "One" );
	//						VertexIndicesOneElement->SetDoubleAttribute( "X", (double)VertexInfo.v4Indices[0].x );
	//						VertexIndicesOneElement->SetDoubleAttribute( "Y", (double)VertexInfo.v4Indices[0].y );
	//						VertexIndicesOneElement->SetDoubleAttribute( "Z", (double)VertexInfo.v4Indices[0].z );
	//						VertexIndicesOneElement->SetDoubleAttribute( "W", (double)VertexInfo.v4Indices[0].w );
	//					VertexIndicesElement->LinkEndChild( VertexIndicesOneElement );
	//					TiXmlElement* VertexIndicesTwoElement = new TiXmlElement( "Two" );
	//						VertexIndicesTwoElement->SetDoubleAttribute( "X", (double)VertexInfo.v4Indices[1].x );
	//						VertexIndicesTwoElement->SetDoubleAttribute( "Y", (double)VertexInfo.v4Indices[1].y );
	//						VertexIndicesTwoElement->SetDoubleAttribute( "Z", (double)VertexInfo.v4Indices[1].z );
	//						VertexIndicesTwoElement->SetDoubleAttribute( "W", (double)VertexInfo.v4Indices[1].w );
	//					VertexIndicesElement->LinkEndChild( VertexIndicesTwoElement );
	//					TiXmlElement* VertexIndicesThreeElement = new TiXmlElement( "Three" );
	//						VertexIndicesThreeElement->SetDoubleAttribute( "X", (double)VertexInfo.v4Indices[2].x );
	//						VertexIndicesThreeElement->SetDoubleAttribute( "Y", (double)VertexInfo.v4Indices[2].y );
	//						VertexIndicesThreeElement->SetDoubleAttribute( "Z", (double)VertexInfo.v4Indices[2].z );
	//						VertexIndicesThreeElement->SetDoubleAttribute( "W", (double)VertexInfo.v4Indices[2].w );
	//					VertexIndicesElement->LinkEndChild( VertexIndicesThreeElement );
	//				VertexInfoElement->LinkEndChild( VertexIndicesElement );

	//				TiXmlElement* VertexPositionElement = new TiXmlElement( "VertexPosition" );
	//					VertexPositionElement->SetDoubleAttribute( "X", (double)VertexInfo.v4CentreVertexPosition.x );
	//					VertexPositionElement->SetDoubleAttribute( "Y", (double)VertexInfo.v4CentreVertexPosition.y );
	//					VertexPositionElement->SetDoubleAttribute( "Z", (double)VertexInfo.v4CentreVertexPosition.z );
	//					VertexPositionElement->SetDoubleAttribute( "W", (double)VertexInfo.v4CentreVertexPosition.w );
	//				VertexInfoElement->LinkEndChild( VertexPositionElement );

	//				TiXmlElement* VertexSurroundingVerticesElement = new TiXmlElement( "VertexSurroundingVertices" );
	//				VertexSurroundingVerticesElement->SetAttribute( "TotalSurroundingVertices", (int)VertexInfo.auiSurroundingVertices.size() );
	//					for each(auto VertexID in VertexInfo.auiSurroundingVertices)
	//					{
	//						TiXmlElement* SurroundingVertexIDElement = new TiXmlElement( "SurroundingVertexID" );
	//							SurroundingVertexIDElement->SetAttribute( "ID", (int)VertexID );
	//						VertexSurroundingVerticesElement->LinkEndChild( SurroundingVertexIDElement );
	//					}
	//				VertexInfoElement->LinkEndChild( VertexSurroundingVerticesElement );

	//			VerticesInformationElement->LinkEndChild( VertexInfoElement );
	//		}
	//RootElement->LinkEndChild( VerticesInformationElement );
 //   XMLDoc->LinkEndChild( RootElement );  

	//// Create Path to XML Document
	//std::string sPathToXML = m_sLocation + m_sNavMeshSubFolder + HelperFunctions::ReplaceStringCharacters( m_sFilename, a_pFBXobj->m_bIsAIEFile ? ".AIE" : ".FBX", ".xml" );

	//// Save Document
 //   XMLDoc->SaveFile( sPathToXML.c_str() );

	//delete XMLDoc;
//}



//==============================================
//		Read and Write NavMesh Document
//==============================================
//bool XMLHandler::ReadAndWriteNavMeshDocumentToFBXObject( FBXObject* a_pFBXobj, const char* a_cFilename )
//{
	//TiXmlDocument* pXMLDoc = new TiXmlDocument( a_cFilename );

	//// Load XML
	//if( pXMLDoc != NULL && pXMLDoc->LoadFile() )
	//{
	//	TiXmlElement* pRoot = pXMLDoc->FirstChildElement( "NavMeshInfo" );
	//	if ( pRoot != NULL )
	//	{
	//		TiXmlElement* pVerticesInformation = pRoot->FirstChildElement( "VerticesInformation" );
	//		if( pVerticesInformation != NULL )
	//		{
	//			unsigned int uiTotalVertices = (unsigned int)atoi(pVerticesInformation->Attribute( "TotalVertices" ));	// Get Total Vertices
	//			a_pFBXobj->m_NavMeshInfo.aNavMeshVertices.resize(uiTotalVertices, FBXObject::NavMeshVertex());			// Resize FBX NavMesh Array to Compensate
	//			TiXmlElement* pVertexInfo = pVerticesInformation->FirstChildElement( "VertexInfo" );					// Get First Vertex Info
	//			for(unsigned int i = 0; i < uiTotalVertices; ++i)
	//			{
	//				if(pVertexInfo != NULL)
	//				{
	//					TiXmlElement* pVertexID = pVertexInfo->FirstChildElement( "VertexID" );
	//					if(pVertexID != NULL )
	//					{
	//						a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].uiVertexID = (unsigned int)atoi(pVertexID->Attribute( "ID" ));
	//					}

	//					TiXmlElement* pVertexCost = pVertexInfo->FirstChildElement( "VertexCost" );
	//					if(pVertexCost != NULL )
	//					{
	//						a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].uiCost = (unsigned int)atoi(pVertexCost->Attribute( "Cost" ));
	//					}

	//					TiXmlElement* pVertexPassable = pVertexInfo->FirstChildElement( "VertexPassable" );
	//					if(pVertexPassable != NULL)
	//					{
	//						a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].bPassable = (strcmp(pVertexPassable->Attribute("Passable"), "1") == 0);
	//					}

	//					TiXmlElement* pVertexIndices = pVertexInfo->FirstChildElement( "VertexIndices" );
	//					if(pVertexIndices != NULL )
	//					{
	//						TiXmlElement* pVertexIndiceOne		= pVertexIndices->FirstChildElement( "One" );
	//						TiXmlElement* pVertexIndiceTwo		= pVertexIndices->FirstChildElement( "Two" );
	//						TiXmlElement* pVertexIndiceThree	= pVertexIndices->FirstChildElement( "Three" );
	//						if(pVertexIndiceOne != NULL)
	//						{
	//							a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].v4Indices[0].x = (float)atof(pVertexIndiceOne->Attribute("X"));
	//							a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].v4Indices[0].y = (float)atof(pVertexIndiceOne->Attribute("Y"));
	//							a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].v4Indices[0].z = (float)atof(pVertexIndiceOne->Attribute("Z"));
	//							a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].v4Indices[0].w = (float)atof(pVertexIndiceOne->Attribute("W"));
	//						}
	//						if(pVertexIndiceTwo != NULL)
	//						{
	//							a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].v4Indices[1].x = (float)atof(pVertexIndiceTwo->Attribute("X"));
	//							a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].v4Indices[1].y = (float)atof(pVertexIndiceTwo->Attribute("Y"));
	//							a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].v4Indices[1].z = (float)atof(pVertexIndiceTwo->Attribute("Z"));
	//							a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].v4Indices[1].w = (float)atof(pVertexIndiceTwo->Attribute("W"));
	//						}
	//						if(pVertexIndiceThree != NULL)
	//						{
	//							a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].v4Indices[2].x = (float)atof(pVertexIndiceThree->Attribute("X"));
	//							a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].v4Indices[2].y = (float)atof(pVertexIndiceThree->Attribute("Y"));
	//							a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].v4Indices[2].z = (float)atof(pVertexIndiceThree->Attribute("Z"));
	//							a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].v4Indices[2].w = (float)atof(pVertexIndiceThree->Attribute("W"));
	//						}
	//					}

	//					TiXmlElement* pVertexPosition = pVertexInfo->FirstChildElement( "VertexPosition" );
	//					if(pVertexPosition != NULL)
	//					{
	//						a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].v4CentreVertexPosition.x = (float)atof(pVertexPosition->Attribute("X"));
	//						a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].v4CentreVertexPosition.y = (float)atof(pVertexPosition->Attribute("Y"));
	//						a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].v4CentreVertexPosition.z = (float)atof(pVertexPosition->Attribute("Z"));
	//						a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].v4CentreVertexPosition.w = (float)atof(pVertexPosition->Attribute("W"));
	//					}

	//					TiXmlElement* pVertexSurroundingVertices = pVertexInfo->FirstChildElement( "VertexSurroundingVertices" );
	//					if(pVertexSurroundingVertices != NULL)
	//					{
	//						unsigned int uiTotalSurroundingVertices = (unsigned int)atoi(pVertexSurroundingVertices->Attribute( "TotalSurroundingVertices" ));
	//						a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].auiSurroundingVertices.resize(uiTotalSurroundingVertices); // Resize Array
	//						TiXmlElement* pSurroundingVertexID = pVertexSurroundingVertices->FirstChildElement( "SurroundingVertexID" );
	//						for( unsigned int j = 0; j < uiTotalSurroundingVertices; ++j )
	//						{
	//							if(pSurroundingVertexID != NULL)
	//								a_pFBXobj->m_NavMeshInfo.aNavMeshVertices[i].auiSurroundingVertices[j] = (unsigned int)atoi(pSurroundingVertexID->Attribute( "ID" ));
	//							else
	//								break;

	//							pSurroundingVertexID = pSurroundingVertexID->NextSiblingElement( "SurroundingVertexID" );
	//						}
	//					}
	//				}
	//				else
	//				{
	//					return false;
	//				}

	//				pVertexInfo = pVertexInfo->NextSiblingElement( "VertexInfo" );
	//			}
	//			return true;
	//		}
	//	}
	//}
	//return false;
//}



//==============================================
//		Convert Anything to a String
//==============================================
template< class T >
const char* XMLHandler::ConvertAnythingToAString(T AnyValue)
{
	// Converting from any non-string value to a string ...
	std::stringstream cString (std::stringstream::in | std::stringstream::out);
	cString << AnyValue;
	return cString.str().c_str();
}