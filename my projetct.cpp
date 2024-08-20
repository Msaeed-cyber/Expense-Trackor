#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>
using namespace std;

class Expense {
private:
    string name;
    double amount;
    string category;
public:
    Expense(string n, double a, string c) : name(n), amount(a), category(c) {}

    // getter and setter functions
    string getName() const {
        return name;
    }

    double getAmount() const {
        return amount;
    }

    string getCategory() const {
        return category;
    }

    void display() const {
        cout << setw(20) << left << name << setw(15) << category << setw(10) << fixed << setprecision(2) << amount << endl;
    }
};

class IExpenseTracker {
public:
    virtual ~IExpenseTracker() = default;
    virtual void addExpense(const Expense& expense) = 0;
    virtual double getTotalExpenses() const = 0;
    virtual vector<Expense> getExpensesByCategory(const string& category) const = 0;
    virtual void editExpense(const string& oldName, const string& newName, double newAmount, const string& newCategory) = 0;
    virtual void deleteExpense(const string& name) = 0;
    virtual void displayAllExpenses() const = 0;
    virtual void generateReport() const = 0;
    virtual void saveToFile(const string& filename) const = 0;
    virtual void setSalary(double salary) = 0;
};

class ExpenseTracker : public IExpenseTracker {
private:
    vector<Expense> expenses;
    double salary;

    // Recursive helper function to calculate total expenses
    double getTotalExpensesRecursively(int index) const {
        if (index == expenses.size()) {
            return 0.0;
        }
        return expenses[index].getAmount() + getTotalExpensesRecursively(index + 1);
    }

public:
    ExpenseTracker() : salary(0.0) {}

    void addExpense(const Expense& expense) override {
        expenses.push_back(expense);
    }

    double getTotalExpenses() const override {
        return getTotalExpensesRecursively(0);
    }

    vector<Expense> getExpensesByCategory(const string& category) const override {
        vector<Expense> result;
        for (const Expense& expense : expenses) {
            if (expense.getCategory() == category) {
                result.push_back(expense);
            }
        }
        return result;
    }

    // edit expense function
    void editExpense(const string& oldName, const string& newName, double newAmount, const string& newCategory) override {
        for (Expense& expense : expenses) {
            if (expense.getName() == oldName) {
                expense = Expense(newName, newAmount, newCategory);
                cout << "The expense is edited successfully." << endl;
                return;
            }
        }
        cout << "Expense not found." << endl;
    }

    // delete function
    void deleteExpense(const string& name) override {
        for (auto it = expenses.begin(); it != expenses.end(); ++it) {
            if (it->getName() == name) {
                expenses.erase(it);
                cout << "The expense is deleted successfully." << endl;
                return;
            }
        }
        cout << "Expense not found." << endl;
    }

    // display function
    void displayAllExpenses() const override {
        cout << setw(20) << left << "Name" << setw(15) << "Category" << setw(10) << "Amount" << endl;
        cout << "----------------------------------------------------------" << endl;
        for (const Expense& expense : expenses) {
            expense.display();
        }
        double totalExpenses = getTotalExpenses();
        cout << "----------------------------------------------------------" << endl;
        cout << setw(35) << left << "Total Expenses: " << setw(10) << totalExpenses << endl;
        cout << setw(35) << left << "Salary: " << setw(10) << salary << endl;
        if (salary >= totalExpenses) {
            cout << "You are within your budget." << endl;
        } else {
            cout << "You have exceeded your budget by " << totalExpenses - salary << endl;
        }
        // type casting
        int totalExpensesInt = static_cast<int>(totalExpenses);
        cout << "Total Expenses (rounded): " << totalExpensesInt << endl;
    }

    // file handling
    void saveToFile(const string& filename) const override {
        ofstream file(filename);
        if (file.is_open()) {
            for (const Expense& expense : expenses) {
                file << expense.getName() << "," << expense.getAmount() << "," << expense.getCategory() << endl;
            }
            file.close();
            cout << "Expenses saved to " << filename << endl;
        } else {
            cout << "Unable to open file." << endl;
        }
    }

    // salary input function
    void setSalary(double salary) override {
        this->salary = salary;
    }

    // operator overloading
    ExpenseTracker& operator+=(const Expense& expense) {
        this->addExpense(expense);
        return *this;
    }

    // Generate report function
    void generateReport() const {
        map<string, double> categoryTotals;
        for (const Expense& expense : expenses) {
            categoryTotals[expense.getCategory()] += expense.getAmount();
        }

        cout << setw(20) << left << "Category" << setw(15) << "Total Amount" << endl;
        cout << "----------------------------------------------------------" << endl;
        for (const auto& category : categoryTotals) {
            cout << setw(20) << left << category.first << setw(15) << fixed << setprecision(2) << category.second << endl;
        }
        double totalExpenses = getTotalExpenses();
        cout << "----------------------------------------------------------" << endl;
        cout << setw(35) << left << "Total Expenses: " << setw(10) << totalExpenses << endl;
    }
};

void clearScreen() {
    #ifdef _WIN32
        system("cls"); // For Windows
    #else
        system("clear"); // For Unix-based systems
    #endif
}

void addExpense(IExpenseTracker& tracker) {
    int category;
    cout << "Expense categories are:" << endl;
    cout << "1: Groceries" << endl;
    cout << "2: Transport" << endl;
    cout << "3: Utilities" << endl;
    cout << "4: Medical" << endl;
    cout << "5: Shopping" << endl;
    cout << "Enter category number: ";
    cin >> category;

    cin.ignore(); // Clear newline character from input buffer

    if (category == 1) {
        string name;
        double amount, foodItem, beverage, householdProduct;
        cout << "Enter FoodItem Expense: ";
        cin >> foodItem;
        cout << "Enter Beverage Expense: ";
        cin >> beverage;
        cout << "Enter Household Products Expense: ";
        cin >> householdProduct;
        cin.ignore();
        cout << "Enter name of grocery item: ";
        getline(cin, name);
        cout << "Enter amount: ";
        cin >> amount;
        cin.ignore();

        tracker.addExpense(Expense("Food Item", foodItem, "Groceries"));
        tracker.addExpense(Expense("Beverage", beverage, "Groceries"));
        tracker.addExpense(Expense("Household Product", householdProduct, "Groceries"));
        tracker.addExpense(Expense(name, amount, "Groceries"));
    } else if (category == 2) {
        string name;
        double amount, fuel, vehicleInsurance;
        cout << "Enter Fuel Expense: ";
        cin >> fuel;
        cout << "Enter Vehicle Insurance Expense: ";
        cin >> vehicleInsurance;
        cin.ignore();
        cout << "Enter transport expense name: ";
        getline(cin, name);
        cout << "Enter amount: ";
        cin >> amount;

        tracker.addExpense(Expense("Fuel", fuel, "Transport"));
        tracker.addExpense(Expense("Vehicle Insurance", vehicleInsurance, "Transport"));
        tracker.addExpense(Expense(name, amount, "Transport"));
    } else if (category == 3) {
        string name;
        double amount, electricity, water, naturalGas, internet, houseRent;
        cout << "Enter Electricity Expense: ";
        cin >> electricity;
        cout << "Enter Water Expense: ";
        cin >> water;
        cout << "Enter Natural Gas Expense: ";
        cin >> naturalGas;
        cout << "Enter Internet Expense: ";
        cin >> internet;
        cout << "Enter House Rent Expense: ";
        cin >> houseRent;
        cin.ignore();
        cout << "Enter utility expense name: ";
        getline(cin, name);
        cout << "Enter amount: ";
        cin >> amount;
        cin.ignore();

        tracker.addExpense(Expense("Electricity", electricity, "Utilities"));
        tracker.addExpense(Expense("Water", water, "Utilities"));
        tracker.addExpense(Expense("Natural Gas", naturalGas, "Utilities"));
        tracker.addExpense(Expense("Internet", internet, "Utilities"));
        tracker.addExpense(Expense("House Rent", houseRent, "Utilities"));
        tracker.addExpense(Expense(name, amount, "Utilities"));
    } else if (category == 4) {
        string name;
        double amount, checkUp, medicine, surgery;
        cout << "Enter Checkup Expense: ";
        cin >> checkUp;
        cout << "Enter Medicine Expense: ";
        cin >> medicine;
        cout << "Enter Surgery Expense: ";
        cin >> surgery;
        cin.ignore();
        cout << "Enter medical expense name: ";
        getline(cin, name);
        cout << "Enter amount: ";
        cin >> amount;
        cin.ignore();

        tracker.addExpense(Expense("Checkup", checkUp, "Medical"));
        tracker.addExpense(Expense("Medicine", medicine, "Medical"));
        tracker.addExpense(Expense("Surgery", surgery, "Medical"));
        tracker.addExpense(Expense(name, amount, "Medical"));
    } else if (category == 5) {
        string name;
        double amount, clothes, shoes;
        cout << "Enter Clothes Expense: ";
        cin >> clothes;
        cout << "Enter Shoes Expense: ";
        cin >> shoes;
        cin.ignore();
        cout << "Enter shopping expense name: ";
        getline(cin, name);
        cout << "Enter amount: ";
        cin >> amount;
        cin.ignore();

        tracker.addExpense(Expense("Clothes", clothes, "Shopping"));
        tracker.addExpense(Expense("Shoes", shoes, "Shopping"));
        tracker.addExpense(Expense(name, amount, "Shopping"));
    } else {
        cout << "Invalid category number. Expense not added." << endl;
    }
}

void editExpense(IExpenseTracker& tracker) {
    clearScreen(); // Clear screen before editing expense
    string oldName, newName, newCategory;
    double newAmount;

    cout << "Enter the name of the expense to edit: ";
    cin.ignore();
    getline(cin, oldName);
    cout << "Enter the new name: ";
    getline(cin, newName);
    cout << "Enter the new amount: ";
    cin >> newAmount;
    cin.ignore();
    cout << "Enter the new category: ";
    getline(cin, newCategory);

    tracker.editExpense(oldName, newName, newAmount, newCategory);
}

void deleteExpense(IExpenseTracker& tracker) {
    clearScreen(); // Clear screen before deleting expense
    string name;
    cout << "Enter the name of the expense to delete: ";
    cin.ignore();
    getline(cin, name);

    tracker.deleteExpense(name);
}

void displayExpenses(IExpenseTracker& tracker) {
    clearScreen(); // Clear screen before displaying expenses
    tracker.displayAllExpenses();
}

void generateExpenseReport(const IExpenseTracker& tracker) {
    clearScreen(); // Clear screen before generating report
    tracker.generateReport();
}

void saveExpensesToFile(const IExpenseTracker& tracker) {
    clearScreen(); // Clear screen before saving expenses
    string filename;
    cout << "Enter filename to save expenses: ";
    cin >> filename;

    tracker.saveToFile(filename);
}

void setSalary(IExpenseTracker& tracker) {
    clearScreen(); // Clear screen before setting salary
    double salary;
    cout << "Enter your salary: ";
    cin >> salary;

    tracker.setSalary(salary);
}

void showMenu() {
    cout << "************ Menu ************" << endl;
    cout << "1. Set Salary" << endl;
	cout << "2. Add Expense" << endl;
    cout << "3. Edit Expense" << endl;
    cout << "4. Delete Expense" << endl;
    cout << "5. Display All Expenses" << endl;
    cout << "6. Generate Expense Report" << endl;
    cout << "7. Save Expenses to File" << endl;
   
    cout << "8. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    ExpenseTracker tracker;
    int choice;

    do {
        clearScreen(); // Clear screen before showing the menu
        showMenu();
        cin >> choice;

        // Process user choice
        switch (choice) {
        	case 1:
        		setSalary(tracker);
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            case 2:
                addExpense(tracker);
                break;
            case 3:
                editExpense(tracker);
                break;
            case 4:
                deleteExpense(tracker);
                break;
            case 5:
                displayExpenses(tracker);
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            case 6:
                generateExpenseReport(tracker);
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            case 7:
                saveExpensesToFile(tracker);
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            case 8:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
        }

    } while (choice != 8);

    return 0;
}