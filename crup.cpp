#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

// --------------------------------------------------
// STRUCTURE
// --------------------------------------------------
struct Project
{
    int id;
    char name[50];
    int type; // 1-Street, 2-Highway

    double estimatedCost;
    double actualCost;
    double materialCost;
    double labourCostPerDay;
    int workers;
    int days;
    double roadLength;

    // Calculated (NOT stored in file)
    double expectedCost;
    double corruptionAmount;
    double corruptionPercent;
};

// --------------------------------------------------
// FUNCTION DECLARATIONS
// --------------------------------------------------
void login();
void menu();
void addProject();
void viewProjects();
void calculate(Project &p);
void corruptionDegree(double percent);
void sortByCorruption();
void searchProject();
void highestCorruption();
void averageCorruption();
void sortByRoadLength();
void criticalProjects();
void clearData();

// --------------------------------------------------
// MAIN
// --------------------------------------------------
int main()
{
    login();
    return 0;
}

// --------------------------------------------------
// LOGIN
// --------------------------------------------------
void login()
{
    string user, pass;
    int attempts = 3;

    while (attempts--)
    {
        cout << "\n==============================\n";
        cout << "        ADMIN LOGIN\n";
        cout << "==============================\n";
        cout << "Username: ";
        cin >> user;
        cout << "Password: ";
        cin >> pass;

        if (user == "emaan" && pass == "emn&152")
        {
            cout << "\nLogin Successful!\n";
            menu();
            return;
        }
        cout << "Wrong credentials. Attempts left: " << attempts << endl;
    }
    cout << "\nToo many failed attempts. Program terminated.\n";
}

// --------------------------------------------------
// MENU
// --------------------------------------------------
void menu()
{
    int choice;
    do
    {
        cout << "\n========================================\n";
        cout << "   CORRUPTION DETECTION SYSTEM\n";
        cout << "========================================\n";
        cout << "1. Add New Project\n";
        cout << "2. View All Projects\n";
        cout << "3. Sort Projects by Corruption\n";
        cout << "4. Search Project by ID\n";
        cout << "5. Highest Corruption Project\n";
        cout << "6. Average Corruption Percentage\n";
        cout << "7. Sort Projects by Road Length\n";
        cout << "8. Projects Above Critical Corruption\n";
        cout << "9. Clear All Project Data\n";
        cout << "0. Exit\n";
        cout << "----------------------------------------\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1: addProject(); break;
        case 2: viewProjects(); break;
        case 3: sortByCorruption(); break;
        case 4: searchProject(); break;
        case 5: highestCorruption(); break;
        case 6: averageCorruption(); break;
        case 7: sortByRoadLength(); break;
        case 8: criticalProjects(); break;
        case 9: clearData(); break;
        case 0: cout << "Exiting system...\n"; break;
        default: cout << "Invalid choice!\n";
        }
    } while (choice != 0);
}

// --------------------------------------------------
// ADD PROJECT (STORE ONLY RAW DATA)
// --------------------------------------------------
void addProject()
{
    Project p;
    ofstream file("data.txt", ios::app);

    cout << "\n========== ADD NEW PROJECT ==========\n";
    cout << "Project ID: ";
    cin >> p.id;
    cin.ignore();
    cout << "Project Name: ";
    cin.getline(p.name, 50);
    cout << "Type (1-Street, 2-Highway): ";
    cin >> p.type;
    cout << "Estimated Cost: ";
    cin >> p.estimatedCost;
    cout << "Actual Cost: ";
    cin >> p.actualCost;
    cout << "Material Cost: ";
    cin >> p.materialCost;
    cout << "Labour Cost Per Day: ";
    cin >> p.labourCostPerDay;
    cout << "Number of Workers: ";
    cin >> p.workers;
    cout << "Number of Days: ";
    cin >> p.days;
    cout << "Road Length (km): ";
    cin >> p.roadLength;

    // WRITE ONLY RAW INPUT
    file << p.id << endl
         << p.name << endl
         << p.type << endl
         << p.estimatedCost << endl
         << p.actualCost << endl
         << p.materialCost << endl
         << p.labourCostPerDay << endl
         << p.workers << endl
         << p.days << endl
         << p.roadLength << endl;

    file.close();
    cout << "\nProject added successfully!\n";
}

// --------------------------------------------------
// CALCULATION (ALWAYS DYNAMIC)
// --------------------------------------------------
void calculate(Project &p)
{
    double baseCost = (p.type == 1) ? 1000000 : 1800000;

    p.expectedCost =
        (p.roadLength * baseCost) +
        p.materialCost +
        (p.labourCostPerDay * p.workers * p.days);

    p.corruptionAmount = p.actualCost - p.estimatedCost;
    if (p.corruptionAmount < 0)
        p.corruptionAmount = 0;

    if (p.estimatedCost > 0)
        p.corruptionPercent =
            (p.corruptionAmount / p.estimatedCost) * 100;
    else
        p.corruptionPercent = 0;
}

// --------------------------------------------------
// VIEW PROJECTS
// --------------------------------------------------
void viewProjects()
{
    Project p;
    ifstream file("data.txt");

    if (!file)
    {
        cout << "No data available.\n";
        return;
    }

    cout << fixed << setprecision(2);
    cout << "\n========== PROJECT DETAILS ==========\n";

    while (file >> p.id)
    {
        file.ignore();
        file.getline(p.name, 50);
        file >> p.type >> p.estimatedCost >> p.actualCost
             >> p.materialCost >> p.labourCostPerDay
             >> p.workers >> p.days >> p.roadLength;

        calculate(p);

        cout << "\n------------------------------------\n";
        cout << "ID: " << p.id << endl;
        cout << "Name: " << p.name << endl;
        cout << "Type: " << (p.type == 1 ? "Street" : "Highway") << endl;
        cout << "Road Length: " << p.roadLength << " km\n";
        cout << "Corruption %: " << p.corruptionPercent << endl;
        corruptionDegree(p.corruptionPercent);
    }
    file.close();
}

// --------------------------------------------------
// CORRUPTION DEGREE
// --------------------------------------------------
void corruptionDegree(double p)
{
    if (p <= 5) cout << "Status: NO CORRUPTION\n";
    else if (p <= 15) cout << "Status: LOW CORRUPTION\n";
    else if (p <= 30) cout << "Status: MEDIUM CORRUPTION\n";
    else cout << "Status: HIGH CORRUPTION\n";
}

// --------------------------------------------------
// CLEAR DATA
// --------------------------------------------------
void clearData()
{
    char ch;
    cout << "\nWARNING: This will delete all project data!\n";
    cout << "Confirm (Y/N): ";
    cin >> ch;

    if (ch == 'Y' || ch == 'y')
    {
        ofstream file("data.txt", ios::trunc);
        file.close();
        cout << "All data cleared successfully.\n";
    }
    else
        cout << "Operation cancelled.\n";
}

// --------------------------------------------------
// SORT BY CORRUPTION
// --------------------------------------------------
void sortByCorruption()
{
    Project p[50];
    int n = 0;
    ifstream file("data.txt");

    while (file >> p[n].id)
    {
        file.ignore();
        file.getline(p[n].name, 50);
        file >> p[n].type >> p[n].estimatedCost >> p[n].actualCost
             >> p[n].materialCost >> p[n].labourCostPerDay
             >> p[n].workers >> p[n].days >> p[n].roadLength;

        calculate(p[n]);
        n++;
    }
    file.close();

    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (p[j].corruptionPercent < p[j + 1].corruptionPercent)
                swap(p[j], p[j + 1]);

    cout << "\n====== SORTED BY CORRUPTION ======\n";
    for (int i = 0; i < n; i++)
        cout << p[i].name << " | " << p[i].corruptionPercent << "%\n";
}

// --------------------------------------------------
// SEARCH PROJECT
// --------------------------------------------------
void searchProject()
{
    int id;
    cout << "Enter Project ID: ";
    cin >> id;

    Project p;
    ifstream file("data.txt");
    bool found = false;

    while (file >> p.id)
    {
        file.ignore();
        file.getline(p.name, 50);
        file >> p.type >> p.estimatedCost >> p.actualCost
             >> p.materialCost >> p.labourCostPerDay
             >> p.workers >> p.days >> p.roadLength;

        calculate(p);

        if (p.id == id)
        {
            cout << "\nProject Found: " << p.name
                 << "\nCorruption %: " << p.corruptionPercent << endl;
            corruptionDegree(p.corruptionPercent);
            found = true;
            break;
        }
    }
    if (!found)
        cout << "Project not found.\n";
    file.close();
}

// --------------------------------------------------
// HIGHEST CORRUPTION
// --------------------------------------------------
void highestCorruption()
{
    Project p, maxP;
    bool first = true;
    ifstream file("data.txt");

    while (file >> p.id)
    {
        file.ignore();
        file.getline(p.name, 50);
        file >> p.type >> p.estimatedCost >> p.actualCost
             >> p.materialCost >> p.labourCostPerDay
             >> p.workers >> p.days >> p.roadLength;

        calculate(p);

        if (first || p.corruptionPercent > maxP.corruptionPercent)
        {
            maxP = p;
            first = false;
        }
    }
    file.close();

    if (first)
        cout << "No data available.\n";
    else
        cout << "Highest Corruption: " << maxP.name
             << " (" << maxP.corruptionPercent << "%)\n";
}

// --------------------------------------------------
// AVERAGE CORRUPTION
// --------------------------------------------------
void averageCorruption()
{
    Project p;
    double sum = 0;
    int count = 0;
    ifstream file("data.txt");

    while (file >> p.id)
    {
        file.ignore();
        file.getline(p.name, 50);
        file >> p.type >> p.estimatedCost >> p.actualCost
             >> p.materialCost >> p.labourCostPerDay
             >> p.workers >> p.days >> p.roadLength;

        calculate(p);
        sum += p.corruptionPercent;
        count++;
    }
    file.close();

    if (count > 0)
        cout << "Average Corruption: " << sum / count << "%\n";
    else
        cout << "No projects available.\n";
}

// --------------------------------------------------
// SORT BY ROAD LENGTH
// --------------------------------------------------
void sortByRoadLength()
{
    Project p[50];
    int n = 0;
    ifstream file("data.txt");

    while (file >> p[n].id)
    {
        file.ignore();
        file.getline(p[n].name, 50);
        file >> p[n].type >> p[n].estimatedCost >> p[n].actualCost
             >> p[n].materialCost >> p[n].labourCostPerDay
             >> p[n].workers >> p[n].days >> p[n].roadLength;

        calculate(p[n]);
        n++;
    }
    file.close();

    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (p[j].roadLength < p[j + 1].roadLength)
                swap(p[j], p[j + 1]);

    cout << "\n====== SORTED BY ROAD LENGTH ======\n";
    for (int i = 0; i < n; i++)
        cout << p[i].name << " | " << p[i].roadLength << " km\n";
}

// --------------------------------------------------
// CRITICAL PROJECTS
// --------------------------------------------------
void criticalProjects()
{
    Project p;
    bool found = false;
    ifstream file("data.txt");

    while (file >> p.id)
    {
        file.ignore();
        file.getline(p.name, 50);
        file >> p.type >> p.estimatedCost >> p.actualCost
             >> p.materialCost >> p.labourCostPerDay
             >> p.workers >> p.days >> p.roadLength;

        calculate(p);

        if (p.corruptionPercent > 30)
        {
            cout << p.name << " | " << p.corruptionPercent << "%\n";
            found = true;
        }
    }
    if (!found)
        cout << "No critical projects found.\n";
    file.close();
}

