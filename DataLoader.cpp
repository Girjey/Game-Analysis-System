#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <limits>
#include "header files/Item.h"
#include "header files/Enemy.h"
#include "header files/Hero.h"

using json = nlohmann::json;

/**
 * Загружает предметы из JSON-файла.
 * Если файл не найден или повреждён — возвращает пустой вектор.
 */
std::vector<Item> loadItems(const std::string& filepath) {
    std::vector<Item> items;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << filepath << std::endl;
        return items;
    }

    json data = json::parse(file, nullptr, false);
    if (data.is_discarded()) {
        std::cerr << "Ошибка парсинга JSON" << std::endl;
        return items;
    }

    // Проходим по каждому элементу в массиве "items" и создаём объект Item
    for (const auto& item_json : data["items"]) {
        Item item(
            item_json.value("name", ""),
            item_json.value("type", ""),
            item_json.value("physical_damage", 0.0),
            item_json.value("sharpness", 1.0),
            item_json.value("magic_damage", 0.0),
            item_json.value("magic_amplification", 1.0),
            item_json.value("crit_damage", 2.0),
            item_json.value("crit_chance", 0.05),
            item_json.value("attack_speed", 1.0),
            item_json.value("defense", 0.0),
            item_json.value("magic_resist", 0.0),
            item_json.value("weight", 0.0),
            item_json.value("durability", 100.0),
            item_json.value("stamina_cost", 1.0)
        );
        items.push_back(item);
    }

    return items;
}

/**
 * Выводит список всех доступных предметов в консоль.
 */
void listItems(const std::vector<Item>& items) {
    std::cout << "\n=== СПИСОК ДОСТУПНОГО ОРУЖИЯ ===" << std::endl;
    std::cout << "Количество ОРУЖИЯ - "<< items.size() << std::endl;

    // Выводим каждый предмет с индексом и характеристиками
    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << i + 1 << ": " << items[i].getName() << std::endl
        << "[TYPE: " << items[i].getType() << "]" << std::endl
        << "[PHYS DMG: " << items[i].getPhysicalDamage() << "]" << std::endl
        << "[SHARPNESS: " << items[i].getSharpness() << "]" << std::endl
        << "[MAG DMG: " << items[i].getMagicDamage() << "]" << std::endl
        << "[MAG AMPL: " << items[i].getMagicAmplification() << "]" << std::endl
        << "[CRIT DMG: " << items[i].getCritDamage() << "]" << std::endl
        << "[CRIT CHC: " << items[i].getCritChance() << "]" << std::endl
        << "[ATK SPD: " << items[i].getAttackSpeed() << "]" << std::endl
        << "[DEF: " << items[i].getDef() << "]" << std::endl
        << "[MAG RES: " << items[i].getMagicRest() << "]" << std::endl
        << "[WEIGHT: " << items[i].getWeight() << "]" << std::endl
        << "[DURAB: " << items[i].getDurability() << "]" << std::endl
        << "[STAM COST: " << items[i].getStaminaCost() << "]" << std::endl
        << "====================" << std::endl;
    }

}

/**
 * Загружает врагов из JSON-файла.
 * Если файл не найден или повреждён — возвращает пустой вектор.
 */
std::vector<Enemy> loadEnemies(const std::string& filepath) {
    std::vector<Enemy> enemies;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << filepath << std::endl;
        return enemies;
    }

    json data = json::parse(file, nullptr, false);
    if (data.is_discarded()) {
        std::cerr << "Ошибка парсинга JSON" << std::endl;
        return enemies;
    }

    // Проходим по каждому элементу в массиве "enemies" и создаём объект Enemy
    for (const auto& enemy_json : data["enemies"]) {
        Enemy enemy(
            enemy_json.value("name", ""),
            enemy_json.value("hp", 100.0),
            enemy_json.value("physical_damage", 50.0),
            enemy_json.value("magic_damage", 0.0),
            enemy_json.value("armor", 0.0),
            enemy_json.value("magic_resist", 0.0),
            enemy_json.value("crit_damage", 2.0),
            enemy_json.value("crit_chance", 0.0),
            enemy_json.value("accuracy", 100.0),
            enemy_json.value("evasion", 0.0),
            enemy_json.value("stamina", 100.0),
            enemy_json.value("max_stamina", 100.0),
            enemy_json.value("stamina_cost", 5.0)
        );
        enemies.push_back(enemy);
    }

    return enemies;
}

/**
 * Выводит список всех доступных врагов в консоль.
 */
void listEnemies(const std::vector<Enemy>& enemies) {
    std::cout << "\n=== СПИСОК ДОСТУПНЫХ ВРАГОВ ===" << std::endl;
    std::cout << "Количество ВРАГОВ - "<< enemies.size() << std::endl;

    // Выводим каждого врага с индексом и характеристиками
    for (size_t i = 0; i < enemies.size(); ++i) {
        std::cout << i + 1 << ": " << enemies[i].getEnemyName() << std::endl
        << "[HP: " << enemies[i].getHp() << "]" << std::endl
        << "[PHYS DMG: " << enemies[i].getPhysicalDamage() << "]" << std::endl
        << "[MAG DMG: " << enemies[i].getMagicDamage() << "]" << std::endl
        << "[ARMOR: " << enemies[i].getArmor() << "]" << std::endl
        << "[MAG RES: " << enemies[i].getMagicResist() << "]" << std::endl
        << "[CRIT DMG: " << enemies[i].getCritDamage() << "]" << std::endl
        << "[CRIT CHC: " << enemies[i].getCritChance() << "]" << std::endl
        << "[ACCUR: " << enemies[i].getAccuracy() << "]" << std::endl
        << "[EVAS: " << enemies[i].getEvasion() << "]" << std::endl
        << "[STAM: " << enemies[i].getStamina() << "]" << std::endl
        << "[MAX STAM: " << enemies[i].getMaxStamina() << "]" << std::endl
        << "[STAM COST: " << enemies[i].getStaminaCost() << "]" << std::endl
        << "====================" << std::endl;
    }

}

/**
 * Загружает героев из JSON-файла.
 * Если файл не найден или повреждён — возвращает пустой вектор.
 */
std::vector<Hero> loadHeroes(const std::string& filepath) {
    std::vector<Hero> heroes;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << filepath << std::endl;
        return heroes;
    }

    json data = json::parse(file, nullptr, false);
    if (data.is_discarded()) {
        std::cerr << "Ошибка парсинга JSON" << std::endl;
        return heroes;
    }

    // Проходим по каждому элементу в массиве "heroes" и создаём объект Hero
    for (const auto& hero_json : data["heroes"]) {
        Hero hero(
            hero_json.value("name", ""),
            hero_json.value("hp", 100.0),
            hero_json.value("physical_damage", 40.0),
            hero_json.value("magic_damage", 20.0),
            hero_json.value("crit_damage", 2.0),
            hero_json.value("crit_chance", 0.15),
            hero_json.value("defense", 15.0),
            hero_json.value("magic_resist", 0.1),
            hero_json.value("accuracy", 100.0),
            hero_json.value("evasion", 10.0),
            hero_json.value("stamina", 100.0),
            hero_json.value("max_stamina", 100.0)
        );
        heroes.push_back(hero);
    }

    return heroes;
}

/**
 * Выводит список всех доступных героев в консоль.
 */
void listHeroes(const std::vector<Hero>& heroes) {
    std::cout << "\n=== СПИСОК ДОСТУПНЫХ ГЕРОЕВ ===" << std::endl;
    std::cout << "Количество ГЕРОЕВ - " << heroes.size() << std::endl;

    // Выводим каждого героя с индексом и характеристиками
    for (size_t i = 0; i < heroes.size(); ++i) {
        std::cout << i + 1 << ": " << heroes[i].get_name() << std::endl
        << "[HP: " << heroes[i].get_base_hp() << "]" << std::endl
        << "[PHYS DMG: " << heroes[i].get_base_physical_damage() << "]" << std::endl
        << "[MAG DMG: " << heroes[i].get_base_magic_damage() << "]" << std::endl
        << "[DEF: " << heroes[i].get_base_defence() << "]" << std::endl
        << "[MAG RES: " << heroes[i].get_base_magic_resist() << "]" << std::endl
        << "[CRIT DMG: " << heroes[i].get_base_crit_damage() << "]" << std::endl
        << "[CRIT CHC: " << heroes[i].get_base_crit_chance() << "]" << std::endl
        << "[ACCUR: " << heroes[i].get_base_accuracy() << "]" << std::endl
        << "[EVAS: " << heroes[i].get_base_evasion() << "]" << std::endl
        << "[STAM: " << heroes[i].get_base_stamina() << "]" << std::endl
        << "[MAX STAM: " << heroes[i].get_base_max_stamina() << "]" << std::endl
        << "====================" << std::endl;
    }

}

/**
 * Вспомогательная функция для безопасного ввода числа.
 * Если пользователь ввёл не число — повторяет запрос.
 */
template<typename T>
T safeInput(const std::string& prompt) {
    T value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        // Проверяем, что ввод успешен и нет мусора в потоке
        if (std::cin.good() && std::cin.peek() == '\n') {
            return value;
        }
        // Очищаем состояние ошибки и пропускаем некорректный ввод
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Некорректный ввод. Попробуйте снова." << std::endl;
    }
}

/**
 * Создаёт нового героя через ввод характеристик пользователем
 * и сохраняет его в JSON-файл. Возвращает обновлённый вектор героев.
 * @param filepath Путь к JSON-файлу с героями
 * @return Вектор всех героев (включая нового)
 */
std::vector<Hero> createAndSaveHero(const std::string& filepath) {
    std::cout << "\n=== СОЗДАНИЕ НОВОГО ГЕРОЯ ===" << std::endl;

    // Загружаем существующих героев из файла
    std::vector<Hero> heroes = loadHeroes(filepath);

    // Считываем все характеристики у пользователя
    std::string name;
    std::cout << "Введите имя героя: ";
    std::cin >> name;

    double hp = safeInput<double>("Введите HP (здоровье): ");
    double phys_dmg = safeInput<double>("Введите физ. урон: ");
    double magic_dmg = safeInput<double>("Введите маг. урон: ");
    double defense = safeInput<double>("Введите защиту: ");
    double magic_resist = safeInput<double>("Введите маг. сопротивление (0-1): ");
    double crit_dmg = safeInput<double>("Введите крит. множитель (например, 2.0): ");
    double crit_chance = safeInput<double>("Введите шанс крита (0-1): ");
    double accuracy = safeInput<double>("Введите точность: ");
    double evasion = safeInput<double>("Введите уклонение: ");
    double stamina = safeInput<double>("Введите выносливость: ");

    // Создаём JSON-объект для нового героя
    json newHero = {
        {"name", name},
        {"hp", hp},
        {"physical_damage", phys_dmg},
        {"magic_damage", magic_dmg},
        {"defense", defense},
        {"magic_resist", magic_resist},
        {"crit_damage", crit_dmg},
        {"crit_chance", crit_chance},
        {"accuracy", accuracy},
        {"evasion", evasion},
        {"stamina", stamina},
        {"max_stamina", stamina}
    };

    // Загружаем или создаём JSON-структуру файла
    json data;
    std::ifstream inFile(filepath);
    if (inFile.is_open()) {
        // Проверяем размер файла перед парсингом
        inFile.seekg(0, std::ios::end);
        std::streamsize fileSize = inFile.tellg();
        inFile.seekg(0, std::ios::beg);

        if (fileSize > 0) {
            try {
                inFile >> data;
                // Убеждаемся, что есть массив "heroes"
                if (!data.contains("heroes") || !data["heroes"].is_array()) {
                    data = json::object();
                    data["heroes"] = json::array();
                }
            }
            catch (const json::parse_error& e) {
                std::cerr << "Файл повреждён, создаём новую структуру" << std::endl;
                data = json::object();
                data["heroes"] = json::array();
            }
        }
        else {
            data = json::object();
            data["heroes"] = json::array();
        }
        inFile.close();
    }
    else {
        std::cerr << "Файл не найден, создаём новый" << std::endl;
        data = json::object();
        data["heroes"] = json::array();
    }

    // Добавляем нового героя и сохраняем в файл
    data["heroes"].push_back(newHero);

    std::ofstream outFile(filepath);
    outFile << data.dump(4);
    outFile.close();

    std::cout << "\nГерой '" << name << "' успешно сохранён в " << filepath << std::endl;

    // Перезагружаем всех героев из обновлённого файла
    return loadHeroes(filepath);
}

std::vector<Item> createAndSaveItem(const std::string& filepath) {

    std::cout << "\n=== СОЗДАНИЕ НОВОГО ПРЕДМЕТА ===" << std::endl;

    std::vector<Item> items = loadItems(filepath);

    std::string name;
    std::cout << "Введите название предмета: ";
    std::cin >> name;

    std::string type;
    std::cout << "Введите тип предмета (weapon/armor): ";
    std::cin >> type;

    double physical_damage = safeInput<double>("Введите физ. урон предмета: ");
    double sharpness = safeInput<double>("Введите остроту (1.0 = норма): ");
    double magic_damage = safeInput<double>("Введите маг. урон предмета: ");
    double magic_amplification = safeInput<double>("Введите коэф. усиления маг. урона (1.0 = норма): ");
    double crit_damage = safeInput<double>("Введите крит. множитель (2.0 = x2): ");
    double crit_chance = safeInput<double>("Введите шанс крита (0-1): ");
    double attack_speed = safeInput<double>("Введите скорость атаки (1.0 = норма): ");
    double defense = safeInput<double>("Введите показатель защиты: ");
    double magic_resist = safeInput<double>("Введите маг. сопротивление (0-1): ");
    double weight = safeInput<double>("Введите вес предмета: ");
    double durability = safeInput<double>("Введите прочность (0-100): ");
    double stamina_cost = safeInput<double>("Введите затраты выносливости: ");

    // Создаём JSON-объект для нового предмета
    json newItem = {
        {"name", name},
        {"type", type},
        {"physical_damage", physical_damage},
        {"sharpness", sharpness},
        {"magic_damage", magic_damage},
        {"magic_amplification", magic_amplification},
        {"crit_damage", crit_damage},
        {"crit_chance", crit_chance},
        {"attack_speed", attack_speed},
        {"defense", defense},
        {"magic_resist", magic_resist},
        {"weight", weight},
        {"durability", durability},
        {"stamina_cost", stamina_cost}
    };

    // Загружаем или создаём JSON-структуру файла
    json data;
    std::ifstream inFile(filepath);
    if (inFile.is_open()) {
        // Проверяем размер файла перед парсингом
        inFile.seekg(0, std::ios::end);
        std::streamsize fileSize = inFile.tellg();
        inFile.seekg(0, std::ios::beg);

        if (fileSize > 0) {
            try {
                inFile >> data;
                // Убеждаемся, что есть массив "items"
                if (!data.contains("items") || !data["items"].is_array()) {
                    data = json::object();
                    data["items"] = json::array();
                }
            }
            catch (const json::parse_error& e) {
                std::cerr << "Файл повреждён, создаём новую структуру" << std::endl;
                data = json::object();
                data["items"] = json::array();
            }
        }
        else {
            data = json::object();
            data["items"] = json::array();
        }
        inFile.close();
    }
    else {
        std::cerr << "Файл не найден, создаём новый" << std::endl;
        data = json::object();
        data["items"] = json::array();
    }

    // Добавляем новый предмет и сохраняем в файл
    data["items"].push_back(newItem);

    std::ofstream outFile(filepath);
    outFile << data.dump(4);
    outFile.close();

    std::cout << "\nПредмет '" << name << "' успешно сохранён в " << filepath << std::endl;

    // Перезагружаем все предметы из обновлённого файла
    return loadItems(filepath);
}

std::vector<Enemy> createAndSaveEnemy(const std::string& filepath) {

    std::cout << "\n=== СОЗДАНИЕ НОВОГО ВРАГА ===" << std::endl;

    std::vector<Enemy> enemies = loadEnemies(filepath);

    std::string name;
    std::cout << "Введите имя врага: ";
    std::cin >> name;

    double hp = safeInput<double>("Введите HP (здоровье): ");
    double physical_damage = safeInput<double>("Введите физ. урон: ");
    double magic_damage = safeInput<double>("Введите маг. урон: ");
    double armor = safeInput<double>("Введите броню: ");
    double magic_resist = safeInput<double>("Введите маг. сопротивление (0-1): ");
    double crit_damage = safeInput<double>("Введите крит. множитель (например, 2.0): ");
    double crit_chance = safeInput<double>("Введите шанс крита (0-1): ");
    double accuracy = safeInput<double>("Введите точность: ");
    double evasion = safeInput<double>("Введите уклонение: ");
    double stamina = safeInput<double>("Введите выносливость: ");
    double max_stamina = safeInput<double>("Введите максимальную выносливость: ");
    double stamina_cost = safeInput<double>("Введите затраты выносливости за атаку: ");

    // Создаём JSON-объект для нового врага
    json newEnemy = {
        {"name", name},
        {"hp", hp},
        {"physical_damage", physical_damage},
        {"magic_damage", magic_damage},
        {"armor", armor},
        {"magic_resist", magic_resist},
        {"crit_damage", crit_damage},
        {"crit_chance", crit_chance},
        {"accuracy", accuracy},
        {"evasion", evasion},
        {"stamina", stamina},
        {"max_stamina", max_stamina},
        {"stamina_cost", stamina_cost}
    };

    // Загружаем или создаём JSON-структуру файла
    json data;
    std::ifstream inFile(filepath);
    if (inFile.is_open()) {
        // Проверяем размер файла перед парсингом
        inFile.seekg(0, std::ios::end);
        std::streamsize fileSize = inFile.tellg();
        inFile.seekg(0, std::ios::beg);

        if (fileSize > 0) {
            try {
                inFile >> data;
                // Убеждаемся, что есть массив "enemies"
                if (!data.contains("enemies") || !data["enemies"].is_array()) {
                    data = json::object();
                    data["enemies"] = json::array();
                }
            }
            catch (const json::parse_error& e) {
                std::cerr << "Файл повреждён, создаём новую структуру" << std::endl;
                data = json::object();
                data["enemies"] = json::array();
            }
        }
        else {
            data = json::object();
            data["enemies"] = json::array();
        }
        inFile.close();
    }
    else {
        std::cerr << "Файл не найден, создаём новый" << std::endl;
        data = json::object();
        data["enemies"] = json::array();
    }

    // Добавляем нового врага и сохраняем в файл
    data["enemies"].push_back(newEnemy);

    std::ofstream outFile(filepath);
    outFile << data.dump(4);
    outFile.close();

    std::cout << "\nВраг '" << name << "' успешно сохранён в " << filepath << std::endl;

    // Перезагружаем всех врагов из обновлённого файла
    return loadEnemies(filepath);
}

