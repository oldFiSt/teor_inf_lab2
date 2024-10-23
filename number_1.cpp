#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <filesystem>

class Symbol {
public:
    char character;
    double probability;

    Symbol(char c, double p) : character(c), probability(p) {}
};

bool compareSymbols(const Symbol& a, const Symbol& b) {
    return a.probability > b.probability; // Сортировка по убыванию вероятности
}

void generateShannonFanoCodes(const std::vector<Symbol>& symbols, std::map<char, std::string>& codes, const std::string& code) {
    if (symbols.size() == 1) {
        codes[symbols[0].character] = code; // Присваиваем код единственному символу
        return;
    }

    // Суммируем вероятности
    double total = 0.0;
    for (const auto& symbol : symbols) {
        total += symbol.probability;
    }

    double cumulative = 0.0;
    size_t splitIndex = 0;
    double minDifference = total; // Инициализируем минимальную разницу

    for (size_t i = 0; i < symbols.size() - 1; ++i) { // Проходимся по всем элементам вектора
        cumulative += symbols[i].probability; // Текущая вероятность сложенных символов
        double difference = std::abs(total / 2 - cumulative); // Вычисляем разницу между элементами

        if (difference < minDifference){
            minDifference = difference;
            splitIndex = i + 1; // Индекс первого элемента второй половины
        }
    }

    generateShannonFanoCodes(std::vector<Symbol>(symbols.begin(), symbols.begin() + splitIndex), codes, code + "0");
    generateShannonFanoCodes(std::vector<Symbol>(symbols.begin() + splitIndex, symbols.end()), codes, code + "1");
}

// Функция для вычисления средней длины кода
double calculateAverageLength(const std::map<char, std::string>& codes, const std::vector<Symbol>& symbols) {
    double averageLength = 0.0;
    for (const auto& symbol : symbols) {
        averageLength += symbol.probability * codes.at(symbol.character).length();
    }
    return averageLength;
}

// Функция для вычисления энтропии
double calculateEntropy(const std::vector<Symbol>& symbols) {
    double entropy = 0.0;
    for (const auto& symbol : symbols) {
        if (symbol.probability > 0) { 
            entropy -= symbol.probability * log2(symbol.probability);
        }
    }
    return entropy;
}

int main() {
    std::vector<Symbol> symbols = {
        {'A', 0.26},
        {'B', 0.19},
        {'C', 0.14},
        {'D', 0.11},
        {'E', 0.10},
        {'F', 0.08},
        {'G', 0.07},
        {'H', 0.05}
    };

    std::sort(symbols.begin(), symbols.end(), compareSymbols);

    std::map<char, std::string> shannonFanoCodes;
    generateShannonFanoCodes(symbols, shannonFanoCodes, "");

    std::cout << "Коды Шеннона-Фано:\n";
    for (const auto& pair : shannonFanoCodes) {
        std::cout << pair.first << ": " << pair.second << "\n";
    }

    double averageLength = calculateAverageLength(shannonFanoCodes, symbols);
    std::cout << "Средняя длина кода: " << averageLength << "\n";

    double entropy = calculateEntropy(symbols);
    std::cout << "Энтропия: " << entropy << "\n";

    double redundancy =  1.0 - (entropy / log2(8));
    std::cout << "Избыточность: " << redundancy << "\n";
    return 0;
}