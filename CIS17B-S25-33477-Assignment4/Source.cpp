#include <iostream>
#include <unordered_map>
#include <map>
#include <memory>
#include <stdexcept>

// Exceptions
class DuplicateItemException : public std::runtime_error {
public:
    DuplicateItemException(const std::string& msg) : std::runtime_error(msg) {}
};

class ItemNotFoundException : public std::runtime_error {
public:
    ItemNotFoundException(const std::string& msg) : std::runtime_error(msg) {}
};

// StoredItem class
class StoredItem {
private:
    std::string id;
    std::string description;
    std::string location;

public:
    StoredItem(std::string id, std::string desc, std::string loc)
        : id(id), description(desc), location(loc) {}

    std::string getId() const { return id; }
    std::string getDescription() const { return description; }
    std::string getLocation() const { return location; }
};

// StorageManager class
class StorageManager {
private:
    // unordered 
    std::unordered_map<std::string, std::shared_ptr<StoredItem>> itemById;
    // ordered alphabetical
    std::map<std::string, std::shared_ptr<StoredItem>> itemByDescription;

public:
    // adds item to maps by id and throws exception if id already exists
    void addItem(const std::shared_ptr<StoredItem>& item) {
        if (itemById.count(item->getId()) > 0) {
            throw DuplicateItemException("Item with ID " + item->getId() + " already exists.");
        }
        // after check adds to both maps
        itemById[item->getId()] = item;
        itemByDescription[item->getDescription()] = item;
    }
    // finds item by ID and throws if doesn't exist
    std::shared_ptr<StoredItem> findById(const std::string& id) const {
        auto it = itemById.find(id);
        if (it == itemById.end()) {
            throw ItemNotFoundException("Item with ID " + id + " not found.");
        }
        return it->second;
    }
    //removes item from maps and throws if doesn't exist
    void removeItem(const std::string& id) {
        auto it = itemById.find(id);
        if (it == itemById.end()) {
            throw ItemNotFoundException("Item with ID " + id + " not found.");
        }

        std::string des = it->second->getDescription();
        itemById.erase(it);
        itemByDescription.erase(des);
    }
    //prints the sortered items
    void listItemsByDescription() const {
        std::cout << "Items in Description Order:\n";
        for (const auto& pair : itemByDescription) {
            std::cout << "- " << pair.second->getDescription()
                << ": " << pair.second->getLocation() << "\n";
        }
    }
};

//TEst Functions

//  adding duplicate item
void testDuplicateAddition(StorageManager& manager) {
    try {
        auto duplicate = std::make_shared<StoredItem>("ITEM001", "Wireless Mouse", "Aisle 4, Shelf 2");
        manager.addItem(duplicate);
    }
    catch (const DuplicateItemException& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

// trying to find/remove a non-existent item
void testItemNotFound(StorageManager& manager) {
    try {
        manager.removeItem("ITEM003");
    }
    catch (const ItemNotFoundException& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

int main() {
    StorageManager manage;

    // add items
    auto item1 = std::make_shared<StoredItem>("ITEM001", "Wireless Mouse", "Aisle 2, Shelf 3");
    auto item2 = std::make_shared<StoredItem>("ITEM002", "Airpods", "Aisle 1, Shelf 7");

    std::cout << "Adding Item ITEM001 - Wireless Mouse...\n";
    manage.addItem(item1);

    std::cout << "Adding Item ITEM002 - Bluetooth Speaker...\n";
    manage.addItem(item2);

    // test function duplicate insertion
    std::cout << "Attempting to add ITEM001 again...\n";
    testDuplicateAddition(manage);

    // find item
    try {
        std::cout << "Searching for ITEM002...\n";
        auto find = manage.findById("ITEM002");
        std::cout << "Located: " << find->getDescription() << " in " << find->getLocation() << "\n";
    }
    // catch the errors
    catch (const ItemNotFoundException& e) {
        std::cout << "Error: " << e.what() << "\n";
    }

    //test function

    // remove a missing item
    std::cout << "Attempting to delete ITEM003...\n";
    testItemNotFound(manage);

    // display all items
    std::cout << "\nInventory sorted by description:\n";
    manage.listItemsByDescription();

    return 0;
}
