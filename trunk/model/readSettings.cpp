#include "readSettings.h"

extern settingsT settings;

void readSettings()
{
	ifstream f;
	string FileName = "data/settings_glob.ini";
	f.open(FileName.c_str(), ios::in);
	if (!f.is_open())
	{
		cout << "Cannot read " << FileName << endl;
		system("pause");
		exit(0);
	}
	string line;
	int lineNum=0;
	while (!f.eof())
	{
		getline(f,line);
		if(line[0] != '#' && line.size()>0)
		{
			stringstream ss(line);
			switch (lineNum)
			{
				case 0: 
					settings.coeffPath = line;
					break;
				case 1:
					settings.inputPath = line;
					break;
				case 2:
					settings.outputPath = line;
					break;
				case 3: 
				{
					int num;
					ss >> num;
					cout<<"numMaps=\t"<<num<<endl;                
					for (int i = 0; i < num; i++)
					{
						getline(f,line);
						if(line[0] != '#' && line.size()>0)
						{
							transform(line.begin(), line.end(), line.begin(), ::tolower);
							settings.parametersMap.insert(line);
						}
						else
						{
							i--;
						}
					}
				}
				break; 
				case 4:
				{
					int num;
					ss >> num;
					cout<<"numTabs=\t"<<num<<endl;
					for (int i = 0; i < num; i++)
					{
						getline(f,line);
						if(line[0] != '#' && line.size()>0)
						{
							transform(line.begin(), line.end(), line.begin(), ::tolower);
							settings.parametersTable.insert(line);
						}
						else
						{
							i--;
						}
					}
				}                
				break;
				case 5:
				{
					int num;
					ss >> num;
					cout<<"numTabsReg=\t"<<num<<endl;
					for (int i = 0; i < num; i++)
					{
						getline(f,line);
						if(line[0] != '#' && line.size()>0)
						{
							transform(line.begin(), line.end(), line.begin(), ::tolower);
							settings.parametersTableReg.insert(line);
						}
						else
						{
							i--;
						}
					}
				}                
				break;                
				case 6:
					settings.produceTabs = (line == "1");
					break;
				case 7:
					settings.tabs[0] = (line == "1");
					break;
				case 8: settings.tabs[1] = (line == "1");
					break;
				case 9: settings.tabs[2] = (line == "1");
					break;
				case 10: settings.produceMaps = (line == "1");
					break;
				case 11: settings.maps[0] = (line == "1");
					break;
				case 12: settings.maps[1] = (line == "1");
					break;
				case 13: settings.maps[2] = (line == "1");
					break;
			}
			lineNum++;
		}
	}
	f.close();
}
