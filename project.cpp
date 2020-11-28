#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <iomanip>

using std::cout, std::cin, std::endl, std::fixed;
using std::string, std::vector;
using std::map, std::pair, std::make_pair;
using std::stoi, std::stod;
using std::setprecision;

/* Created for use within ShoppingList structure. Creates effective data storage
 * tool for use within vector for the applications of this program.*/
struct ListInfo {
    int quantity = 0;
    string name;
    ListInfo(int quantity1, string name1) {
        quantity = quantity1;
        name = name1;
    }
};

struct ShoppingList {
    vector<ListInfo> list_items;
};

/* Returns a ShoppingList object, takes a reference to a ShoppingList and a
 * constant reference to a string representing a line of input.*/
ShoppingList MakeList(ShoppingList& list, const string& line) {
    string::size_type index_after_int;
    int quantity = stoi(line, &index_after_int);
    string item_name =
        line.substr(index_after_int + 1);  /* + 1 is to account for space between
                                              number and item name in the line */
    list.list_items.push_back({ quantity, item_name });
    return list;
}

/* Struct created for use in Store struct's data member 'items'. Most effective
 * data storage method.*/
struct ItemInfo {
    string name;
    int quantity = 0;
    double price = 0.0;
    ItemInfo(string name1, int quantity1, double price1) {
        name = name1;
        quantity = quantity1;
        price = price1;
    };
};

/* Creates Store struct with data members for all important*/
struct Store {
    string name;
    string city;
    vector<ItemInfo> items;
    Store(string n) { name = n; };
};

/* Returns void, takes a constant reference to a string representing a line, and
 * a constant reference to a vector containing type Store. Effectively creates
 * each stores item inventory.*/
void CreateStoreInventory(const string& line, vector<Store>& stores) {
    size_t first_comma = line.find(',');
    size_t second_comma = line.rfind(',');
    string item_name = line.substr(0, first_comma);
    int quantity =
        stoi(line.substr(first_comma + 1, second_comma - first_comma - 1));
    double price = stod(line.substr(second_comma + 2));
    stores[stores.size() - 1].items.push_back({ item_name, quantity, price });
}

/* Gets the first line of the input, returns a string of it. Main purpose is
 * removing first line from further iteration. */
string GetFirstLine() {
    string first_line;
    getline(cin, first_line);
    return first_line;
}

/* Returns void, takes in a constant reference to a Store. Prints store summary
 * for a specific store.*/
void PrintStoreSummary(const Store& store) {
    cout << store.name << " has " << store.items.size() << " distinct items."
        << endl;
}

/* Returns void, takes a constant reference to a vector containing type Store.
 * Prints Store information.*/
void PrintStoresSummary(const vector<Store>& s) {
    cout << "Store Related Information (ordered by in-file order):" << endl;
    cout << "There are " << s.size() << " store(s)." << endl;
    for (auto store : s) {
        PrintStoreSummary(store);
    }
    cout << endl;
}

/* Returns a mapping of strings to integers. Takes a constant reference to a
 * vector containing type Store. Maps each item's name and quantity from each
 * stores inventory.*/
map<string, int> GetItemCount(const vector<Store>& s) {
    map<string, int> distinct_items;
    for (auto store : s) {
        for (auto item : store.items) {
            distinct_items[item.name] += item.quantity;
        }
    }
    return distinct_items;
}

/* Returns void, takes a constant reference to map of strings mapped to
 * integers. Used to print item information.*/
void PrintItemSummary(const map<string, int>& distinct_items) {
    cout << "Item Related Information (ordered alphabetically):" << endl;
    cout << "There are " << distinct_items.size()
        << " distinct item(s) available for purchase." << endl;
    for (auto [item, count] : distinct_items) {
        cout << "There are " << count << " " << item << "(s)." << endl;
    }
    cout << endl;
}

/* Returns an integer representing a counter of the # of stores that sell a
 * particular item. Takes a constant vector of type store, a constant reference
 * to a string, and a reference to a vector containing type Store that is
 * updated in the function to house the store objects that contain the item.*/
int GetStoresThatSellIt(const vector<Store>& s, const string& item_name,
    vector<Store>& sellers) {
    int counter = 0;
    for (auto store : s) {
        for (auto item : store.items) {
            if (item_name == item.name) {
                sellers.push_back(store);
                ++counter;
            }
        }
    }
    return counter;
}

/* Returns void, takes a constant reference to a vector containing type Store
 * and a constant reference to an object of type ShoppingList, returning void.
 */
void PrintShoppingSummary(const vector<Store>& s, const ShoppingList& l,
    size_t i, vector<Store>& sellers) {
    cout << "Trying to order " << l.list_items[i].quantity << " "
        << l.list_items[i].name << "(s)." << endl;
    int number_of_stores = GetStoresThatSellIt(s, l.list_items[i].name, sellers);
    cout << number_of_stores << " store(s) sell " << l.list_items[i].name << "."
        << endl;
}

/* Returns a map of doubles mapped to integers. Takes a constant reference to a
 * vector containing type Store, and a constant reference to a string
 * representing an item's name.*/
map<double, int> GetCheapestPrice(const vector<Store>& sellers,
    const string& item_name) {
    map<double, int> price_quantity;
    for (size_t i = 0; i < sellers.size(); ++i) {
        for (size_t j = 0; j < sellers[i].items.size(); ++j) {
            if (sellers[i].items[j].name == item_name) {
                price_quantity[sellers[i].items[j].price] =
                    sellers[i].items[j].quantity;
            }
        }
    }
    return price_quantity;
}

/* Takes a constant reference to a vector containing type Store and a constant
 * reference to a string representing an item's name. Returns void, used to sort
 * the vector. */
void SortByPrice(vector<Store>& sellers, const string& list_item_name) {
    std::sort(sellers.begin(), sellers.end(),
        [list_item_name](Store s1, Store s2) {
            auto it1 = std::find_if(s1.items.begin(), s1.items.end(),
                [list_item_name](ItemInfo item) {
                    return item.name == list_item_name;
                });
            auto it2 = std::find_if(s2.items.begin(), s2.items.end(),
                [list_item_name](ItemInfo item) {
                    return item.name == list_item_name;
                });
            return (*it1).price < (*it2).price;
        });
}

/* Takes a constant reference to a vector containing type Store. Returns void.
 * Used to print information.*/
void PrintShoppingInfo(const vector<Store>& sellers,
    const vector<int>& quantities) {
    size_t j = 0;
    for (size_t i = 0; i < quantities.size(); ++i) {
        cout << "Order " << quantities[i] << " from " << sellers[j].name << " in "
            << sellers[j].city << endl;
        j += 2;
    }
    cout << endl;
}

/* Takes a reference to a vector of integers, a constant reference to a map of
 * doubles to ints, and a reference to an integer. Returns a double. The vector
 * is manipulated specifically with intent for use outside of this function, and
 * thus needs to be a reference.*/
double GetTotal(vector<int>& quantities,
    const map<double, int>& cheapest_prices,
    int& desired_quantity) {
    double total = 0;
    for (auto [price, quantity] : cheapest_prices) {
        if (desired_quantity - quantity >= 0) {
            desired_quantity = desired_quantity - quantity;
            total += quantity * price;
            quantities.push_back(quantity);
            if (desired_quantity == 0) {
                break;
            }
        }
        else {
            total += desired_quantity * price;
            quantities.push_back(desired_quantity);
            break;
        }
    }
    return total;
}

/* Takes a constant reference to a vector containing type Store and a constant
 * reference to an object of type ShoppingList, returning void. */
void CreateShoppingSummary(const vector<Store>& s, const ShoppingList& l) {
    double order_total;
    for (size_t i = 0; i < l.list_items.size(); ++i) {
        vector<Store> sellers;
        string list_item_name = l.list_items[i].name;
        int desired_quantity = l.list_items[i].quantity;
        GetStoresThatSellIt(s, list_item_name, sellers);
        PrintShoppingSummary(s, l, i, sellers);
        SortByPrice(sellers, list_item_name);
        map<double, int> cheapest_prices =
            GetCheapestPrice(sellers, list_item_name);
        vector<int> quantities;
        double total = GetTotal(quantities, cheapest_prices, desired_quantity);
        order_total += total;
        cout << setprecision(2) << fixed << "Total price: $" << total << endl;
        PrintShoppingInfo(sellers, quantities);
    }
    cout << setprecision(2) << fixed << "Be sure to bring $" << order_total
        << " when you leave for the stores." << endl;
}

/* Returns void, takes a reference to a vector containing type Store and a
 * reference to an object of type ShoppingList which both are empty. Updates the
 * vector and the object's data members based on which line of input is being
 * read.*/
void CreateStoresAndLists(vector<Store>& stores, ShoppingList& list) {
    int counter = 1;
    string line;
    while (getline(cin, line)) {
        if (line.empty()) {  /* resets counter to 0 when empty line is encountered,
                               prepares loop for next store's info */
            counter = 0;
        }
        else if (line.find("shopping list") !=
            string::npos) {  // finds line that is list related
            counter = 43;  /* on next iteration each list item will be added to the
                              list in line 238 */
        }
        else if (counter > 43) {
            list = MakeList(list, line);
        }
        else {  /* creates Store objects and assigns data members values based on
                  counter value / which line from input it is */
            if (counter == 1) {
                stores.push_back(Store(line));
            }
            else if (counter == 2) {
                stores[stores.size() - 1].city = line;
            }
            else {
                CreateStoreInventory(line, stores);
            }
        }
        counter++;
    }
}

int main() {
    vector<Store> stores;
    ShoppingList list;
    string first_line = GetFirstLine();
    CreateStoresAndLists(stores, list);
    PrintStoresSummary(stores);
    map<string, int> distinct_items = GetItemCount(stores);
    PrintItemSummary(distinct_items);
    cout << "Shopping:" << endl;
    CreateShoppingSummary(stores, list);

    return 0;
}