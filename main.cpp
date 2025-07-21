#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"

class Expense {
public:
  string category, date, note;
  double amount;

  Expense(string c, double a, string d, string n) {
    category = c;
    amount = a;
    date = d;
    note = n;
  }

  void display() const {
    cout << CYAN << fixed << setprecision(2);
    cout << "Category: " << category << ", Amount: ₹" << amount
         << ", Date: " << date << ", Note: " << note << RESET << endl;
  }
};

// Utility: Get total expenses
double getTotal(const vector<Expense> &expenses) {
  double total = 0;
  for (const auto &e : expenses)
    total += e.amount;
  return total;
}

// Utility: Get monthly expenses
double getMonthlyTotal(const vector<Expense> &expenses, const string &month) {
  double total = 0;
  for (const auto &e : expenses) {
    if (e.date.substr(0, 7) == month) {
      total += e.amount;
    }
  }
  return total;
}

void saveExpenses(const vector<Expense> &expenses) {
  ofstream file("expenses.txt");
  for (const auto &e : expenses) {
    file << e.category << "|" << e.amount << "|" << e.date << "|" << e.note
         << endl;
  }
  file.close();
}

void loadExpenses(vector<Expense> &expenses) {
  ifstream file("expenses.txt");
  string line;
  while (getline(file, line)) {
    stringstream ss(line);
    string category, date, note;
    double amount;

    getline(ss, category, '|');
    ss >> amount;
    ss.ignore();
    getline(ss, date, '|');
    getline(ss, note);

    expenses.push_back(Expense(category, amount, date, note));
  }
  file.close();
}

// Password management
void savePassword(const string &password) {
  ofstream file("password.txt");
  file << password;
  file.close();
}

string loadPassword() {
  ifstream file("password.txt");
  string password;
  if (file.is_open()) {
    getline(file, password);
    file.close();
  }
  return password;
}

// Budget management
void saveBudget(double budget) {
  ofstream file("budget.txt");
  file << budget;
  file.close();
}

double loadBudget() {
  ifstream file("budget.txt");
  double budget = 5000.00; // Default budget
  if (file.is_open()) {
    file >> budget;
    file.close();
  }
  return budget;
}

void showGraph(const vector<Expense> &expenses) {
  map<string, double> categoryTotal;
  for (const auto &e : expenses)
    categoryTotal[e.category] += e.amount;

  cout << YELLOW << "\n📊 Category-wise Expense Summary:\n" << RESET;
  for (const auto &entry : categoryTotal) {
    cout << entry.first << " | ";
    int bars = entry.second / 100; // ₹100 = 1 block
    for (int i = 0; i < bars; i++)
      cout << "█";
    cout << " ₹" << fixed << setprecision(2) << entry.second << endl;
  }
}

void clearExpenses(vector<Expense> &expenses) {
  char confirm;
  cout << RED
       << "\nAre you sure you want to clear ALL expenses? (y/n): " << RESET;
  cin >> confirm;
  if (confirm == 'y' || confirm == 'Y') {
    expenses.clear();
    saveExpenses(expenses);
    cout << GREEN << "✔ All expenses cleared!" << RESET << endl;
  } else {
    cout << YELLOW << "⚠ Cancelled. Your expenses are safe." << RESET << endl;
  }
}

int main() {
  cout << CYAN << "💰 Welcome to Personal Expense Tracker!\n" << RESET;
  cout << "📝 Your financial companion for tracking daily expenses.\n\n";

  vector<Expense> expenses;
  loadExpenses(expenses);

  double budgetLimit = 1000.00;
  double monthlyBudget = loadBudget(); // Load user's monthly budget

  string storedPassword = loadPassword();
  string inputPassword;

  if (storedPassword.empty()) {
    cout << "🌐 First time setup!\n";
    cout << "Create your secure password: ";
    cin >> storedPassword;
    savePassword(storedPassword);
    cout << GREEN << "✅ Password saved successfully!\n" << RESET;

    cout << "\n💰 Set your monthly budget limit:\n";
    cout << "This will help you track your spending and get alerts when you "
            "exceed it.\n";
    cout << "Enter your monthly budget: ₹";
    cin >> monthlyBudget;
    saveBudget(monthlyBudget);
    cout << GREEN << "✅ Monthly budget set to ₹" << fixed << setprecision(2)
         << monthlyBudget << RESET << endl;
    cout
        << "You'll get alerts when your monthly spending exceeds this limit.\n";
    cout << "Your data will be automatically saved to files.\n\n";
  }

  cout << "🔒 Enter password to access your Expense Tracker: ";
  cin >> inputPassword;
  if (inputPassword != storedPassword) {
    cout << RED << "❌ Access Denied. Please restart and try again.\n" << RESET;
    return 0;
  }

  cout << GREEN << "✅ Welcome back! You have " << expenses.size()
       << " expenses recorded.\n"
       << RESET;

  int choice;
  do {
    cout << GREEN << "\n📋 Expense Tracker Menu\n" << RESET;
    cout << "1. Add Expense\n";
    cout << "2. Show All Expenses\n";
    cout << "3. Filter by Month\n";
    cout << "4. Show Graph Summary\n";
    cout << "5. Show Total\n";
    cout << "6. Set Monthly Budget\n";
    cout << "7. Check Monthly Budget Status\n";
    cout << "8. Reset Password\n";
    cout << "9. Clear All Expenses\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
      string category, date, note;
      double amount;
      cout << "Category: ";
      getline(cin, category);
      cout << "Amount: ";
      cin >> amount;
      cout << "Date (YYYY-MM-DD): ";
      cin >> date;
      cin.ignore();
      cout << "Note: ";
      getline(cin, note);

      expenses.push_back(Expense(category, amount, date, note));
      saveExpenses(expenses);
      cout << GREEN << "✔ Expense Added and Saved!" << RESET << endl;

      double total = getTotal(expenses);
      if (total > budgetLimit) {
        cout << RED << "⚠️ Budget limit exceeded! Total: ₹" << total
             << RESET << endl;
      }

      string currentMonth = date.substr(0, 7);
      double monthlyTotal = getMonthlyTotal(expenses, currentMonth);
      if (monthlyTotal > monthlyBudget) {
        double excess = monthlyTotal - monthlyBudget;
        cout << RED << "\n🚨 BUDGET ALERT! STOP SPENDING! 🚨\n";
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        cout << "💸 You've exceeded your monthly budget!\n";
        cout << "🗕 Month: " << currentMonth << "\n";
        cout << "💰 Your Budget: ₹" << fixed << setprecision(2) << monthlyBudge
              << "\n";
        cout << "📊 Total Spent: ₹" << monthlyTotal << "\n";
        cout << "⚠️  Over Budget by: ₹" << excess << "\n";
        cout << "💡 RECOMMENDATION: Start saving NOW!\n";
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << endl;
      } else {
        double remaining = monthlyBudget - monthlyTotal;
        double percentage = (monthlyTotal / monthlyBudget) * 100;
        cout << CYAN << "\n📊 Budget Status for " << currentMonth << ":\n";
        cout << "💰 Budget: ₹" << monthlyBudget << " | Spent: ₹" << monthlyTotal;
        cout << " | Remaining: ₹" << remaining << "\n";
        cout << "📈 Usage: " << fixed << setprecision(1) << percentage << "%
              << RESET << endl;

        if (percentage > 80) {
          cout << YELLOW << "\n⚠️  Warning: You've used " << percentage
               << "% of your budget!" << RESET;
        }
        cout << endl;
      }
    }

    else if (choice == 2) {
      cout << CYAN << "\n📁 All Expenses:\n" << RESET;
      for (const auto &e : expenses)
        e.display();
    }

    else if (choice == 3) {
      string month;
      cout << "Enter month to filter (YYYY-MM): ";
      cin >> month;
      cout << YELLOW << "\n📅 Expenses for " << month << ":\n" << RESET;
      for (const auto &e : expenses) {
        if (e.date.substr(0, 7) == month)
          e.display();
      }
    }

    else if (choice == 4) {
      showGraph(expenses);
    }

    else if (choice == 5) {
      double total = getTotal(expenses);
      cout << CYAN << "💰 Total Expenses: ₹" << total << RESET << endl;
    }

    else if (choice == 6) {
      cout << "Current monthly budget: ₹" << fixed << setprecision(2)
           << monthlyBudget << endl;
      cout << "Enter new monthly budget: ₹";
      cin >> monthlyBudget;
      saveBudget(monthlyBudget);
      cout << GREEN << "✔ Monthly budget updated to ₹" << fixed
           << setprecision(2) << monthlyBudget << RESET << endl;
    }

    else if (choice == 7) {
      string month;
      cout << "Enter month to check (YYYY-MM): ";
      cin >> month;
      double monthlyTotal = getMonthlyTotal(expenses, month);

      cout << CYAN << "\n📊 Budget Status for " << month << ":\n" << RESET;
      cout << "Monthly Budget: ₹" << fixed << setprecision(2) << monthlyBudget
           << endl;
      cout << "Total Spent: ₹" << monthlyTotal << endl;

      if (monthlyTotal > monthlyBudget) {
        double excess = monthlyTotal - monthlyBudget;
        cout << RED << "🚨 BUDGET EXCEEDED by ₹" << excess << " 🚨" << RESET
             << endl;
      } else {
        double remaining = monthlyBudget - monthlyTotal;
        cout << GREEN << "✅ Within budget! Remaining: ₹" << remaining << RESET
             << endl;
      }

      double percentage = (monthlyTotal / monthlyBudget) * 100;
      cout << "Budget Usage: " << fixed << setprecision(1) << percentage << "%"
           << endl;
    }

    else if (choice == 8) {
      string oldPassword, newPassword;
      cout << "Enter current password: ";
      cin >> oldPassword;
      if (oldPassword != storedPassword) {
        cout << RED << "❌ Incorrect password!" << RESET << endl;
      } else {
        cout << "Enter new password: ";
        cin >> newPassword;
        savePassword(newPassword);
        storedPassword = newPassword;
        cout << GREEN << "✅ Password updated successfully!" << RESET << endl;
      }
    }

    else if (choice == 9) {
      clearExpenses(expenses);
    }

  } while (choice != 0);

  saveExpenses(expenses);
  cout << GREEN << "\n📀 All data saved successfully!" << RESET << endl;
  cout << GREEN << "👋 Thank you for using Personal Expense Tracker!" << RESET
       << endl;
  cout << "📊 Total expenses tracked: " << expenses.size() << endl;
  cout << "💰 Total amount: ₹" << fixed << setprecision(2) << getTotal(expenses)
       << endl;
  return 0;
}