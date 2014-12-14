// This object is used in the construction of reports - Transaction/Transfer History

#ifndef PAGE_H
#define PAGE_H

#include <string>
#include <vector>

using namespace std;

#pragma once
class Page
{
private:
	// Each page has a maximum number of lines so that 
	// it all fits in one console window without scrolling
	unsigned int maximumNumberOfLines = 10;

	// This vector stores individual strings which make up 
	// the lines displayed on the page.
	vector<string> collectionOfLines;

public:
	Page();
	~Page();

	// Get functions
	unsigned int GetMaximumNumberOfLines();
	string GetLine(unsigned int lineNumber);
	// This is different from GetMaximumNumberOfLines()
	// because it returns the number of lines which the 
	// page currently has "written on" it.
	unsigned int GetNumberOfLines();

	// Add a line to the page
	void AddLine(string lineText);
};

#endif