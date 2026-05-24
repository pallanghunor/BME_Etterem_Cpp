#ifndef UI_HPP
#define UI_HPP

/**
 * @file ui.hpp
 * @brief Konzolos felhasználói felület deklarációja.
 */

#include <string>
#include "restaurant.hpp"
#include "table.hpp"

/**
 * @brief A UI üzenetdobozának típusai a színezéshez.
 */
enum class MsgType
{
    SUCCESS,
    ERROR,
    WARNING,
    INFO,
    NONE
};

/**
 * @brief Konzolos felület az étterem kezelésére.
 */
class UI
{
private:
    /**
     * @brief Hivatkozás a központi étteremmodellre.
     */
    Restaurant &restaurant;
    std::string flashMessage;
    MsgType flashType;

    /**
     * @brief A konzolos UI által használt ANSI színkódok.
     */
    static constexpr const char *COLOR_SUCCESS = "\x1b[32m";
    static constexpr const char *COLOR_ERROR = "\x1b[31m";
    static constexpr const char *COLOR_WARNING = "\x1b[33m";
    static constexpr const char *COLOR_INFO = "\x1b[34m";
    static constexpr const char *COLOR_RESET = "\x1b[0m";
    static constexpr int HEADER_LENGTH = 50;

    /**
     * @brief Érvényesített egész szám beolvasása.
     */
    int getIntInput(const std::string &prompt);

    /**
     * @brief Érvényesített valós szám beolvasása.
     */
    double getDoubleInput(const std::string &prompt);

    /**
     * @brief Teljes sor beolvasása.
     */
    std::string getStringInput(const std::string &prompt);

    /**
     * @brief Nem üres nevet olvas be és ellenőriz.
     */
    std::string askValidatedName(const std::string &prompt);

    /**
     * @brief Nem negatív árat olvas be és ellenőriz.
     */
    int askValidatedPrice(const std::string &prompt);

    /**
     * @brief Pozitív űrtartalmat olvas be és ellenőriz.
     */
    double askValidatedVolume(const std::string &prompt);

    /**
     * @brief Igen/nem értéket olvas be.
     */
    bool askYesNo(const std::string &prompt);

    /**
     * @brief Kiírja és törli a várakozó üzenetet.
     */
    void printFlashMessage();

    /**
     * @brief Törli a konzol tartalmát.
     */
    void consoleClear();

    /**
     * @brief Megnyitja az asztalkezelő menüt.
     */
    void tableMenu();

    /**
     * @brief Új asztal felvételi folyamat indítása.
     */
    void addTableMenu();

    /**
     * @brief Asztalmódosítási folyamat indítása.
     */
    void modifyTableMenu();

    /**
     * @brief Asztaltörlési folyamat indítása.
     */
    void deleteTableMenu();

    /**
     * @brief Koordinátát olvas be és ellenőriz.
     */
    int getValidatedCoordinate(const std::string &axis, int maxValue);

    /**
     * @brief Férőhelyszámot olvas be és ellenőriz.
     */
    int getValidatedSeats(int minSeats);

    /**
     * @brief Lehetővé teszi az asztal kiválasztását azonosító alapján.
     */
    Table *selectTableById();

    /**
     * @brief Asztalpozíciót olvas be és ellenőriz.
     */
    std::pair<int, int> getValidatedTablePosition(int excludeTableId = -1);

    /**
     * @brief Megnyitja az étlapkezelő menüt.
     */
    void menuMenu();

    /**
     * @brief Megjeleníti a jelenlegi étlapot.
     */
    void viewMenuMenu();

    /**
     * @brief Új étel vagy ital felvételi folyamat indítása.
     */
    void addMenuItemMenu();

    /**
     * @brief Menü tétel módosítási folyamat indítása.
     */
    void modifyMenuItemMenu();

    /**
     * @brief Menü tétel törlési folyamat indítása.
     */
    void deleteMenuItemMenu();

    /**
     * @brief Lehetővé teszi a menü tétel kiválasztását azonosító alapján.
     */
    MenuItem *selectMenuItemById();

    /**
     * @brief Privát visitor, amely a típus alapján irányítja a szerkesztést.
     */
    class ModifyVisitor : public MenuItemVisitor
    {
        UI &ui;

    public:
        ModifyVisitor(UI &u) : ui(u) {}
        void visitFood(Food *food) override;
        void visitDrink(Drink *drink) override;
    };

    /**
     * @brief Interaktív szerkesztő étel menü tételekhoz.
     */
    void modifyFoodInteractive(Food *food);

    /**
     * @brief Interaktív szerkesztő ital menü tételekhoz.
     */
    void modifyDrinkInteractive(Drink *drink);

    /**
     * @brief Megnyitja a rendeléskezelő menüt.
     */
    void orderMenu();

    /**
     * @brief Szöveget több sorra tördel.
     */
    static List<std::string> wordWrap(const std::string &text, int maxWidth);

public:
    /**
     * @brief UI létrehozása a megadott étteremmodellhez.
     */
    UI(Restaurant &r);

    /**
     * @brief Lefuttatja a fő UI ciklust.
     */
    void run();

    /**
     * @brief Keretbe foglalt konzolos üzenetet ír ki.
     */
    static void printMsgBox(const std::string &msg, MsgType type = MsgType::NONE);

    /**
     * @brief Vár az Enter billentyű lenyomására.
     */
    static void pause();
};

#endif // UI_HPP