#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>

using namespace std;

class RadioTechnics {
private:
    int id;
    string name;
    string manufacturer;
    string purpose;
    float frequency;
    double power;
    double weight;

public:
    // конструкторы
    RadioTechnics() : id(0), frequency(0), power(0), weight(0) {}
    RadioTechnics(int id, const string& name, const string& manufacturer,
                  const string& purpose, float frequency, double power, double weight)
        : id(id), name(name), manufacturer(manufacturer), purpose(purpose),
          frequency(frequency), power(power), weight(weight) {}

    // геттеры
    int getId() const { return id; }
    string getName() const { return name; } /// инкапсуляция получение приватного поля
    string getManufacturer() const { return manufacturer; }
    string getPurpose() const { return purpose; }
    float getFrequency() const { return frequency; }
    double getPower() const { return power; }
    double getWeight() const { return weight; }

    // сеттеры
    void setId(int id) { this->id = id; }
    void setName(const string& name) { this->name = name; } /// инкапсуляция изменение приватного поля
    void setManufacturer(const string& manufacturer) { this->manufacturer = manufacturer; }
    void setPurpose(const string& purpose) { this->purpose = purpose; }
    void setFrequency(float frequency) { this->frequency = frequency; }
    void setPower(double power) { this->power = power; }
    void setWeight(double weight) { this->weight = weight; }

    // ввод/ввывод
    static RadioTechnics fromStream(ifstream& in) {
        int id;
        string name, manufacturer, purpose;
        float frequency;
        double power, weight;
        in >> id >> name >> manufacturer >> purpose >> frequency >> power >> weight;
        return RadioTechnics(id, name, manufacturer, purpose, frequency, power, weight); /// вызов конструктора с параметрами
    }

    void toStream(ofstream& out) const {
        out << id << " " << name << " " << manufacturer << " " << purpose << " "
            << frequency << " " << power << " " << weight << "\n";
    }

    void display() const {
        cout << setw(5) << id << setw(15) << name << setw(20) << manufacturer
             << setw(15) << purpose << setw(10) << frequency << setw(10) << power
             << setw(10) << weight << "\n";
    }
};

// проверОчка
int getValidatedInt(const string& prompt);
double getValidatedDouble(const string& prompt);
float getValidatedFloat(const string& prompt);

RadioTechnics* loadFromFile(int& count);
void displayRecords(const RadioTechnics* records, int count);
void editRecord(RadioTechnics* records, int count);
void deleteRecord(RadioTechnics*& records, int& count);
void calculateAverageWeight(const RadioTechnics* records, int count);
void saveToFile(const RadioTechnics* records, int count);

int main() {
    RadioTechnics* records = nullptr;
    int count = 0;

    records = loadFromFile(count);

    int choice;
    do {
        cout << "\nProgram Menu:\n";
        cout << "1. View data\n";
        cout << "2. Edit record\n";
        cout << "3. Delete record\n";
        cout << "4. Calculate average weight\n";
        cout << "5. Save data to a file\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        choice = getValidatedInt("");

        switch (choice) {
            case 1:
                displayRecords(records, count);
                break;
            case 2:
                editRecord(records, count);
                break;
            case 3:
                deleteRecord(records, count);
                break;
            case 4:
                calculateAverageWeight(records, count);
                break;
            case 5:
                saveToFile(records, count);
                break;
            case 0:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    delete[] records;
    return 0;
}

RadioTechnics* loadFromFile(int& count) {
    string filename;
    RadioTechnics* records = nullptr;

    cout << "Enter the filename to load data: ";
    cin >> filename;

    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file.\n";
        return nullptr;
    }

    count = 0;
    string tempLine;
    while (getline(file, tempLine)) {
        ++count;
    }

    if (count <= 0) {
        cerr << "No valid records in the file.\n";
        return nullptr;
    }

    file.clear();
    file.seekg(0, ios::beg);

    records = new RadioTechnics[count];

    for (int i = 0; i < count; ++i) {
        records[i] = RadioTechnics::fromStream(file);
    }

    file.close();
    return records;
}

void displayRecords(const RadioTechnics* records, int count) {
    if (count == 0) {
        cout << "No records available.\n";
        return;
    }

    cout << left << setw(5) << "ID" << setw(15) << "Name" << setw(20) << "Manufacturer"
         << setw(15) << "Purpose" << setw(10) << "Frequency" << setw(10) << "Power"
         << setw(10) << "Weight\n";
    cout << string(75, '-') << "\n";

    for (int i = 0; i < count; ++i) {
        records[i].display();
    }
}

void editRecord(RadioTechnics* records, int count) {
    int id = getValidatedInt("Enter the ID of the record to edit: ");
    for (int i = 0; i < count; ++i) {
        if (records[i].getId() == id) {
            cout << "Editing record with ID " << id << ":\n";
            string name, manufacturer, purpose;
            cout << "New name: ";
            cin >> name;
            cout << "New manufacturer: ";
            cin >> manufacturer;
            cout << "New purpose: ";
            cin >> purpose;
            float frequency = getValidatedFloat("New frequency (Hz): ");
            double power = getValidatedDouble("New power (W): ");
            double weight = getValidatedDouble("New weight (kg): ");

            records[i].setName(name);
            records[i].setManufacturer(manufacturer);
            records[i].setPurpose(purpose);
            records[i].setFrequency(frequency);
            records[i].setPower(power);
            records[i].setWeight(weight);

            cout << "Record updated successfully.\n";
            return;
        }
    }
    cout << "Record with ID " << id << " not found.\n";
}

void deleteRecord(RadioTechnics*& records, int& count) {
    int id = getValidatedInt("Enter the ID of the record to delete: ");
    int index = -1;

    for (int i = 0; i < count; ++i) {
        if (records[i].getId() == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Record with ID " << id << " not found.\n";
        return;
    }

    for (int i = index; i < count - 1; ++i) {
        records[i] = records[i + 1];
    }

    --count;
    RadioTechnics* newRecords = new RadioTechnics[count];
    for (int i = 0; i < count; ++i) {
        newRecords[i] = records[i]; /// вызов конструктора копирования
    }

    delete[] records;
    records = newRecords;

    cout << "Record deleted successfully.\n";
}

void calculateAverageWeight(const RadioTechnics* records, int count) {
    if (count == 0) {
        cout << "No records available.\n";
        return;
    }

    double totalWeight = 0;
    for (int i = 0; i < count; ++i) {
        totalWeight += records[i].getWeight();
    }

    cout << "Average weight: " << totalWeight / count << " kg\n";
}

void saveToFile(const RadioTechnics* records, int count) {
    string filename;
    cout << "Enter the filename to save data: ";
    cin >> filename;

    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file for writing.\n";
        return;
    }

    file << count << "\n";
    for (int i = 0; i < count; ++i) {
        records[i].toStream(file);
    }
    file.close();
    cout << "Data saved successfully to " << filename << ".\n";
}

int getValidatedInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (!cin.fail()) {
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Try again.\n";
    }
}

double getValidatedDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (!cin.fail()) {
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Try again.\n";
    }
}

float getValidatedFloat(const string& prompt) {
    float value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (!cin.fail()) {
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Try again.\n";
    }
}
