#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;


const string FileClientsName = "ClientsData.txt";
const string FileUsersName = "Users.txt";

void ShowMainMenue();
void ShowTransactionMenueScreen();
void Login();
void ShowManageUserScreen();
void ShowAddUserScreen();
void AddNewUser();
void ShowAccessDeniedScreen();



struct stUsers
{
	string UserName;
	string Password;
	short Permission = 0;
	bool MarkForDelete = false;
};

stUsers CurrentUser;


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
	eFindClient = 5, eShowTransactionMenue = 6, eManageUsers = 7, eLogout = 8
};

enum enTransactionsMenueQustions
{
	eDeposit = 1, eWidthdraw = 2, eShowTotalBalance = 3, eShowMainMenue = 4
};
enum enManageUsersOptions
{
	eListUsers = 1, eAddUser = 2, eDeleteUser = 3, eUpdateUser = 4, eFindUser = 5, eMainMenue = 6
};

enum enPermission
{
	ShowClientsList = 1, enAddNewClient = 2,
	DeleteClient = 4, UpdateClient = 8,
	FindClient = 16, Transaction = 32,
	ManageUsers = 64
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

stUsers ConvertLineUserToRecord(string Line, string Delim = "#//#")
{
	vector<string>vLine = SplitString(Line);
	stUsers User;

	User.UserName = vLine[0];
	User.Password = vLine[1];
	User.Permission = stoi(vLine[2]);


	return User;

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

string ConvertRecordToLine(stUsers User, string Delim = "#//#")
{
	string S2 = "";
	S2 += User.UserName + Delim;
	S2 += User.Password + Delim;
	S2 += to_string(User.Permission);

	return S2;

}

vector<stClient> LoadDataFromFile(string FileClientsName)
{
	vector<stClient> vClients;
	fstream MyFile;
	MyFile.open(FileClientsName, ios::in); //Read Mode

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

vector<stUsers> LoadDataFromFileUser(string FileUsersName)
{
	vector<stUsers> vUsers;
	fstream MyFile;
	MyFile.open(FileUsersName, ios::in); //Read Mode

	if (MyFile.is_open())
	{
		stUsers User;
		string Line = "";

		while (getline(MyFile, Line))
		{
			User = ConvertLineUserToRecord(Line);
			vUsers.push_back(User);
		}
		MyFile.close();
	}

	return vUsers;
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
	if (CurrentUser.Permission & enPermission::ShowClientsList)
	{
		vector<stClient>vClients = LoadDataFromFile(FileClientsName);

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
	else
	{
		system("cls");
		ShowAccessDeniedScreen();
	}


}

string ReadAccountNumber()
{
	string AccountNumber = "";
	cout << "\nEnter Account Number?";
	cin >> AccountNumber;
	return AccountNumber;
}

short ReadOptionNumber(short Number)
{
	short Choice = 0;

	cout << "Choose Waht do you want to do [1 to " << Number << "] ? ";
	cin >> Choice;
	return Choice;
}

void GoBackToMainMenueScreen()
{
	cout << "\nPress any key to go back to Main Menue...";
	system("pause>0");

	ShowMainMenue();

}

bool ClientExistByAccountNumber(string AccountNumber, string FileClientsName)
{
	fstream MyFile;
	MyFile.open(FileClientsName, ios::in);

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

	while (ClientExistByAccountNumber(Client.AccountNumber, FileClientsName))
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

void SaveClientDataToFile(stClient sClient, string FileClientsName)
{

	fstream MyFile;
	MyFile.open(FileClientsName, ios::out | ios::app);

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
	MyFile.open(FileClientsName, ios::out);

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
	SaveClientDataToFile(Client, FileClientsName);
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
	if (CurrentUser.Permission & enPermission::enAddNewClient)
	{
		cout << "\n--------------------------------------------\n";
		cout << "\tAdd New CLient Screen\n";
		cout << "\n--------------------------------------------\n";

		AddNewClients();
	}
	else
	{
		system("cls");
		ShowAccessDeniedScreen();
	}

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
			vClients = LoadDataFromFile(FileClientsName);

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
	if (CurrentUser.Permission & enPermission::DeleteClient)
	{
		cout << "--------------------------------------------\n";
		cout << "\tDelete Client Screen\n";
		cout << "--------------------------------------------\n";

		string AccountNumber = ReadAccountNumber();
		vector<stClient>vClient = LoadDataFromFile(FileClientsName);
		DeleteClientByAccountNumber(AccountNumber, vClient);
	}
	else
	{
		system("cls");
		ShowAccessDeniedScreen();
	}


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
	if (CurrentUser.Permission & enPermission::UpdateClient)
	{
		cout << "--------------------------------------------\n";
		cout << "\tUpdate CLient Screen\n";
		cout << "--------------------------------------------\n";

		vector<stClient>vClients = LoadDataFromFile(FileClientsName);
		string AccountNumber = ReadAccountNumber();
		UpdateClientByAccountNumber(AccountNumber, vClients);
	}
	else
	{
		system("cls");
		ShowAccessDeniedScreen();
	}


}

void ShowFindClientScreen()
{
	if (CurrentUser.Permission & enPermission::FindClient)
	{
		cout << "--------------------------------------------\n";
		cout << "\tShow Find CLient Screen\n";
		cout << "--------------------------------------------\n";

		stClient Client;
		vector<stClient>vClients = LoadDataFromFile(FileClientsName);
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
	else
	{
		system("cls");
		ShowAccessDeniedScreen();
	}



}

//void ShowExitScreen()
//{
//	cout << "--------------------------------------------\n";
//	cout << "\tProgram Ends :-)\n";
//	cout << "--------------------------------------------\n";
//	cout << "\tThanks for using out Bank system :-)\n";
//	cout << "--------------------------------------------\n";
//}

void PrintClientBalanceInfo(stClient Client)
{
	cout << "|" << left << setw(35) << Client.AccountNumber;
	cout << "|" << left << setw(40) << Client.Name;
	cout << "|" << left << setw(15) << Client.AccountBalance;
}

void ShowTotalBalance()
{
	double TotalBalance = 0;
	vector<stClient>vClients = LoadDataFromFile(FileClientsName);

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
		cout << "\tTransaction cancled.!\n";
	}
}

void ShowDepositScreen()
{
	cout << "----------------------------------------------\n";
	cout << "\tDeposit Screen\n";
	cout << "----------------------------------------------\n";


	stClient Client;

	vector<stClient> vClients = LoadDataFromFile(FileClientsName);
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

	vector<stClient> vClients = LoadDataFromFile(FileClientsName);
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
	if (CurrentUser.Permission & enPermission::Transaction)
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

		PerformTransactionsMenueOption(enTransactionsMenueQustions(ReadOptionNumber(4)));
	}
	else
	{
		system("cls");
		ShowAccessDeniedScreen();
	}

}

void GoBackToManageUsersMenueScreen()
{
	cout << "\nPress any key to go back to Manage Users Menue Screen...";
	system("pause>0");

	ShowManageUserScreen();

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
		GoBackToMainMenueScreen();
		break;

	case enMainMenueOptions::eManageUsers:

		system("cls");
		ShowManageUserScreen();
		GoBackToMainMenueScreen();
		break;


	case  enMainMenueOptions::eLogout:
		system("cls");
		Login();
		break;
	}

}

void PrintUsersInfo(stUsers Users)
{
	cout << "|" << left << setw(20) << Users.UserName;
	cout << "|" << left << setw(15) << Users.Password;
	cout << "|" << left << setw(15) << Users.Permission;
}

void ShowListUsers()
{
	vector<stUsers>vUsers = LoadDataFromFileUser(FileUsersName);

	cout << "\t\t\tUsers List (" << vUsers.size() << ") User(s).";
	cout << "\n-----------------------------------------------------------------------------"
		"----------------------------------------\n";
	cout << "|" << left << setw(20) << "UserName ";
	cout << "|" << left << setw(15) << "Password";
	cout << "|" << left << setw(15) << "Permission";
	cout << "\n-----------------------------------------------------------------------------"
		"----------------------------------------\n";
	{
		for (stUsers& U : vUsers)
		{
			PrintUsersInfo(U);
			cout << endl;
		}
		cout << "\n-----------------------------------------------------------------------------"
			"----------------------------------------\n" << endl;
	}
}

bool UserExcistByUserName(string UserName, string FileName)
{
	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		stUsers User;

		string Line = "";

		while (getline(MyFile, Line))
		{
			User = ConvertLineUserToRecord(Line);

			if (User.UserName == UserName)
			{
				MyFile.close();
				return true;
			}

		}

		MyFile.close();

	}
	return false;
}

string ConvertRecordToLineUser(stUsers User, string Dlim = "#//#")
{
	string S2 = "";

	S2 += User.UserName + Dlim;
	S2 += User.Password + Dlim;
	S2 += to_string(User.Permission);
	return S2;

}

void SaveUserDateToFile(stUsers User, string FileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		string Line = "";

		Line = ConvertRecordToLineUser(User);

		MyFile << Line << endl;
		MyFile.close();
	}

}

short ReadPermissionsToSet()
{
	int Permissions = 0;
	char Access = 'y';

	cout << "\nDo you want to give full access y/n? ";
	cin >> Access;

	if (tolower(Access) == 'y')
	{
		Permissions = -1;
	}
	else
	{
		cout << "\nDo you want to give access to:\n\n";

		cout << "Show Client List y/n? ";
		cin >> Access;

		if (tolower(Access) == 'y' )
		{
			Permissions = Permissions | enPermission::ShowClientsList;
		}
		cout << "\nAdd New Client y/n? ";
		cin >> Access;

		if (tolower(Access) == 'y')
		{
			Permissions = Permissions | enPermission::enAddNewClient;
		}
		cout << "\nDelete Client y/n? ";
		cin >> Access;

		if (tolower(Access) == 'y')
		{
			Permissions = Permissions | enPermission::DeleteClient;
		}
		cout << "\nUpdate Client y/n? ";
		cin >> Access;

		if (tolower(Access) == 'y')
		{
			Permissions = Permissions | enPermission::UpdateClient;
		}
		cout << "\nFind Client y/n? ";
		cin >> Access;

		if (tolower(Access) == 'y')
		{
			Permissions = Permissions | enPermission::FindClient;
		}
		cout << "\nTransactions y/n? ";
		cin >> Access;

		if (tolower(Access) == 'y')
		{
			Permissions = Permissions | enPermission::Transaction;
		}

		cout << "\nManage Users y/n? ";
		cin >> Access;

		if (tolower(Access) == 'y')
		{
			Permissions = Permissions | enPermission::ManageUsers;

		}
	}

	return Permissions;
}

stUsers ReadNewUser()
{
	short Access = 'n';
	stUsers User;


	cout << "Enter Username? ";
	getline(cin >> ws, User.UserName);

	while (UserExcistByUserName(User.UserName, FileUsersName))
	{
		cout << "\nUser with [" << User.UserName << "] already exists, Enter another Username? ";
		getline(cin >> ws, User.UserName);
	}

	cout << "Enter Password? ";
	getline(cin >> ws, User.Password);

	User.Permission = ReadPermissionsToSet();


	return User;
}

void AddNewUser()
{
	stUsers User;
	User = ReadNewUser();

	SaveUserDateToFile(User, FileUsersName);
}

void ReadNewUsers()
{
	char Answer = 'y';

	do
	{
		cout << "Adding New User:\n\n";

		AddNewUser();


		cout << "\nUser Added Successfully ,do you want to add more clients Y/N ?";
		cin >> Answer;

	} while (tolower(Answer) == 'y');

}

void ShowAccessDeniedScreen()
{
	cout << "--------------------------------------\n";
	cout << "Access Denied\n";
	cout << "You Dont have Permission To Do This,\n",
		cout << "Please Contact Your Admin\n";
	cout << "--------------------------------------\n";
}

void ShowAddUserScreen()
{

	cout << "--------------------------------------------\n";
	cout << "\tAdd New User Screen\n";
	cout << "--------------------------------------------\n";

	ReadNewUsers();

}

void PrintUserCard(stUsers User)
{
	cout << "\nThe following are the User details:\n";
	cout << "-----------------------------------";
	cout << "\nUserName        : " << User.UserName;
	cout << "\nPassword        : " << User.Password;
	cout << "\nPermission      : " << User.Permission;
	cout << "\n-----------------------------------\n";
}

bool FindUserByUserName(string UserName, vector<stUsers>vUsers, stUsers& User)
{

	for (stUsers& U : vUsers)
	{
		if (U.UserName == UserName)
		{
			User = U;
			return true;
		}
	}

	return false;
}

bool MarkUserForDeleteByUserName(string UserName, vector<stUsers>& vUsers)
{
	for (stUsers& U : vUsers)
	{
		if (U.UserName == UserName)
		{
			U.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

void SaveDataToFileWithUserFalseMark(vector<stUsers>& vUsers)
{
	fstream MyFile;


	MyFile.open(FileUsersName, ios::out);

	if (MyFile.is_open())
	{
		string Line;

		for (stUsers& U : vUsers)
		{
			if (U.MarkForDelete == false)
			{
				Line = ConvertRecordToLineUser(U);
				MyFile << Line << endl;
			}

		}
		MyFile.close();
	}

}

void DeleteUserByUserName(string UserName, vector<stUsers>& vUsers)
{
	stUsers User;
	char Answer = 'y';

	if (FindUserByUserName(UserName, vUsers, User))
	{
		if (UserName == "Admin")
		{
			cout << "\n\nYou Can't Delete This User.\n";
		}
		else
		{
			PrintUserCard(User);

			cout << "\n\nAre you sure do you want to delete this User y/n? ";
			cin >> Answer;

			if (tolower(Answer) == 'y')
			{
				MarkUserForDeleteByUserName(UserName, vUsers);
				SaveDataToFileWithUserFalseMark(vUsers);

				vUsers = LoadDataFromFileUser(FileUsersName);
				cout << "\n\nUser Deleted Successfully.\n";

			}
		}

	}
	else
	{
		cout << "\nUser with Username [" << UserName << "] is Not Found!\n";

	}
}

string ReadUserName()
{
	string UserName;
	cout << "Please enter Username? ";
	getline(cin >> ws, UserName);
	return UserName;
}

void ShowDeleteUserScreen()
{
	cout << "--------------------------------------------\n";
	cout << "\tDelete Users Screen\n";
	cout << "--------------------------------------------\n";

	string Username = ReadUserName();
	vector<stUsers>vUsers = LoadDataFromFileUser(FileUsersName);
	DeleteUserByUserName(Username, vUsers);
}

stUsers ChangeUserRecord(string Username)
{
	stUsers User;

	User.UserName = Username;

	cout << "\n\nEnter Password? ";
	getline(cin >> ws, User.Password);

	User.Permission = ReadPermissionsToSet();

	return User;
}

void UpdateNewUsers(string UserName, vector<stUsers>& vUsers)
{
	stUsers User;
	char Answer = 'y';

	if (FindUserByUserName(UserName, vUsers, User))
	{
		PrintUserCard(User);

		cout << "\n\nAre you sure do you want to Update this user y/n? ";
		cin >> Answer;

		if (tolower(Answer) == 'y')
		{
			for (stUsers& U : vUsers)
			{
				if (U.UserName == UserName)
				{
					U = ChangeUserRecord(UserName);
					break;

				}

			}
			SaveDataToFileWithUserFalseMark(vUsers);
			cout << "\nUser Updated Successfully.\n";

		}
		else
		{
			cout << "\nUpdate Canceled.\n";

		}
	}
	else
	{
		cout << "\nUser with Username [" << UserName << "] is Not Found!\n";

	}



}

void ShowUpdateUserScreen()
{

	cout << "--------------------------------------------\n";
	cout << "\tUpdate Users Screen\n";
	cout << "--------------------------------------------\n";

	string Username = ReadUserName();
	vector<stUsers>vUsers = LoadDataFromFileUser(FileUsersName);
	UpdateNewUsers(Username, vUsers);


}

void ShowFindUserScreen()
{

	cout << "--------------------------------------------\n";
	cout << "\tFind Users Screen\n";
	cout << "--------------------------------------------\n";

	string Username = ReadUserName();
	vector<stUsers>vUsers = LoadDataFromFileUser(FileUsersName);
	stUsers User;

	if (FindUserByUserName(Username, vUsers, User))
	{
		PrintUserCard(User);
	}
	else
	{
		cout << "\nUser with Username [" << Username << "] is Not Found!\n";

	}


}

void PerformManageUsersOption(enManageUsersOptions Option)
{
	switch (Option)
	{
	case enManageUsersOptions::eListUsers:
		system("cls");
		ShowListUsers();
		GoBackToManageUsersMenueScreen();
		break;

	case enManageUsersOptions::eAddUser:
		system("cls");
		ShowAddUserScreen();
		GoBackToManageUsersMenueScreen();
		break;

	case enManageUsersOptions::eDeleteUser:
		system("cls");
		ShowDeleteUserScreen();
		GoBackToManageUsersMenueScreen();
		break;

	case enManageUsersOptions::eUpdateUser:
		system("cls");
		ShowUpdateUserScreen();
		GoBackToManageUsersMenueScreen();
		break;

	case enManageUsersOptions::eFindUser:
		system("cls");
		ShowFindUserScreen();
		GoBackToManageUsersMenueScreen();
		break;

	case enManageUsersOptions::eMainMenue:
		system("cls");
		ShowMainMenue();
		break;


	}

}

void ShowManageUserScreen()
{
	if (CurrentUser.Permission & enPermission::ManageUsers)
	{
		system("cls");
		cout << "===========================================\n";
		cout << "\t\tManage Users Menue Screen\n";
		cout << "===========================================\n";
		cout << "\t[1] List Users.\n";
		cout << "\t[2] Add New User.\n";
		cout << "\t[3] Delete User.\n";
		cout << "\t[4] Update User.\n";
		cout << "\t[5] Find User.\n";
		cout << "\t[6] Main Menue.\n";
		cout << "===========================================\n";

		PerformManageUsersOption(enManageUsersOptions(ReadOptionNumber(6)));
	}
	else
	{
		system("cls");
		ShowAccessDeniedScreen();
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
	cout << "\t[7] Manage Users.\n";
	cout << "\t[8] Logout.\n";
	cout << "===========================================\n";

	PerformMainMenueOption(enMainMenueOptions(ReadOptionNumber(8)));
}

void ShowLoginScreen()
{
	cout << "-------------------------\n";
	cout << "\tLogin Screen\n";
	cout << "-------------------------\n";
}

void Login()
{
	//ShowLoginScreen();

	vector<stUsers>vUsers = LoadDataFromFileUser(FileUsersName);

	string UserName , Password;
	
	bool IsLoginSuccsess = false;

	ShowLoginScreen();

	do
	{

		cout << "Enter UserName? ";
		getline(cin >> ws, UserName);

		cout << "Enter Password? ";
		getline(cin >> ws, Password);


		for (stUsers& U : vUsers)
		{
			if (UserName == U.UserName && Password == U.Password)
			{
				IsLoginSuccsess = true;
				CurrentUser = U;
				break;
			}
		}

		if (!IsLoginSuccsess)
		{
			system("cls");
			ShowLoginScreen();
			cout << "Invalid UserName or Password!\n";

		}

	} while (IsLoginSuccsess == false);


	system("cls");
	ShowMainMenue();

}


int main()
{
	Login();

	return 0;

}

