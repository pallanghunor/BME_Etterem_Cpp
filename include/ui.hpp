#ifndef UI_HPP
#define UI_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

// 1. Üzenettípusok a színezéshez (mint a C-s kódodban)
enum class MsgType { SUCCESS, ERROR, WARNING, INFO, NONE };

class UI {
private:
    // ANSI Színkódok
    static constexpr const char* COLOR_SUCCESS = "\x1b[32m";
    static constexpr const char* COLOR_ERROR   = "\x1b[31m";
    static constexpr const char* COLOR_WARNING = "\x1b[33m";
    static constexpr const char* COLOR_INFO    = "\x1b[34m";
    static constexpr const char* COLOR_RESET   = "\x1b[0m";

    static constexpr int HEADER_LENGTH = 50;

    // Segédfüggvény az automatikus sortöréshez (Word Wrap)
    static std::vector<std::string> wordWrap(const std::string& text, int maxWidth) {
        std::vector<std::string> lines;
        std::istringstream words(text);
        std::string word;
        std::string currentLine;

        while (words >> word) {
            if (currentLine.empty()) {
                currentLine = word;
            } else if (currentLine.length() + 1 + word.length() <= (size_t)maxWidth) {
                currentLine += " " + word; // Még odafér a sorba
            } else {
                lines.push_back(currentLine); // Sor vége, új sor kezdése
                currentLine = word;
            }
        }
        if (!currentLine.empty()) {
            lines.push_back(currentLine);
        }
        return lines;
    }

public:
    // A régi printMsgBox C++-os megfelelője
    static void printMsgBox(const std::string& msg, MsgType type = MsgType::NONE) {
        if (msg.empty()) return;

        const int maxContentWidth = HEADER_LENGTH - 4; // Levonjuk a "| " és " |" szélességét

        // Szín kiválasztása
        std::string color;
        switch(type) {
            case MsgType::SUCCESS: color = COLOR_SUCCESS; break;
            case MsgType::ERROR:   color = COLOR_ERROR; break;
            case MsgType::WARNING: color = COLOR_WARNING; break;
            case MsgType::INFO:    color = COLOR_INFO; break;
            default: color = COLOR_RESET; break;
        }

        std::string border(HEADER_LENGTH, '=');

        // Felső keret kirajzolása színnel
        std::cout << "\n" << color << border << "\n";

        // Szöveg darabolása, ha túl hosszú
        std::vector<std::string> lines = wordWrap(msg, maxContentWidth);

        // Sorok középre igazítása és kiírása
        for (const std::string& line : lines) {
            int padLeft = (maxContentWidth - line.length()) / 2;
            int padRight = maxContentWidth - line.length() - padLeft;

            std::cout << "| " 
                      << std::string(padLeft, ' ') 
                      << line 
                      << std::string(padRight, ' ') 
                      << " |\n";
        }

        // Alsó keret és szín visszaállítása
        std::cout << border << COLOR_RESET << "\n";
    }

    // Enterre várás (opcionális, de jó ha van a képernyőtörlés előtt)
    static void pause() {
        std::cout << "(Nyomj ENTER-t a folytatashoz...)\n";
        std::cin.clear();
        // Ha maradt sortörés a pufferben, kidobjuk
        if (std::cin.rdbuf()->in_avail() > 0) {
            std::cin.ignore(10000, '\n'); 
        }
        std::cin.get();
    }
};

#endif // UI_HPP