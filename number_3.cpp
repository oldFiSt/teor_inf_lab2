#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>

std::string readFromFile(std::string& filename) {
    std::ifstream inputFile(filename);
    std::string text;

    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            text += line; // Добавляем строку
        }
        inputFile.close();
    }
    else {
        std::cerr << "Не удалось открыть файл." << std::endl;
    }

    return text;
}

// Вспомогательная функция для дополнения строки пробелами до заданной длины
std::string padString(const std::string& str, int length) {
    if (str.length() < length) {
        return str + std::string(length - str.length(), ' '); // Дополняем пробелами
    }
    return str;
}

void countCombination(std::string& text,
    std::unordered_map<std::string, int>& frequencyMap, int number) {

    for (int i = 0; i < text.length(); i += number) {
        if (i + 1 <= text.length()) {
            std::string combination = padString(text.substr(i, number), number);
            frequencyMap[combination]++; 
        }
    }
}

void EnsembleAnalysis( std::unordered_map<std::string, int>& frequencyMap) {
    int totalCount = 0;

    // Подсчет общего количества комбинаций
    for (auto& pair : frequencyMap) {
        totalCount += pair.second;
    }
    double entropy = 0.0;
    // Вывод вероятностей для каждой комбинации
    std::cout << "Вероятности появления блоков:\n";
    for ( auto& pair : frequencyMap) {
        double probability = (double)pair.second / totalCount; 
        std::cout << "Комбинация: \"" << pair.first << "\", Вероятность: " << probability << std::endl;

        if (probability > 0) {
            entropy -= probability * log2(probability);
        }
    }
    std::cout << "Энтропия: " << entropy << std::endl;

    double maxEntropy = log2(frequencyMap.size());
    double redundancy = (entropy == 0.0 && maxEntropy == 0.0) ? (1 - 1) : (1 - entropy / maxEntropy);
    std::cout << "Избыточность: " << redundancy << std::endl;
    
    std::cout << "\n";
}

int main() {
    std::string filename = "input.txt";

    std::string text = readFromFile(filename);

    for (int i = 1; i < 6; i++){
        std::unordered_map<std::string, int> frequencyMap;
        countCombination(text, frequencyMap, i);
        std::cout << "Для блока длинной : " << i << '\n';
        EnsembleAnalysis(frequencyMap);
    }

    return 0;
}