#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QFrame>
#include <QFont>
#include <unordered_map>
#include <string>
#include <sstream>

int romanToInt(const std::string& s) {
    std::unordered_map<char, int> romanValues = {
        {'I', 1}, {'V', 5}, {'X', 10}, {'L', 50}, {'C', 100}, {'D', 500}, {'M', 1000}
    };
    int total = 0;
    int prevValue = 0;
    for (char c : s) {
        int value = romanValues[c];
        if (value > prevValue) {
            total += value - 2 * prevValue;
        }
        else {
            total += value;
        }
        prevValue = value;
    }
    return total;
}
std::string intToRoman(int num) {
    std::vector<std::pair<int, std::string>> values = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
        {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
    };
    std::string res;
    for (auto& val : values) {
        while (num >= val.first) {
            num -= val.first;
            res += val.second;
        }
    }
    return res;
}
std::unordered_map<char, std::string> textToMorse = {
    {'A', ".-"}, {'B', "-..."}, {'C', "-.-."}, {'D', "-.."}, {'E', "."},
    {'F', "..-."}, {'G', "--."}, {'H', "...."}, {'I', ".."}, {'J', ".---"},
    {'K', "-.-"}, {'L', ".-.."}, {'M', "--"}, {'N', "-."}, {'O', "---"},
    {'P', ".--."}, {'Q', "--.-"}, {'R', ".-."}, {'S', "..."}, {'T', "-"},
    {'U', "..-"}, {'V', "...-"}, {'W', ".--"}, {'X', "-..-"}, {'Y', "-.--"},
    {'Z', "--.."}, {'1', ".----"}, {'2', "..---"}, {'3', "...--"},
    {'4', "....-"}, {'5', "....."}, {'6', "-...."}, {'7', "--..."},
    {'8', "---.."}, {'9', "----."}, {'0', "-----"}, {' ', "/"}
};
std::unordered_map<std::string, char> morseToText;
std::string textToMorseCode(const std::string& text) {
    std::string result;
    for (char c : text) {
        auto search = textToMorse.find(toupper(c));
        if (search != textToMorse.end()) {
            result += search->second + " ";
        }
    }
    return result;
}
std::string morseCodeToText(const std::string& morseCode) {
    std::string result, token;
    std::istringstream tokenStream(morseCode);
    while (getline(tokenStream, token, ' ')) {
        auto search = morseToText.find(token);
        if (search != morseToText.end()) {
            result += search->second;
        }
        else if (token == "/") {
            result += ' ';
        }
    }
    return result;
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    for (const auto& pair : textToMorse)
        morseToText[pair.second] = pair.first;

    QFont titleFont("Arial", 18, QFont::Bold);
    QFont labelFont("Arial", 10);

    QWidget window;
    QVBoxLayout* mainLayout = new QVBoxLayout(&window);

    QLabel* titleLabel = new QLabel("SPY");
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    mainLayout->addSpacing(20);

    QGroupBox* conversionTypeGroup = new QGroupBox("Type of conversion");
    QVBoxLayout* conversionLayout = new QVBoxLayout(conversionTypeGroup);
    QRadioButton* arabToRomanButton = new QRadioButton("Arabic in Roman");
    QRadioButton* romanToArabButton = new QRadioButton("Roman in Arabic");
    QRadioButton* textToMorseButton = new QRadioButton("Text in Morse");
    QRadioButton* morseToTextButton = new QRadioButton("Morse in Text");
    conversionLayout->addWidget(arabToRomanButton);
    conversionLayout->addWidget(romanToArabButton);
    conversionLayout->addWidget(textToMorseButton);
    conversionLayout->addWidget(morseToTextButton);
    arabToRomanButton->setChecked(true);

    QLineEdit* inputEdit = new QLineEdit();
    inputEdit->setPlaceholderText("Type here...");
    QPushButton* convertButton = new QPushButton("Convert");
    QLabel* resultLabel = new QLabel("Result...");
    resultLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    mainLayout->addWidget(conversionTypeGroup);
    mainLayout->addWidget(inputEdit);
    mainLayout->addWidget(convertButton);
    mainLayout->addWidget(resultLabel);

    window.setStyleSheet(R"(
    QWidget { background-color: #333333; }
    QGroupBox { font-weight: bold; margin-top: 2ex; color: #CCCCCC; }
    QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 3px 0 3px; color: #CCCCCC; }
    QPushButton { background-color: #008000; font-weight: bold; border-radius: 5px; padding: 5px; color: #FFFFFF; }
    QPushButton:hover { background-color: #006400; }
    QLabel { color: #FFFFFF; }
    QLineEdit { border: 1px solid #777; border-radius: 4px; padding: 2px; background-color: #555; color: #FFF; }
    QRadioButton { font-weight: normal; color: #CCCCCC; }
    QRadioButton::indicator { width: 13px; height: 13px; }
    QRadioButton::indicator:checked { background-color: #4CAF50; }
    QRadioButton::indicator:unchecked { background-color: #777; }
    )");

    QObject::connect(convertButton, &QPushButton::clicked, [&]() {
        QString input = inputEdit->text();
        if (arabToRomanButton->isChecked()) {
            bool ok;
            int num = input.toInt(&ok);
            if (ok) {
                std::string roman = intToRoman(num);
                resultLabel->setText(QString::fromStdString(roman));
            }
            else {
                resultLabel->setText("Invalid input for Arabic to Roman conversion.");
            }
        }
        else if (romanToArabButton->isChecked()) {
            int result = romanToInt(input.toUpper().toStdString());
            resultLabel->setText(QString::number(result));
        }
        else if (textToMorseButton->isChecked()) {
            std::string morse = textToMorseCode(input.toUpper().toStdString());
            resultLabel->setText(QString::fromStdString(morse));
        }
        else if (morseToTextButton->isChecked()) {
            std::string text = morseCodeToText(input.toStdString());
            resultLabel->setText(QString::fromStdString(text));
        }
        });

    window.show();
    return app.exec();
}
