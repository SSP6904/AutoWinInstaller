// AutoWinInstaller.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS
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
	cout << "Welcome to the Automated Windows Installer!\n";
    cout << "==============================================\n";
    cout << "Created by SSP6904 (https://shaunhoffer.cc)\n";
    cout << "If you're ready to go, press the enter key to continue! Otherwise, you may close this application!";
	if (cin.ignore()) {
		system("cls");
		optionSel();
	}
}

void commandRun(string cmd) {
    int runCmd = system(cmd.c_str());
    if (runCmd != 0) {
        cout << "Something went wrong while trying to run this command! Sorry about that! If this is an issue with the program, please submit it to the programs GitHub repository!";
        cout << "Press any key to exit the program";
        cin.ignore();
        exit(-1);
    }
}

void optionSel() {
	int option = 0;
    cout << "1 - Setup the disk\n";
    cout << "2 - Write the image file\n";
    cout << "3 - Quit the application\n";
    cout << "Enter the number: ";
    cin >> option;
    switch (option) {
        // Setup the disk
	    case 1:
		    system("cls");
		    setupDisk();
		    break;
	    // Write the image file
        case 2:
            system("cls");
		    WriteImg();
		    break;
	    // Quit the application
	    case 3:
		    exit(-1);
	    // Invalid option (default)
        default:
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

	// Checks if the image file exists
	if (fopen(imgFile.c_str(), "r") == NULL) {
		cout << "The image file does not exist! Please try again!";
		system("pause>nul");
		system("cls");
		WriteImg();
	}
    int indexImg;
    string gptan;

	// Grabs the data from the image file and asks for the index number
    commandRun("dism /Get-WimInfo /WimFile:" + imgFile + "");
    cout << "Input your index number from above: ";
    cin >> indexImg;

	// Asks if the disk partition scheme is MBR or GPT
    cout << "Did you use MBR or GPT for disk partition scheme? [m/g] ";
    cin >> gptan;
    
	// Choose this if the partion scheme is GPT
    if (gptan == "g") {
        string efiPar;
        string winPar;
        cout << "Input the EFI partiton letter: ";
        cin >> efiPar;
        cout << "Input the Windows partition letter: ";
        cin >> winPar;
        cout << "Starting the image writing! Please do not close this window while the writing is in progress!\n";
        commandRun("dism /apply-image /imagefile:" + imgFile + " /index:" + to_string(indexImg) + " /applydir:" + winPar + ":""\"");
        cout << "Adding the required boot files. Please wait!\n";
        commandRun("bcdboot " + winPar + ":""\\Windows"" /s " + efiPar + ":"" /f UEFI");
        system("cls");
        cout << "Image write was successful! Press the enter key to go back to the main menu!";
        backMenu();
    }

	// Choose this if the partion scheme is MBR
    if (gptan == "m") {
        string winPar;
        string sysresPar;
        cout << "Input the Windows partition letter: ";
        cin >> winPar;
        cout << "Input the System Reserved partition letter: ";
        cin >> sysresPar;
        cout << "Starting the image writing! Please do not close this window while the writing is in progress!\n";
        commandRun("dism /apply-image /imagefile:" + imgFile + " /index:" + to_string(indexImg) + " /applydir:" + winPar + ":""\"");
        cout << "Adding the required boot files. Please wait!\n";
        commandRun("bcdboot " + winPar + ":""\\Windows"" /s " + sysresPar + ":"" /f BIOS");
        system("cls");
        cout << "Image write was successful! Press the enter key to go back to the main menu!";
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