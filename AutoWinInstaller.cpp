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

void WriteImg();
void commandRun(string cmd);
void createFile(string file, string text);

int main() {
    int option;
    SetConsoleTitle(L"Automated Windows Installer");
    
    cout << "Welcome to the Automated Windows Installer!\n";
    cout << "Created by SSP6904 (https://shaunhoffer.cc)\n";
    cout << "Select one of the options below to get started!\n";
    cout << "1 - Setup the disk\n";
    cout << "2 - Write the image file\n";
    cout << "3 - Quit the application\n";
    cout << "Enter the number: ";
    while (true) {
        option = _getch();
        if (option == '1') {
            system("cls");
            cout << "Check the README for how you can setup your disk!\n";
            system("diskpart");
            cout << "\n";
            cout << "Please be sure that you know what your drive letters that you set for your disk setup! \n";
            cout << "If you know what they are, you may exit this step!";
            cin.ignore();
            system("cls");
            main();
        }
        else if (option == '2') {
            system("cls");
            WriteImg();
        }
        else if (option == '3') {
            exit(-1);
        }
        else {
            cout << "Invalid option! Please try again!";
            cin.ignore();
            system("cls");
            main();
        }
    }
}

void commandRun(string cmd) {
    int runCmd = system(cmd.c_str());
    if (runCmd != 0) {
        cout << "Something went wrong while trying to run this command! Sorry about that! If this is an issue with the program, please submit it to this program's GitHub repository!";
        cout << "Press any key to exit the program";
        cin.ignore();
        exit(-1);
    }
}

void WriteImg() {
    int indexImg;
    char gptan;
    string imgFile;

    cout << "Enter the WIM file path: ";
    cin >> imgFile;
    ifstream file(imgFile);

	// Checks if the image file exists
	if (file.is_open()) {
        // Grabs the data from the image file and asks for the index number
        commandRun("dism /Get-WimInfo /WimFile:" + imgFile + "");
        cout << "Input the index number from above: ";
        cin >> indexImg;

        if (indexImg == NULL) {
            cout << "The number you typed in is either invaild or is not a number! Please try again! Exiting the program!";
            cin.ignore();
            exit(-1);
        }

        // Asks if the disk partition scheme is MBR or GPT
        cout << "Are you using either MBR or GPT for the disk partition scheme? [m/g] ";

        while (true) {
            gptan = _getch();
            // Choose this if the partion scheme is GPT
            if (gptan == 'g') {
                string efiPar;
                string winPar;
                cout << "\n";
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

                cin.ignore();
                system("cls");
                main();
            }
            // Choose this if the partion scheme is MBR
            else if (gptan == 'm') {
                string winPar;
                string sysresPar;
                cout << "\n";
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

                cin.ignore();
                system("cls");
                main();
            }
        }
    }
    else {
        cout << "The image file does not exist! Please try again!";
        cin.ignore();
        system("cls");
        WriteImg();
    }
}

void createFile(string file, string text) {
    ofstream fileSel(file);
    fileSel << text;
    fileSel.close();
}

//ofstream seldisk("seldisk.txt");
//seldisk << "sel disk ", x;
//seldisk.close();
//std::cout << "Would you like to use MBR or GPT for your disk? (M/G)" << std::endl;