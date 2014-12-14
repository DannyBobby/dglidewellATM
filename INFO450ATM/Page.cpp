#include "Page.h"


Page::Page()
{
}

Page::~Page()
{
}

// Return the string at the specified "line"
// or index within the vector
string Page::GetLine(unsigned int lineNumber)
{
	// Check to see if it is possible for there to be
	// anything "written" on this "lineNumber" by seeing
	// if the lineNumber is less than the size of the vector.
	if (lineNumber < collectionOfLines.size())
	{
		// If so, return the string stored at this index
		return collectionOfLines[lineNumber];
	}
	// If it isn't possible for there to be anything there...
	else
	{
		// Return the empty string
		return "";
	}
}

unsigned int Page::GetMaximumNumberOfLines()
{
	return this->maximumNumberOfLines;
}

// Return the number of lines currently "written"
// to the page (the size of the vector).
unsigned int Page::GetNumberOfLines()
{
	return this->collectionOfLines.size();
}

// Add a line to the page
void Page::AddLine(string lineText)
{
	// If the number of lines on the page has not yet reached
	// the maximum allowable number...
	if (collectionOfLines.size() < (maximumNumberOfLines + 1))
	{
		// Add the new line to the page.
		collectionOfLines.push_back(lineText);
	}
}