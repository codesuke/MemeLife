#include "Quiz.h"
#include <random>
#include <ctime>

void QuizBank::LoadDefault() {
    questions.clear();
    // Provided quiz questions (correctIndex is 0-based)
    questions.push_back({
        "Who's most likely to say 'Don't worry, I'll fix it later' and never touch it again?",
        {"Manas", "Gautam", "Chirayush", "The ghost of unmerged commits"},
        1 // Gautam
    });
    questions.push_back({
        "Who fixes everyone's mess quietly while the rest debate memes?",
        {"Gautam", "Manas", "Chirayush", "GitHub Copilot"},
        2 // Chirayush
    });
    questions.push_back({
        "When someone suggested improving graphics, who replied : Dude come on, it's already enough?",
        {"Gautam", "Chirayush", "Manas", "Every dev running on 2 hours of sleep"},
        0 // Gautam
    });
    questions.push_back({
        "The famous meme \"Arey kehna kya chahte ho?\" comes from which Bollywood movie?",
        {"Welcome", "Phir Hera Pheri", "3 Idiots", "Housefull"},
        2 // 3 Idiots
    });
    questions.push_back({
        "What happens when Manas says, Don't worry bro, I tested it ?",
        {"Game runs perfectly at 120 FPS", "Code compiles with 0 errors", "Game crashes before main menu", "PC starts chanting 'bruh moment'"},
        2 // Game crashes before main menu
    });
}

void QuizBank::LoadFromFolder(const std::string& folderPath) {
    // File-based loading not implemented; fallback to default.
    (void)folderPath;
    if (questions.empty()) LoadDefault();
}

const QuizQuestion& QuizBank::GetRandom() const {
    static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<size_t> dist(0, questions.size() - 1);
    return questions[dist(rng)];
}
