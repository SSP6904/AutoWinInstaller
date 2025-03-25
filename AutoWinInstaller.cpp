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
void commandRun(string cmd);
void backMenu();
void createFi(string file, string text);

int main()
{
    SetConsoleTitle(L"Automated Windows Installer");
    optionSel();
}

void commandRun(string cmd) {
    system(cmd.c_str());
}

void optionSel() {
    int option;
    cout << "1 - Setup the disk\n";
    cout << "2 - Write the image file\n";
    cout << "3 - Quit the application\n";
    cout << "Enter the number: ";
    cin >> option;
    if (option == 1) {
		system("cls");
        setupDisk();
    }
    if (option == 2) {
        system("cls");
        WriteImg();
    }
    if (option == 3) {
        exit(-1);
	}
    else {
        cout << "Invalid option! Please try again!";
        system("pause>nul");
        system("cls");
        optionSel();
    }
}

void WriteImg() {
    string imgFile;
    cout << "Enter the WIM file path. Should be either be named 'install.esd' or 'install.wim'. \n";
    cin >> imgFile;
    cout << "Is this correct? [Y/N] ";
    string option;
    cin >> option;
    if (option == "y") {
        string efiPar;
        string winPar;
        int indexImg;
        cout << "Input the EFI partition letter: ";
        cin >> efiPar;
        cout << "Input the Windows partition letter: ";
        cin >> winPar;
        commandRun("dism /Get-WimInfo /WimFile:" + imgFile + "");
        cout << "Input your index number from above: ";
        cin >> indexImg;
        cout << "Once your ready to write the image, press enter to proceed with this operation!";
        system("pause>nul");
        commandRun("dism /apply-image /imagefile:" + imgFile + " /index:" + to_string(indexImg) + " /applydir:" + winPar + ":""\"");
        cout << "Did you use MBR or GPT for disk partition scheme? [m/g] ";
        string gptan;
        cin >> gptan;
        if (gptan == "g") {
            commandRun("bcdboot " + winPar + ":""\\Windows"" /s " + efiPar + ":");
        }
        if (gptan == "m") {
            commandRun("bcdboot " + winPar + ":""\\Windows""");
        }
        cout << "Image write was successful! Press the enter key to go back to the main menu!";
        backMenu();
    }
    if (option == "n") {
        cout << "Please retry this operation to change your image file!";
        backMenu();
    }
}

void backMenu() {
    cout << "\n";
    cout << "Press the any key to exit to the main menu!";
    system("pause>nul");
    system("cls");
    main();
}

void createFi(string file, string text) {
    ofstream fileSel(file);
    fileSel << text;
    fileSel.close();
}

void setupDisk() {
    cout << "Check the README for how you can setup your disk!\n";
    system("diskpart");
    cout << "\n";
    cout << "Please be sure that you know what your drive letters that you set for your disk setup! \n";
    cout << "If you know what they are, you may exit this step!";
    backMenu();
}

//ofstream seldisk("seldisk.txt");
//seldisk << "sel disk ", x;
//seldisk.close();
//std::cout << "Would you like to use MBR or GPT for your disk? (M/G)" << std::endl;