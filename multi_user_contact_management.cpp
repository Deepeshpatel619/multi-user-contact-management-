#include <iostream>
#include <fstream>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

using namespace std;

//Contact class
class Contact
{
  public:
    string first_name;
    string last_name;
    string mobile_number;

    bool isvalidNumber()
    {
        if (mobile_number.size() != 10)
        {
            cout << "Number should have 10 digits!" << endl;
            return false;
        }
        for (auto &ch : mobile_number)
        {
            if (!isdigit(ch))
            {
                cout << "should be numerical!" << endl;
                return false;
            }
        }
        return true;
    }
};

//User class
class User
{
    string user_file_name;

  public:
    string user_name;
    User() : user_file_name("") {}
    void set_user_file(string file)
    {
        user_file_name = file;
    }
    string get_user_file()
    {
        return user_file_name;
    }
    void show_user_name()
    {
        user_name[0] = toupper(user_name[0]);
        system("color 6");
        cout << "\n@@@ " << user_name << " @@@" << endl;
    }
    void addContact();
    void removeContact();
    int showContact();
};

// key=hash_key value=user_file where its contacts are saved
class Book
{
  public:
    string get_file_name(string hash)
    {
        string key = "", file_name = "";
        ifstream fin("map_user_file.txt");
        while (!fin.eof())
        {
            fin >> key;
            fin >> file_name;
            if (key == hash)
                break;
        }
        fin.close();
        return file_name;
    };
    string createFile()
    {
        ifstream fin("map_user_file.txt");
        string key = "";
        string file_name = "user0";
        while (!fin.eof())
        {
            fin >> key;
            fin >> file_name;
        }
        string ans = "user" + to_string(stoi(file_name.substr(4)) + 1);
        fin.close();
        return ans;
    };
    void set_file_name(string hash)
    {
        ofstream fout("map_user_file.txt", ios::app);
        fout << hash << " " << (createFile() + ".txt") << endl;
        fout.close();
    };
};
//login class
class login : private Book
{
    string username;
    string password;

  public:
    User getUser();
    void createUser();
};

User login::getUser()
{
    User newUser;
    cout << "Enter Username:" << endl;
    cin >> username;
    cout << "Enter Password:" << endl;
    cin >> password;

    bool isFound = false;
    string name, pass;
    ifstream fin("users.txt");
    while (!fin.eof())
    {
        fin >> name;
        fin >> pass;
        if ((name == username) && (pass == password))
        {
            isFound = true;
            break;
        }
    }
    fin.close();
    if (isFound)
    {
        string key = username + '@' + password;
        newUser.user_name = username;
        newUser.set_user_file(get_file_name(key));
    }
    else
        cout << "User Not Found!" << endl;
    return newUser;
}
void login::createUser()
{
    cout << "Enter username:" << endl;
    cin >> username;
    cout << "Enter password:" << endl;
    cin >> password;

    //writing to file
    ofstream fout;
    fout.open("users.txt", ios::app);
    fout << username << " " << password << endl;
    fout.close();

    string hash = username + '@' + password;
    set_file_name(hash);
};

//Adding contacts
void User::addContact()
{
    Contact temp;
    cout << "Enter First Name: " << endl;
    cin >> (temp.first_name);
    cout << "Enter Last Name: " << endl;
    cin >> (temp.last_name);
    do
    {
        cout << "Enter Mobile Number:" << endl;
        cin >> (temp.mobile_number);
    } while (!temp.isvalidNumber());
    //writing to file
    ofstream fout(get_user_file(), ios::app);
    if (!fout)
    {
        cout << "File can't open ! " << endl;
    }
    else
    {
        fout.write((char *)&temp, sizeof(temp));
        cout << "Successfully Added !" << endl;
    }
    fout.close();
}
//show contacts
int User::showContact()
{
    clrscr();
    Contact temp;
    int id = 1;
    ifstream fin(get_user_file());
    //  fin.read((char*)&temp,sizeof(temp));
    if (!fin)
    {
        cout << "No Contact To Show !" << endl;
        fin.close();
        return 0;
    }
    system("color a");
    cout << "Id"
         << " "
         << "FirstName"
         << " "
         << "LastName"
         << " "
         << "Mobile Number " << endl;
    system("color b");
    fin.read((char *)&temp, sizeof(temp));
    while (!fin.eof())
    {
        cout << (id++) << " " << (temp.first_name) << "  " << (temp.last_name) << "  " << (temp.mobile_number) << endl;
        fin.read((char *)&temp, sizeof(temp));
    }

    fin.close();
    system("color 7");
    return id;
}
void User::removeContact()
{
    int list_size = showContact();

    string id_str;
    do
    {
        cout << "Enter I'd of Contact to be removed:" << endl;
        cin >> id_str;
        if (id_str.size() > 10)
        {
            cout << "Too long I'd !" << endl;
            continue;
        }
        bool isnumber = true;
        for (auto &ch : id_str)
        {
            if (!isdigit(ch))
            {
                cout << "Id should be numeric!" << endl;
                isnumber = false;
                break;
            }
        }
        if (isnumber && stoi(id_str) <= list_size)
        {
            char consent;
            cout << "Do you really want to remove contact with id=" << id_str << " [y/n] " << endl;
            cin >> consent;
            if (consent == 'y')
            {
                Contact temp;
                int id = 1;
                ifstream fin(get_user_file());
                fin.read((char *)&temp, sizeof(temp));
                ofstream fout("temp.txt");
                while (!fin.eof())
                {
                    if (id != stoi(id_str))
                    {
                        fout.write((char *)&temp, sizeof(temp));
                    }
                    id++;
                    fin.read((char *)&temp, sizeof(temp));
                }
                string filename = get_user_file();
                remove(filename.c_str());
                rename("temp.txt", filename.c_str());
                cout << "Successfully Removed !" << endl;
                fout.close();
                fin.close();
            }
        }

        break;
    } while (true);
}
void userChoices(User *usr)
{
    usr->show_user_name();
    string choice;
    do
    {
        cout << "\n********************************\n\n\n";
        cout << "          1.Add Contact:" << endl;
        cout << "          2.Remove Contact:" << endl;
        cout << "          3.Show All Contacts:" << endl;
        cout << "          4.Logout:" << endl
             << endl;
        cout << "\n\n\n********************************\n";
        cout << "Enter your Choice:" << endl;
        cin >> choice;

        switch (choice[0])
        {
        case '1':
            usr->addContact();
            break;
        case '2':
            usr->removeContact();
            break;
        case '3':
            usr->showContact();
            break;
        case '4':
            cout << "Successfully logout !" << endl;
            break;
        default:
            cout << "Wrong Choice!" << endl;
            break;
        };

    } while (choice[0] != '4');
}
User login_func()
{
    login *root = new login();
    User usr1;
    string choice;
    do
    {
        cout << "\n*********🤗 WELCOME 🤗*************\n\n";
        cout << "           1.Create New User:" << endl;
        cout << "           2.Login:" << endl;
        cout << "\n\n********************************\n";
        cout << "Enter choice:" << endl;
        cin >> choice;
        switch (choice[0])
        {
        case '1':
            root->createUser();
            clrscr();
            cout << "Login Please!" << endl;
        case '2':
            usr1 = root->getUser();
            choice[0] = '3';
            break;
        default:
            cout << "Wrong Choice" << endl;
            break;
        }

    } while (choice[0] != '3');

    return usr1;
}

//Main function!
int main(int argc, char *argv[])
{
    User usr1;
    while (usr1.get_user_file() == "")
    {
        usr1 = login_func();
    }
    clrscr();
    userChoices(&usr1);
    return 0;
}