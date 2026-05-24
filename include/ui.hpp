#ifndef UI_HPP
#define UI_HPP

#include <string>
#include <vector>
#include "restaurant.hpp"
#include "table.hpp"

enum class MsgType
{
    SUCCESS,
    ERROR,
    WARNING,
    INFO,
    NONE
};

class UI
{
private:
    Restaurant &restaurant; // Referencia a központi modellre
    std::string flashMessage;
    MsgType flashType;

    // ANSI Színkódok
    static constexpr const char *COLOR_SUCCESS = "\x1b[32m";
    static constexpr const char *COLOR_ERROR = "\x1b[31m";
    static constexpr const char *COLOR_WARNING = "\x1b[33m";
    static constexpr const char *COLOR_INFO = "\x1b[34m";
    static constexpr const char *COLOR_RESET = "\x1b[0m";
    static constexpr int HEADER_LENGTH = 50;

    int getIntInput(const std::string &prompt);
    double getDoubleInput(const std::string &prompt);
    std::string getStringInput(const std::string &prompt);

    // Helper input validators
    std::string askValidatedName(const std::string &prompt);
    int askValidatedPrice(const std::string &prompt);
    double askValidatedVolume(const std::string &prompt);
    bool askYesNo(const std::string &prompt);

    void printFlashMessage();
    void consoleClear();

    // Asztal menedzsment
    void tableMenu();
    void addTableMenu();
    void modifyTableMenu();
    void deleteTableMenu();

    // Asztal menedzsment helper függvények
    int getValidatedCoordinate(const std::string &axis, int maxValue);
    int getValidatedSeats(int minSeats);
    Table *selectTableById();
    std::pair<int, int> getValidatedTablePosition(int excludeTableId = -1);

    // Étlap menedzsment
    void menuMenu();
    void viewMenuMenu();
    void addMenuItemMenu();
    void modifyMenuItemMenu();
    void deleteMenuItemMenu();

    MenuItem *selectMenuItemById();

    // Rendelés menedzsment
    void orderMenu();

    static List<std::string> wordWrap(const std::string &text, int maxWidth);

public:
    UI(Restaurant &r);

    void run();

    static void printMsgBox(const std::string &msg, MsgType type = MsgType::NONE);
    static void pause();
};

#endif // UI_HPP