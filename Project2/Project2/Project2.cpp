// Project2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

constexpr auto MAX_BOOKS = 10;
constexpr auto MAX_USERS = 10;

struct User
{
    int id{ 0 };
    std::string name{};
    std::vector<int> borrowed_books{};
};

struct Book
{
    int id{ 0 };
    std::string name{};
    int quantity{ 0 };

    bool has_prefix(const std::string &prefix)
    {
        if (name.size() < prefix.size())
        {
            return false;
        }

        for (int i = 0; i < prefix.size(); i++)
        {
            if (prefix.at(i) != name.at(i))
            {
                return false;
            }
        }

        return true;
    }
};

struct BorrowedBook
{
    int book_id{ 0 };
    int user_id{ 0 };
};

void read_book(Book& b)
{
    std::cout << "Please enter {id} {name} {quantity}: ";
    std::cin >> b.id >> b.name >> b.quantity;
}

void read_user(User& u)
{
    std::cout << "Please enter {name} {id}: ";
    std::cin >> u.name >> u.id;
}

struct Library
{
    std::vector<Book> books{};
    std::vector<User> users{};
    std::vector<BorrowedBook> borrowed_books{};

    int find_book_by_id(int id)
    {
        for (int i = 0; i < books.size(); i++)
        {
            if (books.at(i).id == id)
            {
                return i;
            }
        }

        return -1;
    }

    int find_book_by_name(std::string& name)
    {
        for (int i = 0; i < books.size(); i++)
        {
            if (books.at(i).name == name)
            {
                return i;
            }
        }

        return -1;
    }

    int find_user_by_id(int id)
    {
        for (int i = 0; i < users.size(); i++)
        {
            if (users.at(i).id == id)
            {
                return i;
            }
        }

        return -1;
    }

    int find_user_by_name(std::string& name)
    {
        for (int i = 0; i < users.size(); i++)
        {
            if (users.at(i).name == name)
            {
                return i;
            }
        }

        return -1;
    }

    bool is_book_checked_out(int id)
    {
        for (int i = 0; i < borrowed_books.size(); i++)
        {
            if (borrowed_books.at(i).book_id == id)
            {
                return true;
            }
        }

        return false;
    }

    void print_borrowed_books()
    {
        std::cout << "Enter book name: ";
        std::string name{};
        std::cin >> name;

        int idx = find_book_by_name(name);

        if (idx < 0)
        {
            std::cout << name << " was not found." << std::endl;
            return;
        }

        int id = books.at(idx).id;

        std::set<int> userIds;
        std::sort(borrowed_books.begin(), borrowed_books.end(), [](BorrowedBook& a, BorrowedBook& b) { return a.user_id < b.user_id; });
        for (auto& borrowed : borrowed_books)
        {
            if (borrowed.book_id != id)
            {
                continue;
            }

            auto result = userIds.insert(borrowed.user_id);
            if (result.second)
            {
                int idx = find_user_by_id(borrowed.user_id);
                auto& user = users.at(idx);
                std::cout << user.name << std::endl;
            }
        }
    }

    int number_borrowed_by_id(int id)
    {
        auto borrowed = std::count_if(borrowed_books.cbegin(), borrowed_books.cend(), [id](const BorrowedBook& b) { return b.book_id == id; });
        return (int)borrowed;
    }

    void print_by_id()
    {
        std::sort(books.begin(), books.end(), [](Book& a, Book& b) { return a.id < b.id; });

        for (const auto& b : books)
        {
            std::cout << "id: " << b.id << ", name: " << b.name << ", copies: " << b.quantity << ", borrowed: " << number_borrowed_by_id(b.id) << std::endl;
        }
    }

    void print_by_name()
    {
        std::sort(books.begin(), books.end(), [](Book& a, Book& b) { return a.name < b.name; });

        for (const auto& b : books)
        {
            std::cout << "id: " << b.id << ", name: " << b.name << ", copies: " << b.quantity << ", borrowed: " << number_borrowed_by_id(b.id) << std::endl;
        }
    }

    void print_menu()
    {
        std::cout << "Library menu:" << std::endl
            << "1) Add book" << std::endl
            << "2) Search books by prefix" << std::endl
            << "3) Print who borrowed book by name" << std::endl
            << "4) Print library by id" << std::endl
            << "5) Print library by name" << std::endl
            << "6) Add user" << std::endl
            << "7) User borrow book" << std::endl
            << "8) User return book" << std::endl
            << "9) Print users" << std::endl
            << "10) Exit" << std::endl << std::endl
            << "Enter your menu choice [1-10]: ";
    }

    void add_book()
    {
        if (books.size() == MAX_BOOKS)
        {
            std::cout << "Maximum number of books reached" << std::endl;
            return;
        }

        Book b;
        read_book(b);
        books.push_back(std::move(b));
    }

    void add_user()
    {
        if (users.size() == MAX_USERS)
        {
            std::cout << "Maximum number of users reached" << std::endl;
        }

        User u;
        read_user(u);
        users.push_back(std::move(u));
    }

    void print_users()
    {
        std::cout << users.size() << " users in the system:" << std::endl;

        for (auto& user : users)
        {
            std::cout << user.id << " " << user.name << std::endl;
        }

        std::cout << std::endl;
    }

    void search_by_prefix()
    {
        std::cout << "Please enter search prefix: ";
        std::string prefix{};
        std::cin >> prefix;

        for (int i = 0; i < books.size(); i++)
        {
            if (books.at(i).has_prefix(prefix))
            {
                std::cout << books.at(i).name << " ";
            }
        }

        std::cout << std::endl;
    }

    void borrow_book()
    {
        std::cout << "Enter user name and book name: ";
        std::string user_name;
        std::string book_name;
        std::cin >> user_name >> book_name;

        auto user_idx = find_user_by_name(user_name);
        auto book_idx = find_book_by_name(book_name);

        if (user_idx < 0)
        {
            std::cout << "User '" << user_name << "' not found." << std::endl;
            return;
        }

        if (book_idx < 0)
        {
            std::cout << "Book '" << book_name << "' not found." << std::endl;
            return;
        }

        auto& book = books.at(book_idx);
        auto borrowed = std::count_if(borrowed_books.cbegin(), borrowed_books.cend(), [book](const BorrowedBook& b) { return b.book_id == book.id; });

        if (borrowed >= book.quantity)
        {
            std::cout << "No more copies to borrow." << std::endl;
            return;
        }

        BorrowedBook bb;
        bb.book_id = books.at(book_idx).id;
        bb.user_id = users.at(user_idx).id;
        borrowed_books.push_back(std::move(bb));
    }

    void return_book()
    {
        std::cout << "Enter user name and book name: ";
        std::string user_name;
        std::string book_name;
        std::cin >> user_name >> book_name;

        auto user_idx = find_user_by_name(user_name);
        auto book_idx = find_book_by_name(book_name);

        if (user_idx < 0)
        {
            std::cout << "User '" << user_name << "' not found." << std::endl;
            return;
        }

        if (book_idx < 0)
        {
            std::cout << "Book '" << book_name << "' not found." << std::endl;
            return;
        }

        int user_id = users.at(user_idx).id;
        int book_id = books.at(book_idx).id;
        auto find_result = std::find_if(borrowed_books.cbegin(), borrowed_books.cend(), [book_id, user_id](const BorrowedBook& bb) { return bb.user_id == user_id && bb.book_id == book_id; });
        if (find_result == std::end(borrowed_books))
        {
            return;
        }

        borrowed_books.erase(find_result);
    }

    void run()
    {
        while (true)
        {
            print_menu();

            int input{ 0 };
            std::cin >> input;

            if (input < 1 || input > 10 || std::cin.fail())
            {
                std::cout << "Please enter a number 1-10." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            if (input == 10)
            {
                break;
            }

            switch (input)
            {
            case 1:
                add_book();
                break;
            case 2:
                search_by_prefix();
                break;
            case 3:
                print_borrowed_books();
                break;
            case 4:
                print_by_id();
                break;
            case 5:
                print_by_name();
                break;
            case 6:
                add_user();
                break;
            case 7:
                borrow_book();
                break;
            case 8:
                return_book();
                break;
            case 9:
                print_users();
                break;
            default:
                std::cout << "Not implemented." << std::endl;
                break;
            }
        }
    }
};

int main()
{
    Library().run();

    return 0;
}
