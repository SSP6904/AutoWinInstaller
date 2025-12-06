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
#include <vector>

using namespace std;

void WriteImageFile();
void DiskPartionSetup();
string writeDiskScript(string content);
void commandRun(string cmd);

int main() {
    SetConsoleTitle(L"Automated Windows Installer");
    cout << "Menu selection: \n";
    cout << "[1] - Setup the disk\n";
    cout << "[2] - Write the image file\n";
    cout << "[3] - Quit the application\n";
    cout << "Enter the number: ";
    while (true) {
        int option = _getch();
        switch (option) {
            case '1':
                system("cls");
                DiskPartionSetup();
                break;
            case '2':
                system("cls");
                WriteImageFile();
                break;
            case '3':
                exit(-1);
                break;
            default:
                cout << "Invalid option! Please try again!";
                cin.ignore();
                system("cls");
                main();
                break;
        }
    }
}

void DiskPartionSetup() {
    // Variables
    string disk_number;

    // Warning
    cout << "Make sure that you have backed up any important data from your hard disk before you continue forward!\n";
    cout << "If you are ready to begin, press the enter key!\n";
    if (cin.ignore()) {
        // Step 1
        commandRun("diskpart /s ./" + writeDiskScript(
            "list disk\n"
            "exit"
        ));
        cout << "Enter the disk number from above: ";
        cin >> disk_number;

        // Step 2
        cout << "Will you be using GPT or MBR for your partition table? [m/g]";
        while (true) {
            int disk_type = _getch();
            switch (disk_type) {
                case 'm':
                case 'M':
                    commandRun("diskpart /s " + writeDiskScript(
                        "select disk " + disk_number + "\n"
                        "clean\n"
                        "convert mbr\n"
                        "exit"
                    ));
                    cout << "\n";
                    cout << "Creating the required partitons! Please wait a moment!";
                    commandRun("diskpart /s " + writeDiskScript(
                        "select disk " + disk_number + "\n"
                        "create part primary size=100\n"
                        "format fs=ntfs label=SYSTEM quick\n"
                        "assign letter w\n"
                        "active\n"
                        "create par primary\n"
                        "format fs=ntfs label=Windows quick\n"
                        "assign letter r\n"
                        "exit"
                    ));
                    system("cls");
                    cout << "All done with this step! You can now write the image file! Make sure that you remember these drive letters for later!\n";
                    cout << "Boot partition: W:\n";
                    cout << "Windows partition: R:\n";
                    remove("./diskpart_script.txt");
                    system("pause>nul");
                    system("cls");
                    main();
                    break;
                case 'g':
                case 'G':
                    commandRun("diskpart /s " + writeDiskScript(
                        "select disk " + disk_number + "\n"
                        "clean\n"
                        "convert gpt\n"
                        "exit"
                    ));
                    cout << "\n";
                    cout << "Creating the required partitons! Please wait a moment!";
                    commandRun("diskpart /s " + writeDiskScript(
                        "select disk " + disk_number + "\n"
                        "create part EFI size=500\n"
                        "format fs=fat32 label=EFI quick\n"
                        "assign letter w\n"
                        "create par primary\n"
                        "format fs=ntfs label=Windows quick\n"
                        "assign letter r\n"
                        "exit"
                    ));
                    system("cls");
                    cout << "All done with this step! You can now write the image file! Make sure that you remember these drive letters for later!\n";
                    cout << "Boot partition: W:\n";
                    cout << "Windows partition: R:\n";
                    remove("./diskpart_script.txt");
                    system("pause>nul");
                    system("cls");
                    main();
                    break;
                default:
                    system("cls");
                    main();
                    break;
            }
        }
    }
}

void WriteImageFile() {
    int indexImg;
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
            int win_type = _getch();
            switch (win_type) {
                case 'm':
                case 'M': {
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
                    break;
                }
                case 'g':
                case 'G': {
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
                    break;
                }
                default:
                    system("cls");
                    main();
                    break;
            }
        }
    }
    else {
        cout << "The image file does not exist! Please try again!";
        cin.ignore();
        system("cls");
        WriteImageFile();
    }
}

// Helper functions
void commandRun(string cmd) {
    int runCmd = system(cmd.c_str());
    if (runCmd != 0) {
        cout << "\n";
        cout << "Something went wrong while trying to run this command!\n";
        cout << "Press any key to exit the program";
        cin.ignore();
        exit(-1);
    }
}

string writeDiskScript(string content) {
    string file_name = "./diskpart_script.txt";
    ofstream scriptFile(file_name);
    if (scriptFile.is_open()) {
        scriptFile << content.c_str();
        scriptFile.close();
        return file_name;
    }
    else {
        perror("Something went wrong while trying to create the diskpart script! Exiting now!");
        cin.ignore();
        exit(-1);
    }
}

//char tempPath[MAX_PATH];
//char tempFileName[MAX_PATH];
//UINT uniqueNum;
//GetTempPathA(MAX_PATH, tempPath);
//uniqueNum = GetTempFileNameA(tempPath, "tempScript", 0, tempFileName);
//DeleteFileA(tempFileName);