#include <iostream>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>
#include <iomanip>
#include <map>

// Установка локализации
void SetupLocale() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    std::wcout.imbue(std::locale("ru_RU.UTF-8"));
}

// Класс для управления позиций меню
class MenuItem {
private:
    std::wstring name; // Название блюда
    double price;      // Цена блюда

public:
    MenuItem(std::wstring n, double p) : name(n), price(p) {}

    void PrintItem() const {
        std::wcout << L"Блюдо: " << name << L" | Цена: " << std::fixed << std::setprecision(2) << price << L" руб." << std::endl;
    }

    std::wstring GetName() const { return name; }
    double GetPrice() const { return price; }
};

// Класс для управления меню
class Menu {
private:
    std::vector<MenuItem> items;

public:
    void AddItem(const MenuItem& item) {
        items.push_back(item);
    }

    void PrintMenu() const {
        std::wcout << L"--- Меню ресторана ---\n";
        for (size_t i = 0; i < items.size(); ++i) {
            std::wcout << i + 1 << L". " << items[i].GetName() << L" - " << items[i].GetPrice() << L" руб.\n";
        }
    }

    MenuItem GetItem(int index) const {
        return items[index];
    }
};

// Класс для управления складом
class Inventory {
private:
    std::map<std::wstring, int> stock;

public:
    void AddStock(const std::wstring& itemName, int quantity) {
        stock[itemName] += quantity;
    }

    bool RemoveStock(const std::wstring& itemName, int quantity) {
        if (stock.find(itemName) != stock.end() && stock[itemName] >= quantity) {
            stock[itemName] -= quantity;
            return true;
        }
        return false;
    }

    void PrintStock() const {
        for (const auto& item : stock) {
            std::wcout << item.first << L": " << item.second << L" шт." << std::endl;
        }
    }
};

// Класс для отчётов о продажах
class Reports {
private:
    std::map<std::wstring, int> salesData;

public:
    void RecordSale(const std::wstring& itemName, int quantity) {
        salesData[itemName] += quantity;
    }

    void PrintSalesReport() const {
        std::wcout << L"--- Отчёт о продажах ---\n";
        for (const auto& sale : salesData) {
            std::wcout << sale.first << L": продано " << sale.second << L" шт." << std::endl;
        }
    }

    void PrintTopSellingItem() const {
        if (salesData.empty()) {
            std::wcout << L"Нет продаж.\n";
            return;
        }

        auto topItem = *std::max_element(salesData.begin(), salesData.end(),
            [](const std::pair<std::wstring, int>& a, const std::pair<std::wstring, int>& b) {
                return a.second < b.second;
            });

        std::wcout << L"Самое популярное блюдо: " << topItem.first << L" | Количество продаж: " << topItem.second << L" шт." << std::endl;
    }
};

// Главная функция для тестирования системы
int main() {
    SetupLocale();

    // Создаём склад, меню и отчёт
    Inventory inventory;
    Reports reports;
    Menu menu;

    // Добавляем товары на склад
    inventory.AddStock(L"Борщ", 10);
    inventory.AddStock(L"Пельмени", 5);
    inventory.AddStock(L"Чай", 20);

    // Добавляем блюда в меню
    menu.AddItem(MenuItem(L"Борщ", 150.50));
    menu.AddItem(MenuItem(L"Пельмени", 200.00));
    menu.AddItem(MenuItem(L"Чай", 50.00));

    while (true) {
        std::wcout << L"Главное меню:\n";
        std::wcout << L"1. Просмотреть меню\n";
        std::wcout << L"2. Оформить заказ\n";
        std::wcout << L"3. Просмотреть отчёт о продажах\n";
        std::wcout << L"4. Выйти\n";
        int choice;
        std::wcin >> choice;

        if (choice == 1) {
            menu.PrintMenu();
        }
        else if (choice == 2) {
            int itemChoice;
            std::wcout << L"Выберите блюдо для заказа:\n";
            std::wcin >> itemChoice;

            if (itemChoice >= 1 && itemChoice <= 3) {
                auto item = menu.GetItem(itemChoice - 1);
                if (inventory.RemoveStock(item.GetName(), 1)) {
                    reports.RecordSale(item.GetName(), 1);
                    std::wcout << L"Блюдо добавлено в заказ: " << item.GetName() << L" | Цена: " << item.GetPrice() << L" руб.\n";
                }
                else {
                    std::wcout << L"Не хватает товара на складе.\n";
                }
            }
            else {
                std::wcout << L"Некорректный выбор.\n";
            }
        }
        else if (choice == 3) {
            reports.PrintSalesReport();
            reports.PrintTopSellingItem();
        }
        else if (choice == 4) {
            break;
        }
        else {
            std::wcout << L"Некорректный выбор, попробуйте снова.\n";
        }
    }

    return 0;
}
