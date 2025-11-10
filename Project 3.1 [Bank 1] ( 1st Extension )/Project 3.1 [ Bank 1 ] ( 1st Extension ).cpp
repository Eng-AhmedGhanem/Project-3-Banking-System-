#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;


const string FileName = "ClientsData.txt";


void ShowMainMenue();
void ShowTransactionMenueScreen();



struct stClient
{
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance;
	bool MarkForDelete = false;
};

enum enMainMenueOptions
{
	eShowList = 1, eAddNewClient = 2
	, eDeleteClient = 3, eUpdateClient = 4,
	eFindClient = 5, eShowTransactionMenue = 6, eExit = 7
};

enum enTransactionsMenueQustions
{
	eDeposit = 1, eWidthdraw = 2, eShowTotalBalance = 3, eShowMainMenue = 4
};

vector<string> SplitString(string S1, string Delim = "#//#")
{
	short Pos = 0;
	string sWord = "";
	vector<string>vLine;

	while ((Pos = S1.find(Delim)) != string::npos)
	{
		sWord = S1.substr(0, Pos);

		if (sWord != "")
		{
			vLine.push_back(sWord);
		}
		S1.erase(0, Pos + Delim.length());
	}

	if (S1 != "")
	{
		vLine.push_back(S1);
	}
	return vLine;

}

stClient ConvertLineToRecord(string Line, string Delim = "#//#")
{
	vector<string>vLine = SplitString(Line);
	stClient Client;

	Client.AccountNumber = vLine[0];
	Client.PinCode = vLine[1];
	Client.Name = vLine[2];
	Client.Phone = vLine[3];
	Client.AccountBalance = stod(vLine[4]);

	return Client;

}

string ConvertRecordToLine(stClient Client, string Delim = "#//#")
{
	string S2 = "";
	S2 += Client.AccountNumber + Delim;
	S2 += Client.PinCode + Delim;
	S2 += Client.Name + Delim;
	S2 += Client.Phone + Delim;
	S2 += to_string(Client.AccountBalance);

	return S2;

}

vector<stClient> LoadDataFromFile(string FileName)
{
	vector<stClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in); //Read Mode

	if (MyFile.is_open())
	{
		stClient Client;
		string Line = "";

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}

	return vClients;
}

void PrintClientCard(stClient Client)
{
	cout << "\nThe following are the client details:\n";
	cout << "-----------------------------------";
	cout << "\nAccout Number  : " << Client.AccountNumber;
	cout << "\nPin Code       : " << Client.PinCode;
	cout << "\nName           : " << Client.Name;
	cout << "\nPhone          : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n-----------------------------------\n";
}

void PrintClientInfo(stClient Client)
{
	cout << "|" << left << setw(15) << Client.AccountNumber;
	cout << "|" << left << setw(10) << Client.PinCode;
	cout << "|" << left << setw(35) << Client.Name;
	cout << "|" << left << setw(12) << Client.Phone;
	cout << "|" << left << setw(12) << Client.AccountBalance;
}

void ShowAllClientsList()
{
	vector<stClient>vClients = LoadDataFromFile(FileName);

	cout << "\t\t\tClient List (" << vClients.size() << ") CLient(s).";
	cout << "\n-----------------------------------------------------------------------------"
		"----------------------------------------\n";
	cout << "|" << left << setw(15) << "Account Number";
	cout << "|" << left << setw(10) << "Pin Code";
	cout << "|" << left << setw(35) << "Name";
	cout << "|" << left << setw(12) << "Phone";
	cout << "|" << left << setw(12) << "Account Balance";
	cout << "\n-----------------------------------------------------------------------------"
		"----------------------------------------\n";

	if (vClients.size() == 0)
	{
		cout << "\t\t\tNo Clients Available In The System!";
	}
	else
	{
		for (stClient& C : vClients)
		{
			PrintClientInfo(C);
			cout << endl;
		}
		cout << "\n-----------------------------------------------------------------------------"
			"----------------------------------------\n" << endl;
	}

}

string ReadAccountNumber()
{
	string AccountNumber = "";
	cout << "\nEnter Account Number?";
	cin >> AccountNumber;
	return AccountNumber;
}

short ReadMainMenueChoice()
{
	short Choice = 0;

	cout << "Choose Waht do you want to do [1 to 7]? ";
	cin >> Choice;
	return Choice;
}

void GoBackToMainMenueScreen()
{
	cout << "\nPress any key to go back to Main Menue...";
	system("pause>0");

	ShowMainMenue();

}

bool ClientExistByAccountNumber(string AccountNumber, string FileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		stClient Client;
		string Line = "";
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);

			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();

				return true;
			}

		}
		MyFile.close();
	}
	return false;
}

stClient ReadNewClient()
{
	stClient Client;

	cout << "\nEnter Account Number? ";
	getline(cin >> ws, Client.AccountNumber);

	while (ClientExistByAccountNumber(Client.AccountNumber, FileName))
	{
		cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";

		getline(cin >> ws, Client.AccountNumber);

	}

	cout << "\nEnter Pin Code? ";
	getline(cin, Client.PinCode);

	cout << "\nEnter Name? ";
	getline(cin, Client.Name);

	cout << "\nEnter Phone? ";
	getline(cin, Client.Phone);


	cout << "\nEnter Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;

}

void SaveClientDataToFile(stClient sClient, string FileName)
{

	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		string Line = "";
		Line = ConvertRecordToLine(sClient);

		MyFile << Line << endl;

		MyFile.close();
	}

}

void SaveClientsDataToFileWithFalseMark(vector<stClient>& vClient)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		string Line = "";

		for (stClient& C : vClient)
		{
			if (C.MarkForDelete == false)
			{
				Line = ConvertRecordToLine(C);
				MyFile << Line << endl;
			}
		}
	}
}

void AddNewClient()
{
	stClient Client;
	Client = ReadNewClient();
	SaveClientDataToFile(Client, FileName);
}

void AddNewClients()
{

	char Answer = 'n';

	do
	{
		cout << "\nAdding New Client:\n";

		AddNewClient();


		cout << "\nClient Added Successfully ,do you want to add more clients Y/N ?";
		cin >> Answer;

	} while (tolower(Answer) == 'y');


}

bool FindClientByAccountNumber(string AccountNumber, vector<stClient>vClient, stClient& Client)
{

	for (stClient& C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;

			return true;
		}

	}
	return false;
}

bool MarkClientForDelete(string AccountNumber, vector<stClient>& vClient)
{
	for (stClient& C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;

		}
	}
	return false;
}

void ShowAddClientsScreen()
{
	cout << "\n--------------------------------------------\n";
	cout << "\tAdd New CLient Screen\n";
	cout << "\n--------------------------------------------\n";

	AddNewClients();
}

void DeleteClientByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
	stClient Client;

	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\nAre you sure you want to delete this client y/n? ";
		cin >> Answer;

		if (tolower(Answer) == 'y')
		{
			MarkClientForDelete(AccountNumber, vClients);
			SaveClientsDataToFileWithFalseMark(vClients);

			//Refresh Vector
			vClients = LoadDataFromFile(FileName);

			cout << "\n\nCLient Deleted Successfully.";

		}

	}
	else
	{
		cout << "\nClient with Account Number [" << AccountNumber << "] is Not Found!\n";
	}

}

void ShowDeleteClientScreen()
{
	cout << "--------------------------------------------\n";
	cout << "\tDelete Client Screen\n";
	cout << "--------------------------------------------\n";

	string AccountNumber = ReadAccountNumber();
	vector<stClient>vClient = LoadDataFromFile(FileName);
	DeleteClientByAccountNumber(AccountNumber, vClient);

}

stClient ChangeClientRecord(string AccountNumber)
{
	stClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "\nEnter Pin Code? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name ? ";
	getline(cin, Client.Name);

	cout << "Enter Phone ? ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;


}

void UpdateClientByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
	stClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\nAre you sure do you want to update this client y/n? ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			for (stClient& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}

			SaveClientsDataToFileWithFalseMark(vClients);
			cout << "\nClient updated Successfully.\n";

		}
		else
		{
			cout << "\nUpdate Canceled.\n";

		}



	}
	else
	{
		cout << "\nClient with Account Number(" << AccountNumber << ") is Not Found!\n";
	}

}

void ShowUpdateClientScreen()
{
	cout << "--------------------------------------------\n";
	cout << "\tUpdate CLient Screen\n";
	cout << "--------------------------------------------\n";

	vector<stClient>vClients = LoadDataFromFile(FileName);
	string AccountNumber = ReadAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClients);

}

void ShowFindClientScreen()
{
	cout << "--------------------------------------------\n";
	cout << "\tShow Find CLient Screen\n";
	cout << "--------------------------------------------\n";

	stClient Client;
	vector<stClient>vClients = LoadDataFromFile(FileName);
	string AccountNumber = ReadAccountNumber();

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
	}
	else
	{
		cout << "\nClient with Account number [" << AccountNumber << "] is Not Found!\n";
	}


}

void ShowExitScreen()
{
	cout << "--------------------------------------------\n";
	cout << "\tProgram Ends :-)\n";
	cout << "--------------------------------------------\n";
	cout << "\tThanks for using out Bank system :-)\n";
	cout << "--------------------------------------------\n";
}

short ReadTransactionsMenueOption()
{
	short Choice = 0;
	cout << "Choose what do you want to do ? [1 to 4]? ";
	cin >> Choice;
	return Choice;

}

void PrintClientBalanceInfo(stClient Client)
{
	cout << "|" << left << setw(35) << Client.AccountNumber;
	cout << "|" << left << setw(40) << Client.Name;
	cout << "|" << left << setw(15) << Client.AccountBalance;
}

void ShowTotalBalance()
{
	double TotalBalance = 0;
	vector<stClient>vClients = LoadDataFromFile(FileName);

	cout << "\t\t\t\nClient List (" << vClients.size() << ") CLient(s).";
	cout << "\n-----------------------------------------------------------------------------"
		"----------------------------------------\n";
	cout << "|" << left << setw(35) << "Account Number";
	cout << "|" << left << setw(40) << "Name";
	cout << "|" << left << setw(15) << "Balance";
	cout << "\n-----------------------------------------------------------------------------"
		"----------------------------------------\n";

	if (vClients.size() == 0)
	{
		cout << "\t\t\tNo Clients Available In The System!";
	}
	else
	{
		for (stClient& C : vClients)
		{
			PrintClientBalanceInfo(C);
			TotalBalance += C.AccountBalance;
			cout << endl;
		}
		cout << "\n-----------------------------------------------------------------------------"
			"----------------------------------------\n" << endl;
		cout << "\t\t\t\t\t\tTotal Balance = " << TotalBalance << endl;
	}

}

void ShowTotalBalanceScreen()
{
	ShowTotalBalance();

}

void GoBackToTransactionMenue()
{
	cout << "\nPress any key to go to Transaction Screen...";
	system("pause>0");
	ShowTransactionMenueScreen();
}

void DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector<stClient>& vClients)
{
	char Answer = 'n';

	cout << "\nAre you sure do you want to perform this transaction y/n? ";
	cin >> Answer;

	if (tolower(Answer) == 'y')
	{

		for (stClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;

				SaveClientsDataToFileWithFalseMark(vClients);
				cout << "\nDone Successfully New Balance is : " << C.AccountBalance << endl;

				break;
			}
		}
	}
	else
	{
		cout << "\transaction cancled.!\n";
	}
}

void ShowDepositScreen()
{
	cout << "----------------------------------------------\n";
	cout << "\tDeposit Screen\n";
	cout << "----------------------------------------------\n";


	stClient Client;

	vector<stClient> vClients = LoadDataFromFile(FileName);
	string AccountNumber = ReadAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadAccountNumber();

	}

	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease enter deposit amount?";
	cin >> Amount;

	DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);

}

void ShowWithdrawScreen()
{

	cout << "----------------------------------------------\n";
	cout << "\tWithdraw Screen\n";
	cout << "----------------------------------------------\n";


	stClient Client;
	double Amount = 0;
	char Answer = 'n';

	vector<stClient> vClients = LoadDataFromFile(FileName);
	string AccountNumber = ReadAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadAccountNumber();

	}

	PrintClientCard(Client);

	cout << "\nPlease enter withdraw amount? ";
	cin >> Amount;

	while (Amount > Client.AccountBalance)
	{
		cout << "\nAmount Exceeds the balance, you can withdraw up to :" << Client.AccountBalance << endl;

		cout << "\nPlease enter another withdraw amount? ";
		cin >> Amount;
	}

	DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);
}

void PerformTransactionsMenueOption(enTransactionsMenueQustions Transaction)
{
	switch (Transaction)
	{
	case enTransactionsMenueQustions::eDeposit:
		system("cls");
		ShowDepositScreen();
		GoBackToTransactionMenue();
		break;

	case enTransactionsMenueQustions::eWidthdraw:
		system("cls");
		ShowWithdrawScreen();
		GoBackToTransactionMenue();
		break;

	case enTransactionsMenueQustions::eShowTotalBalance:
		system("cls");
		ShowTotalBalanceScreen();
		GoBackToTransactionMenue();
		break;

	case enTransactionsMenueQustions::eShowMainMenue:
		ShowMainMenue();;
		break;

	}

}

void ShowTransactionMenueScreen()
{
	system("cls");
	cout << "===============================================\n";
	cout << "\t\tTransaction Menue Screen\n";
	cout << "===============================================\n";
	cout << "\t[1]Deposit.\n";
	cout << "\t[2]Widthdraw.\n";
	cout << "\t[3]ShowTotalBalance.\n";
	cout << "\t[4]Main Menue.\n";
	cout << "==============================================\n";

	PerformTransactionsMenueOption(enTransactionsMenueQustions(ReadTransactionsMenueOption()));
}

void PerformMainMenueOption(enMainMenueOptions Option)
{
	switch (Option)
	{

	case enMainMenueOptions::eShowList:

		system("cls");
		ShowAllClientsList();
		GoBackToMainMenueScreen();
		break;

	case  enMainMenueOptions::eAddNewClient:
		system("cls");
		ShowAddClientsScreen();
		GoBackToMainMenueScreen();
		break;

	case  enMainMenueOptions::eDeleteClient:
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainMenueScreen();
		break;

	case  enMainMenueOptions::eUpdateClient:
		system("cls");
		ShowUpdateClientScreen();
		GoBackToMainMenueScreen();
		break;


	case  enMainMenueOptions::eFindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenueScreen();
		break;

	case enMainMenueOptions::eShowTransactionMenue:

		system("cls");
		ShowTransactionMenueScreen();
		break;


	case  enMainMenueOptions::eExit:
		system("cls");
		ShowExitScreen();
		break;
	}

}

void ShowMainMenue()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transaction.\n";
	cout << "\t[7] Exit.\n";
	cout << "===========================================\n";

	PerformMainMenueOption(enMainMenueOptions(ReadMainMenueChoice()));
}


int main()
{
	system("cls");

	ShowMainMenue();
	return 0;

}

