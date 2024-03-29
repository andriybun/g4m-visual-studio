#ifndef STRING_UTILS_H_
#define STRING_UTILS_H_

#include <string>
#include <vector>

using namespace std;

enum isTagT
{
	IS_TAG,
	IS_NOT_TAG,
	SAME_AS_BEFORE
};

int RemoveComments(string fileName, string outFileName)
{
	ifstream file;
	ofstream outFile;
	file.open(fileName.c_str(), ios::in);
	outFile.open(outFileName.c_str(), ios::out);

	if (file.is_open())
	{
		string line;
		while(!file.eof())
		{
			getline(file, line);
			size_t slashPos = line.find("/");
			if (slashPos != string::npos)
			{
				if (line[slashPos+1]=='/')
				{
					// Remove single-line comments
					line = line.substr(0, slashPos);
				}
				else if (line[slashPos+1]=='*')
				{
					// Remove other comments
					string lineBeforeComment = line.substr(0, slashPos);
					outFile.write(lineBeforeComment.c_str(), lineBeforeComment.length());
					line = line.substr(slashPos, line.length()-1);
					size_t commentEnd = line.find("*/");
					while (commentEnd == string::npos)
					{
						if (file.eof()) return 0;
						getline(file, line);
						commentEnd = line.find("*/");
					}
					line = line.substr(commentEnd+2, line.length()-1);
				}
			}
			line += "\n";
			outFile.write(line.c_str(), line.length());
		}

		file.close();
		outFile.close();
	}
	else
	{
		printf("Error! Unable to open file: %s\n", fileName.c_str());
		return 2;
	}

	return 0;
}

bool FindAndReplace(string &base, const string & strToReplace, const string & strReplacement)
{
	if (base.find(strToReplace) != string::npos)
	{
		base.replace(base.find(strToReplace), strToReplace.size(), strReplacement);
		return true;
	}
	return false;
}

void FindAndReplaceAll(string &base, const string & strToReplace, const string & strReplacement)
{
	while (FindAndReplace(base, strToReplace, strReplacement)) {}
}


void SurroundMarksWithSpaces(const string & marks, string & str)
{
	for (int i = 0; i < marks.length(); i++)
	{
		string replacement = " ";
		replacement += marks[i];
		replacement += " ";
		size_t pos = str.find(marks[i], 0);
		while (pos != string::npos)
		{
			str.replace(pos, 1, replacement);
			pos = str.find(marks[i], pos+2);
		}
	}
}

void Tokenize(const string& str,
			  vector<string>& tokens,
			  const string& delimiters = " ")
{
	// Skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		if (string::npos != lastPos)
		{
			if (str[lastPos] != '\"')
			{
				pos = str.find_first_of(delimiters, lastPos);
			}
			else
			{
				pos = str.find_first_of("\"", lastPos+1)+1;
			}
		}
		else
		{
			pos = string::npos;
		}
	}
}

void Tokenize(const string & str,
			  vector<string> & tokens,
			  vector<isTagT> & tokensSwitchTag,
			  const string & delimiters = " ")
{
	// Skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	string::size_type pos     = str.find_first_of(delimiters, lastPos);
	isTagT isTag;

	while (string::npos != pos || string::npos != lastPos)
	{
		if (string::npos != pos && string::npos != lastPos)
		{
			if ((str[lastPos-1] == '<') && (str[pos] == '>'))
			{
				isTag = IS_TAG;
			}
			else
			{
				isTag = IS_NOT_TAG;
			}
		}
		else
		{
			isTag = IS_NOT_TAG;
		}
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		tokensSwitchTag.push_back(isTag);
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

#endif