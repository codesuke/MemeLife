#ifndef QUIZ_H
#define QUIZ_H

#include <string>
#include <vector>

struct QuizQuestion {
    std::string question;
    std::vector<std::string> options; // 2-5 options
    int correctIndex; // 0-based index
};

class QuizBank {
public:
    std::vector<QuizQuestion> questions;

    void LoadDefault(); // hardcoded fallback
    void LoadFromFolder(const std::string& folderPath); // optional file-based
    const QuizQuestion& GetRandom() const;
};

#endif // QUIZ_H
