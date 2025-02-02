// AutoWinInstaller.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <conio.h>
#include <sstream>
#include <string>
#include <fstream>

using namespace std;

void setupDisk();
void WriteImg();
void optionSel();
void efiOpt(int disk);
void mbrOpt(int disk);
void backMenu();
void createFi(string file, string text);

int main()
{
    SetConsoleTitle(L"Automated Windows Installer");
    optionSel();
}

void optionSel() {
    int option;
    cout << "1 - Setup the disk\n";
    cout << "2 - Write the image file\n";
    cout << "3 - Quit the application\n";
    cout << "Enter the number: ";
    cin >> option;
    if (option == 1) {
        setupDisk();
    }
    if (option == 2) {
        WriteImg();
    }
    if (option == 3) {
        exit(-1);
    }
}

void WriteImg() {
    std::string imgFile;
    std::cout << "Please input the path of your WIM file!" << std::endl;
    std::cin >> imgFile;
    std::cout << "Is this correct? [Y/N]" << std::endl;
    string option;
    cin >> option;
    if (option == "y") {
        std::cout << "Once your ready to write the image, press enter to proceed with this operation!" << std::endl;
        system("pause");
        std::string command = "dism /apply-image /imagefile:" + imgFile + " /index:1 /applydir:R:";
        system(command.c_str());
        cout << "Did you use MBR or GPT for disk partition scheme? [m/g]";
        string gptan;
        cin >> gptan;
        if (gptan == "g") {
            string command = "bcdboot R:""\\Windows"" /s W:";
            system(command.c_str());
        }
        if (gptan == "m") {
            string command = "bcdboot R:""\\Windows"";
            system(command.c_str());
        }
        std::cout << "Image write was successful! Press the enter key to go back to the main menu!" << std::endl;
        backMenu();
    }
    if (option == "n") {
        cout << "Please retry this operation to change your image file!";
        backMenu();
    }
}

void mbrOpt(int disk) {
    createFi("pardisk.txt", "sel disk " + std::to_string(disk) + "\nclean\nconvert mbr\ncreate partition primary\nformat quick fs=ntfs\nassign letter r\nactive");
    system("diskpart.exe /s pardisk.txt");

    // Completed with the disk setup
    std::cout << "Completed with the disk setup!" << std::endl;
    std::cout << "If you need to know what the letters are, see below. Once you're ready, press enter to continue!" << std::endl;
    system("del pardisk.txt");
    system("echo Windows partition: R");
}

void efiOpt(int disk) {
    createFi("pardisk.txt", "sel disk " + std::to_string(disk) + "\nclean\nconvert gpt\ncreate partition efi size=300\nformat quick fs=fat32\nassign letter w\ncreate partition primary\nformat quick fs=ntfs\nassign letter r");
    system("diskpart.exe /s pardisk.txt");

    // Completed with the disk setup
    std::cout << "Completed with the disk setup!" << std::endl;
    std::cout << "If you need to know what the letters are, see below. Once you're ready, press enter to continue!" << std::endl;
    system("del pardisk.txt");
    system("echo Windows partition: R");
    system("echo EFI partition: W");
}

void backMenu() {
    system("pause");
    system("cls");
    main();
}

void createFi(string file, string text) {
    ofstream fileSel(file);
    fileSel << text;
    fileSel.close();
}

void setupDisk() {
    int x;
    string option;
    system("echo list disk > disklist.txt");
    system("diskpart.exe /s disklist.txt");
    std::cout << "Which disk would you like to use?" << std::endl;
    std::cin >> x;
    std::cout << "You have selected the disk number: " << x << std::endl;
    std::cout << "Is this correct? (Y/N)" << std::endl;
    createFi("seldisk.txt", "sel disk " + std::to_string(x));
    system("del disklist.txt");
    cin >> option;
    if (option == "y") {
        cout << "Would you like to use MBR or GPT for your disk? (M/G)";
        string format;
        cin >> format;
        if (format == "m") {
            cout << "Are you sure you want to proceed? This will wipe all data on the selected drive! [Y/N]";
			string ans;
            cin >> ans;
            if (ans == "y") {
				mbrOpt(x);
				backMenu();
            }
			if (ans == "n") {
				cout << "Operation has been cannceled! Returning to the main menu!";
				system("pause");
				main();
			}
        }
        if (format == "g") {
            cout << "Are you sure you want to proceed? This will wipe all data on the selected drive! [Y/N]";
            string ans;
            cin >> ans;
            if (ans == "y") {
                efiOpt(x);
                backMenu();
            }
            if (ans == "n") {
                cout << "Operation has been cannceled! Returning to the main menu!";
                system("pause");
                main();
            }
        }
    }
    if (option == "n") {
        cout << "Please retry this operation to change your disk number!";
        backMenu();
    }
}

//ofstream seldisk("seldisk.txt");
//seldisk << "sel disk ", x;
//seldisk.close();
//std::cout << "Would you like to use MBR or GPT for your disk? (M/G)" << std::endl;