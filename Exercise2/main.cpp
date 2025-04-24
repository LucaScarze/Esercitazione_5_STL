#include <iostream>
#include <fstream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

int main()
{
    PolygonalMesh mesh;

    if(!ImportMesh(mesh))
    {
        cerr << "file not found" << endl;
        return 1;
    }
	
	//controllo non ci siano lati di lunghezza nulla
	bool lati_ok = true;
	for(unsigned int j = 0; j<mesh.NumCell1Ds;j++){
		unsigned int id_origine = mesh.Cell1DsExtrema(0,j);
		unsigned int id_fine = mesh.Cell1DsExtrema(1,j);
		double delta_x = mesh.Cell0DsCoordinates(0,id_origine)-mesh.Cell0DsCoordinates(0,id_fine);
		double delta_y =  mesh.Cell0DsCoordinates(1,id_origine)-mesh.Cell0DsCoordinates(1,id_fine);
		double lunghezza = sqrt(delta_x*delta_x + delta_y*delta_y);
		if(lunghezza < 1e-12){
			lati_ok = false;
			break;
		}
	}
	if(lati_ok == false){
		cout << "c'è almeno un lato di lunghezza nulla" << endl;
		return 1;
	}
	else
		cout << "tutti i lati hanno lunghezza diversa da zero" << endl;
	
	//controllo non ci siano poligoni di area nulla
	bool area_ok = true;
	for(unsigned int j=0;j<mesh.NumCell2Ds;j++){
		unsigned int n = (mesh.Cell2DsVertices[j]).size();
		vector<unsigned int> id_vertici;
		id_vertici.reserve(n);
		id_vertici = mesh.Cell2DsVertices[j];
		double area = 0;
		for(unsigned int i=0;i<(n-1);i++)
			area += 0.5*abs(mesh.Cell0DsCoordinates(0,id_vertici[i])*mesh.Cell0DsCoordinates(1,id_vertici[i+1])-mesh.Cell0DsCoordinates(0,id_vertici[i+1])*mesh.Cell0DsCoordinates(1,id_vertici[i]));
		area += 0.5*abs(mesh.Cell0DsCoordinates(0,id_vertici[n-1])*mesh.Cell0DsCoordinates(1,id_vertici[0])-mesh.Cell0DsCoordinates(0,id_vertici[0])*mesh.Cell0DsCoordinates(1,id_vertici[n-1]));
		if(area < 1e-24){
			 area_ok = false;
			 break;
		}
	}
	if(area_ok==false){
		cout << "c'è almeno un poligono di area nulla" << endl;
		return 1;
	}
	else
		cout << "tutti i poligoni hanno area non nulla" << endl;
	
	//controllo dei marker celle 0d
	bool marker_0d_ok = true;
	ifstream file("./Cell0Ds.csv");
	string primariga;
	getline(file,primariga); //scartata prima riga
	list<string> ListLines;
	string line;
	while(getline(file, line))
		ListLines.push_back(line);
	file.close(); //lista con tutte righe
	for (string& line : ListLines){
			replace(line.begin(), line.end(), ';', ' ');
			istringstream converter(line);
			unsigned int id;
			unsigned int marker;
			converter >>  id >> marker;
			if(marker!=0){
				auto it = mesh.MarkerCell0Ds.find(marker);
				if(it==mesh.MarkerCell0Ds.end()){
					marker_0d_ok = false;
					break;
				}
			}
	}
	if(marker_0d_ok== false){
		cout << "errore in memorizzazione marker celle 0d"<<endl;
		return 1;
	}
	else
		cout<<"nessun problema nella memorizzazione dei marker 0d"<<endl;
	
	//controllo dei marker celle 1d
	bool marker_1d_ok = true;
	ifstream file_1("./Cell1Ds.csv");
	string primariga_1;
	getline(file_1,primariga_1); //scartata prima riga
	list<string> ListLines_1;
	string line_1;
	while(getline(file_1, line_1))
		ListLines_1.push_back(line_1);
	file_1.close(); //lista con tutte righe
	for (string& line_1 : ListLines_1){
		replace(line_1.begin(), line_1.end(), ';', ' ');
		istringstream converter(line_1);
		unsigned int id;
		unsigned int marker;
		converter >>  id >> marker;
		if(marker!=0){
			auto it = mesh.MarkerCell1Ds.find(marker);
			if(it==mesh.MarkerCell1Ds.end()){
				marker_1d_ok = false;
				break;
			}
		}
	}
	if(marker_1d_ok== false){
		cout << "errore in memorizzazione marker celle 1d"<<endl;
		return 1;
	}
	else
		cout<<"nessun problema nella memorizzazione dei marker 1d"<<endl;
	
	//controllo dei marker celle 2d
	bool marker_2d_ok = true;
	ifstream file_2("./Cell2Ds.csv");
	string primariga_2;
	getline(file_2,primariga_2); //scartata prima riga
	list<string> ListLines_2;
	string line_2;
	while(getline(file_2, line_2))
		ListLines_2.push_back(line_2);
	file_2.close(); //lista con tutte righe
	for (string& line_2 : ListLines_2){
			replace(line_2.begin(), line_2.end(), ';', ' ');
			istringstream converter(line_2);
			unsigned int id;
			unsigned int marker;
			converter >>  id >> marker;
			if(marker!=0){
				auto it = mesh.MarkerCell2Ds.find(marker);
				if(it==mesh.MarkerCell2Ds.end()){
					marker_2d_ok = false;
					break;
				}
			}
	}
	if(marker_2d_ok== false){
		cout << "errore in memorizzazione marker celle 2d"<<endl;
		return 1;
	}
	else
		cout<<"nessun problema nella memorizzazione dei marker 2d"<<endl;
	
	

    Gedim::UCDUtilities utilities;
    utilities.ExportPoints("./Cell0Ds.inp",
                           mesh.Cell0DsCoordinates);

    utilities.ExportSegments("./Cell1Ds.inp",
                             mesh.Cell0DsCoordinates,
                             mesh.Cell1DsExtrema);					 
						 

    return 0;
}

