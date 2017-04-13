// Assignment4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int READERROR = 100;
const int WRITEERROR = 200;
const int ARRAYSIZE = 100;

class Album
{
	string Name;
	string Artist;
	string Genre;
	int Rating;

public:
	Album();
	Album(string n, string a, string g, int r);
	void CaptureAlbum();
	void ShowAlbum();	
	int SaveAlbum(ofstream &outfile);
};

class ItemList
{
	Album **list;
	int numrecords;
	int listsize;
	int ReallocateArray();
	int check();
public: 
	ItemList();
	~ItemList();
	void GetInput();
	void ShowList();
	int SaveAlbumList(string filename);
	int ReadAlbumList(string filename);
	//delete album?

};

Album::Album()
{
	Name = ""; Artist = ""; Genre = ""; Rating = 0;
}

Album::Album(string n, string a, string g, int r)
{
	Name = n; Artist = a; Genre = g; Rating = r;
}

void Album::CaptureAlbum()
{
	cout << "Album Name: ";
	getline(cin, Name);
	cout << "Artist Name: ";
	getline(cin, Artist);
	cout << "Album Genre: ";
	getline(cin, Genre);
	cout << "Rating? (0-5)";
	cin >> Rating;

	cin.ignore();
	cin.clear();
}

void Album::ShowAlbum()
{
	cout << "" << endl;
	cout << "Album: " << Name << endl;
	cout << " Artist: " << Artist << endl;
	cout << " Genre: " << Genre << endl;
	cout << " Rating: " << Rating << endl;
}

int Album::SaveAlbum(ofstream &outfile)
{
	if (outfile.is_open())
	{
		outfile << Name << "|" << Artist << "|" << Genre << "|" << Rating << endl;
		return 0;
	}
	else
		return WRITEERROR;
}

ItemList::ItemList()
{
	list = new Album*[ARRAYSIZE];
	numrecords = 0;
	listsize = ARRAYSIZE;
}

ItemList::~ItemList()
{
	for (int i = 0; i < numrecords; i++)
	{
		delete list[i];
	}
	delete[]list;
}

int ItemList::ReallocateArray()
{
	Album **temp;
	temp = new Album*[listsize + ARRAYSIZE];
	listsize = listsize + ARRAYSIZE;
	for (int i = 0; i < numrecords; i++)
	{
		temp[i] = list[i];
	}
	delete[]list;
	list = temp;
	return 0;
}

void ItemList::GetInput()
{
	string answer = "Y";
	//check space
	cout << "Enter Album Y/N?" << endl;
	getline(cin, answer);
	while (answer == "Y" || answer == "y")
	{
		list[numrecords] = new Album();
		list[numrecords]->CaptureAlbum();
		numrecords++;
		cout << "Enter another album Y/N?" << endl;
		getline(cin, answer);
	}
}

void ItemList::ShowList()
{
	for (int i = 0; i < numrecords; i++)
	{
		list[i]->ShowAlbum();
	}
}

int ItemList::SaveAlbumList(string filename)
{
	ofstream output(filename, ios::trunc);
	bool duplicate = false;
	if (output)
	{
		for (int i = 0; i < numrecords; i++)
		{
			list[i]->SaveAlbum(output);
		}
	}
	else
	{
		return WRITEERROR;
	}
	return 0;
}



int ItemList::ReadAlbumList(string filename)
{
	string aname, aartist, agenre, arating;
	ifstream infile(filename, ios::in);
	if (!infile)
	{
		cout << "File not found" << endl;
		return READERROR;
	}

	while (!infile.eof())
	{
		if (numrecords == listsize)
		{
			ReallocateArray();
		}
		getline(infile, aname, '|');
		if (!aname.empty())
		{
			getline(infile, aartist, '|');
			getline(infile, agenre, '|');
			getline(infile, arating);
			int rating = stoi(arating);
			list[numrecords] = new Album(aname, aartist, agenre, rating);
			numrecords++;
		}
	}

	infile.close();
	return 0;
}

int main()
{
	string filename;
	cout << "Enter full-path filename: ";
	getline(cin, filename);
	ItemList myList;
	string answer;
	int error;
	error = myList.ReadAlbumList(filename);
	if (error)
	{
		cout << "Cannot read list - Create new list? Y/N -->";
		getline(cin, answer);
		if (answer != "Y" && answer != "y")
		{
			return -1;
		}
	}
	myList.GetInput();
	myList.SaveAlbumList(filename);
	myList.ShowList();

    return 0;
}

