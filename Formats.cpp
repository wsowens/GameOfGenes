#include "Formats.h"

BoardData loadLife(string filename)
{
	ifstream in;
	in.open(filename.c_str());

	if (!in.is_open())
		throw "Error Opening File";
	string line;
	// check the header
	getline(in, line);
	if (line.compare("#Life 1.05"))
		throw "Invalid Life 1.05 Header";
	// skip filler lines
	while(line.size() == 0 || line.at(0) == '#')
		getline(in, line);
	// cache the lines to avoid having to re-read
	string boardLines[20];
	int width = 0, height = 0;
	do
	{
		 boardLines[height++] = line;
		 if(width != 0 && (unsigned int)width != line.length())
			throw "Error: Non-rectangular matrix";
		 width = line.length();

	}while(getline(in, line));

	in.close();

	// create & apply the data set
	BoardData ret = {true, height, width, 0, 0, 0, vector<vector<bool>>(height, vector<bool> (width, 0))};
	for(int i = 0; i < width; i++)
		for(int j = 0; j < height; j++)
			if(boardLines[i].at(j) == '*')
				ret.matrix[j][i] = true;
	return ret;


}

BoardData loadRLE(string filename)
{
	ifstream in;
	in.open(filename.c_str());

	if (!in.is_open())
		throw "Error Opening File";
	string line;
	int width = 0, height = 0;


	while (getline(in, line))
		if (line.at(0) == '#')
			continue;
		else
			break;
	// handle first line
	sscanf(line.c_str(), "x = %d, y = %d%*s", &width, &height);
	BoardData ret = {true, height, width, 0, 0, 0, vector<vector<bool>>(height, vector<bool> (width, 0))};
	int x=0, y=0;
	while (getline(in, line))
	{
		if (line.at(0) == '#')
			continue;
		//cout << "LINE: " << line << endl;
		int count = 0;
		for (int i = 0; i < (int)line.length(); i++)
		{
			char c = line.at(i);
			// handle newlines
			if (c == '\n' || c == '\r')
				continue;
			// handle RLE number
			if ('0' <= c && c <= '9')
			{
				count = count * 10 + (c - '0');
				continue;
			}

			// handle board newline / quit chars
			if (c == '$')
			{
				if (count > 0)
					y += count;
				else
					y++;
				x = 0;
				count = 0;
				continue;
			}
			if (c == '!')
			{
				break;
			}

			// ok, actually print
			if (count)
			{
				for (int j = 0; j < count; j++)
				{
					if (c == 'o')
						ret.matrix[y][x] = true;
					x++;
				}
				count = 0;
			}
			else
			{
				if (c == 'o')
					ret.matrix[y][x] = true;
				x++;
			}
		}

	}
	in.close();

	return ret;
}

BoardData loadBRD(string filename)
{
	ifstream in;
	in.open(filename.c_str());

	if (!in.is_open())
	{
		cerr << "File not opened '" << filename << "'" << endl;
	}

	string line;

	int height = fs_atoi(in);
	int width = fs_atoi(in);
	bool wrapAround = fs_atoi(in);
	int iterations = fs_atoi(in);
	int births = fs_atoi(in);
	int deaths = fs_atoi(in);
	BoardData ret = {wrapAround, height, width, iterations,
		 			births, deaths, vector<vector<bool>>(height, vector<bool> (width, 0))};

	int row = 0;
	string a;
	while(getline(in, line))
	{
		for(int i = 0; i <width; i++)
		{
			a = line[i];
			ret.matrix[row][i] = atoi(a.c_str());
		}
		row++;
	}

	in.close();
	return ret;
}

BoardData loadFormat(string filename)
{
	if      (endsWith(filename, ".life") ||
			 endsWith(filename, ".lif"))
		return loadLife(filename);

	else if (endsWith(filename, ".rle"))
		return loadRLE(filename);

	else if (endsWith(filename, ".brd"))
		return loadBRD(filename);

	else
		throw "Unknown File Type";
}
/*
int main( int argc, char* args[] )
{
	Board *test = loadFormat("rlepack/copperhead.rle");
	test->printBoard();
	cout << endl;

	return 0;
}
*/
