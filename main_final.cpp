#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <map>
#include <set>

using namespace std;

class Date {
public:
  Date (const int& a, const int& b, const int& c) {
    year = a;
    month = b;
    day = c;
  };
    int GetYear() const {
      return year;
    };
    int GetMonth() const {
      return month;
    };
    int GetDay() const {
      return day;
    };
private:
  int year = 0;
  int month = 0;
  int day = 0;
};

class Database {
public:
    void AddEvent(const Date& date, const string& event) {
      dbMap[date].insert(event);
    };

    bool DeleteEvent(const Date& date, const string& event) {
      bool fFind = false;
      auto it = dbMap.find(date);
      if (it == dbMap.end()) {
        cout << "Event not found" << endl;
        return false;
      } else {
        for (auto k = it->second.begin(); k != it->second.end(); k++) {
          if (*k == event) {
            fFind = true;
            dbMap[date].erase(k);
              cout << "Deleted successfully" << endl;
              return true;
          }
        }
        if (fFind == false) {
          cout << "Event not found" << endl;
          return false;
        }
      }
      return fFind;
    };

    int DeleteDate(const Date& date) {
      int i = 0;
      if (dbMap.count(date) > 0) {
        i = dbMap.at(date).size();
        dbMap.erase(date);
        return i;
      }
      return 0;
    };

    void Find(const Date& date) const {
      auto it = dbMap.find(date);
      if (it != dbMap.end()) {
        for(auto k = it->second.begin(); k != it->second.end(); k++) {
          cout << *k << endl;
        }
      }
    };

    void Print() const {
      for (auto k = dbMap.begin(); k != dbMap.end(); k++) {
        set<string> v = k->second;
        for (auto kk = v.begin(); kk != v.end(); kk++) {
          cout << setfill('0');
            cout << setw(4);
            cout << k->first.GetYear();
            cout << '-';
            cout << setw(2);
            cout << k->first.GetMonth();
            cout << '-';
            cout << setw(2);
            cout << k->first.GetDay();
            cout << ' ';

            cout << *kk << endl;
        }
      }
      return;
    };
private:
  map <Date, set<string>> dbMap;
};

ostream& operator<< (ostream& stream, const Date& date) {
  stream << date.GetYear() << '-' << date.GetMonth() << '-' << date.GetDay();
  return stream;
}

bool operator<(const Date& lhs, const Date& rhs) {
  if (lhs.GetYear() == rhs.GetYear()) {
    if (lhs.GetMonth() == rhs.GetMonth()) {
      return lhs.GetDay() < rhs.GetDay();
    } else {
      return lhs.GetMonth() < rhs.GetMonth();
    }
  } else {
    return lhs.GetYear() < rhs.GetYear();
  }
};

void CheckCommand (const string& str) {
  if (str == "Add" or str == "Del" or str == "Find" or str == "Print" or str == "") {
    return;
  } else {
    throw runtime_error("Unknown command: " + str);
  }
  return;
}

Date CreateDate (const string& date) {
  for (auto k : date) {
    if (k == '|' or k == '.' or k == ',') {
      throw runtime_error("Wrong date format " + date);
    }
  }
    if (date[0] == '-' and date[1] == '-') {
        throw runtime_error("Wrong date format: " + date);
    }
    for (unsigned int i = 2; i < date.size(); i++) {
        if (date[i-1] == '-' and date[i] == '-' and date[i+1] == '-') {
            throw runtime_error("Wrong date format: " + date);
        }
    }

    if (date[0] != '-' and !isdigit(date[0]) and date[0] != '+') {
      throw runtime_error("Wrong date format: " + date);
    }

    stringstream ss(date);
    int y = 0, m = 0, d = -1;
    bool flag = false;

    ss >> y;
    //cout << y << " y " << endl;
    if (ss.peek() != '-') {
      throw runtime_error("Wrong date format: " + date);
    }
    ss.ignore(1);
    if (!ss) {
      throw runtime_error("Wrong date format: " + date);
    }
    ss >> m;
    //cout << m << " m " << endl;
    if (ss.peek() != '-') {
      throw runtime_error("Wrong date format: " + date);
    }
    ss.ignore(1);
    if (!ss) {
      throw runtime_error("Wrong date format: " + date);
    }
    if (!(ss >> d)) {
      throw runtime_error("Wrong date format: " + date);
    }
    //ss >> d;
    //cout << d << " d " << endl;
    if(ss.peek() != ' ' and ss) {
      throw runtime_error("Wrong date format: " + date);
    }

    if (m < 1 or m > 12) {
      throw runtime_error("Month value is invalid: " + to_string(m));
    } else if (d < 1 or d > 31) {
      throw runtime_error("Day value is invalid: " + to_string(d));
    } else {
      flag = true;
    }

    if (flag) {
      return Date{y, m, d};
    }

    return Date{0, 0, 0};
}

int main() {
  Database db;
  string command = "", str = "", err = "", event = "", date = "", fss = "";

  try {
    while (getline(cin, str)) {
      stringstream ss(str);
      getline(ss, command, ' ');
      if (command == "") {
        getline(ss, command);
      }

      CheckCommand(command);

      if (command == "Print") {
        db.Print();
      } else {
        getline(ss, date, ' ');
        Date d = CreateDate(date);

        if (command == "Add") {
          getline(ss, event, '\n');
          db.AddEvent(d, event);
        } else if (command == "Del") {
          if (ss.rdbuf()->in_avail() == 0) {
            cout << "Deleted " << db.DeleteDate(d) << " events" << endl;
          } else {
            getline(ss, event, '\n');
            db.DeleteEvent(d, event);
          }
        } else if (command == "Find") {
          db.Find(d);
        } else if (command == "\n") {
          continue;
        }
      }
    }
  } catch (exception& ex) {
    cout << ex.what() << endl;
  }
    return 0;
}
