#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

namespace PolygonalLibrary{
	bool ImportMesh(PolygonalMesh& mesh){
		if(!ImportCell0Ds(mesh))
			return false;
		if(!ImportCell1Ds(mesh))
			return false;
		if(!ImportCell2Ds(mesh))
			return false;
		return true;
	}
	
	bool ImportCell0Ds(PolygonalMesh& mesh){
		ifstream file ("./Cell0Ds.csv");
		if(file.fail())
			return false;
		string primariga;
		getline(file,primariga); //scartata prima riga
		list<string> ListLines;
		string line;
		while(getline(file, line))
			ListLines.push_back(line);
		file.close(); //così ho lista con tutti dati, in ordine Id,Marker,X,Y
		mesh.NumCell0Ds = ListLines.size();
		if (mesh.NumCell0Ds == 0)
		{
			cerr << "There is no cell 0D" << endl;
			return false;
		}
		mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
		mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds);
		for (string& line : ListLines){
			replace(line.begin(), line.end(), ';', ' ');
			istringstream converter(line);
			unsigned int id;
			unsigned int marker;
			converter >>  id >> marker >> mesh.Cell0DsCoordinates(0, id) >> mesh.Cell0DsCoordinates(1, id);
			mesh.Cell0DsId.push_back(id);	
			//memorizziamo solo marker non nulli
			if(marker!=0){
				auto it = mesh.MarkerCell0Ds.find(marker);
				if(it!=mesh.MarkerCell0Ds.end())
					mesh.MarkerCell0Ds[marker].push_back(id);
				else
					mesh.MarkerCell0Ds.insert({marker,{id}});
			}
		}
			return true;
	}
	
	bool ImportCell1Ds(PolygonalMesh& mesh){
		ifstream file("./Cell1Ds.csv");
		if(file.fail())
			return false;
		string primariga;
		getline(file,primariga); //scartata prima riga
		list<string> ListLines;
		string line;
		while(getline(file,line))
			ListLines.push_back(line);
		file.close(); //lista di stringhe con dati, in ordine Id,marker,origin,end
		mesh.NumCell1Ds = ListLines.size();
		if (mesh.NumCell1Ds == 0)
		{
			cerr << "There is no cell 1D" << endl;
			return false;
		}
		mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
		mesh.Cell1DsExtrema = Eigen::MatrixXi(3, mesh.NumCell1Ds);
		for(string& line: ListLines){
			replace(line.begin(),line.end(),';',' ');
			istringstream converter(line);
			unsigned int id;
			unsigned int marker;
			converter >> id >> marker >> mesh.Cell1DsExtrema(0,id) >> mesh.Cell1DsExtrema(1,id);
			mesh.Cell1DsId.push_back(id);
			if(marker!=0){
				auto it = mesh.MarkerCell1Ds.find(marker);
				if(it!=mesh.MarkerCell1Ds.end())
					mesh.MarkerCell1Ds[marker].push_back(id);
				else
					mesh.MarkerCell1Ds.insert({marker,{id}});
			}
		}
		return true;
	}
	
	bool ImportCell2Ds(PolygonalMesh& mesh){
		ifstream file ("./Cell2Ds.csv");
		if(file.fail())
			return false;
		string primariga;
		getline(file,primariga); //scartata prima riga
		list<string> ListLines;
		string line;
		while(getline(file,line))
			ListLines.push_back(line);
		file.close();//creata lista di stringhe con dati in ciascuna riga, in ordine Id,marker,NumVertices,Vertices,NumEdges,Edges
		mesh.NumCell2Ds = ListLines.size();
		if(mesh.NumCell2Ds == 0){
			cerr << "there is no cell 2D" << endl;
			return false;
		}
		mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
		mesh.NumVerticesCell2Ds.reserve(mesh.NumCell2Ds);
		mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
		mesh.NumEdgesCell2Ds.reserve(mesh.NumCell2Ds);
		mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);
		for(string& line : ListLines){
			replace(line.begin(),line.end(),';',' ');
			istringstream converter(line);
			unsigned int id;
			unsigned int marker;
			unsigned int NumVertices;
			unsigned int NumEdges;
			converter >> id >> marker >> NumVertices;
			vector<unsigned int> Vertices;
			Vertices.reserve(NumVertices);
			for(unsigned int i =0; i<NumVertices;i++){
				unsigned int v;
				converter >> v;
				Vertices.push_back(v);
			}
			converter >> NumEdges;
			vector<unsigned int> Edges;
			Edges.reserve(NumEdges);
			for(unsigned int i =0; i<NumEdges;i++){
				unsigned int e;
				converter >> e;
				Edges.push_back(e);
			}
			mesh.Cell2DsId.push_back(id);
			mesh.NumVerticesCell2Ds.push_back(NumVertices);
			mesh.Cell2DsVertices.push_back(Vertices);
			mesh.NumEdgesCell2Ds.push_back(NumEdges);
			mesh.Cell2DsEdges.push_back(Edges);
			if(marker!=0){
				auto it = mesh.MarkerCell2Ds.find(marker);
				if(it!=mesh.MarkerCell2Ds.end())
					mesh.MarkerCell2Ds[marker].push_back(id);
				else
					mesh.MarkerCell2Ds.insert({marker,{id}});	
			}
		}
		return true;
	}
}