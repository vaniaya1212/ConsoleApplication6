#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

enum class SourceType {
    BOOK,
    MAGAZINE,
    NEWSPAPER
};

class Source {
private:
    SourceType type;
    string title;
    string author;
    string category;
    int year;

public:
    Source(SourceType type, const string& title, const string& author = "", const string& category = "", int year = 0)
        : type(type), title(title), author(author), category(category), year(year) {}

    SourceType getType() const { return type; }
    const string& getTitle() const { return title; }
    const string& getAuthor() const { return author; }
    const string& getCategory() const { return category; }
    int getYear() const { return year; }

    void display() const {
        cout << "Type: " << (type == SourceType::BOOK ? "Book" : (type == SourceType::MAGAZINE ? "Magazine" : "Newspaper")) << "\n"
            << "Title: " << title << "\n";
        if (type == SourceType::BOOK) {
            cout << "Author: " << author << "\n"
                << "Category: " << category << "\n";
        }
        else {
            cout << "Year: " << year << "\n";
        }
        cout << "-------------------------\n";
    }
};

class Library {
private:
    vector<Source*> sources;

public:
    ~Library() {
        for (auto source : sources) {
            delete source;
        }
    }

    void fillDatabase() {
        int count;
        cout << "How many sources to add: ";
        cin >> count;
        cin.ignore();
        for (int i = 0; i < count; ++i) {
            addSource();
        }
    }

    void addSource() {
        int typeInput;
        cout << "Choose type (1 - Book, 2 - Magazine, 3 - Newspaper): ";
        cin >> typeInput;
        cin.ignore();

        SourceType type = static_cast<SourceType>(typeInput - 1);
        string title, author, category;
        int year = 0;

        cout << "Enter title: ";
        getline(cin, title);

        if (type == SourceType::BOOK) {
            cout << "Enter author: ";
            getline(cin, author);
            cout << "Enter category (fiction, detective, etc.): ";
            getline(cin, category);
        }
        else {
            cout << "Enter year: ";
            cin >> year;
            cin.ignore();
        }

        sources.push_back(new Source(type, title, author, category, year));
        cout << "Source added to database.\n";
    }

    void displayAll() const {
        if (sources.empty()) {
            cout << "Database is empty.\n";
            return;
        }
        for (const auto& source : sources) {
            source->display();
        }
    }

    void deleteSource(int index) {
        if (index < 0 || index >= sources.size()) {
            cout << "Invalid index.\n";
            return;
        }
        delete sources[index];
        sources.erase(sources.begin() + index);
        cout << "Source deleted.\n";
    }

    void sortSources() {
        sort(sources.begin(), sources.end(), [](Source* a, Source* b) {
            if (a->getType() == b->getType()) {
                return a->getTitle() < b->getTitle();
            }
            return a->getType() < b->getType();
            });
        cout << "Sources sorted.\n";
    }

    void searchBook(const string& author, const string& title) const {
        for (const auto& source : sources) {
            if (source->getType() == SourceType::BOOK && source->getAuthor() == author && source->getTitle() == title) {
                source->display();
                return;
            }
        }
        cout << "Book not found.\n";
    }

    void searchMagazineByYear(int year) const {
        for (const auto& source : sources) {
            if (source->getType() == SourceType::MAGAZINE && source->getYear() == year) {
                source->display();
                return;
            }
        }
        cout << "Magazine not found.\n";
    }

    void filterBooksByAuthor(const string& author) const {
        for (const auto& source : sources) {
            if (source->getType() == SourceType::BOOK && source->getAuthor() == author) {
                source->display();
            }
        }
    }

    void removeNewspapersByYear(int year) {
        sources.erase(remove_if(sources.begin(), sources.end(), [&](Source* source) {
            if (source->getType() == SourceType::NEWSPAPER && source->getYear() == year) {
                delete source;
                return true;
            }
            return false;
            }), sources.end());
        cout << "Newspapers from " << year << " year deleted.\n";
    }
};

int main() {
    Library library;
    int choice;

    while (true) {
        cout << "\nMenu:\n"
            << "1. Fill database\n"
            << "2. View all sources\n"
            << "3. Add source\n"
            << "4. Delete source\n"
            << "5. Sort sources\n"
            << "6. Search book\n"
            << "7. Search magazine by year\n"
            << "8. Filter books by author\n"
            << "9. Remove newspapers by year\n"
            << "0. Exit\n"
            << "Your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: library.fillDatabase(); break;
        case 2: library.displayAll(); break;
        case 3: library.addSource(); break;
        case 4: {
            int index;
            cout << "Enter index to delete: ";
            cin >> index;
            library.deleteSource(index);
            break;
        }
        case 5: library.sortSources(); break;
        case 6: {
            string author, title;
            cin.ignore();
            cout << "Enter author: ";
            getline(cin, author);
            cout << "Enter title: ";
            getline(cin, title);
            library.searchBook(author, title);
            break;
        }
        case 7: {
            int year;
            cout << "Enter year: ";
            cin >> year;
            library.searchMagazineByYear(year);
            break;
        }
        case 8: {
            string author;
            cin.ignore();
            cout << "Enter author: ";
            getline(cin, author);
            library.filterBooksByAuthor(author);
            break;
        }
        case 9: {
            int year;
            cout << "Enter year: ";
            cin >> year;
            library.removeNewspapersByYear(year);
            break;
        }
        case 0: return 0;
        default: cout << "Invalid choice.\n";
        }
    }
}
