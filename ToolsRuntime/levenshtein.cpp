#include <QString>
#include <QVector>
#include <algorithm>
#include "toolsruntime.h"

// Функция для вычисления расстояния Левенштейна
int toolLevenshteinDistance(const QString &s1, const QString &s2)
{
    const int len1 = s1.length();
    const int len2 = s2.length();
    QVector<QVector<int>> d(len1 + 1, QVector<int>(len2 + 1));

    for (int i = 0; i <= len1; ++i)
        d[i][0] = i;

    for (int j = 0; j <= len2; ++j)
        d[0][j] = j;

    for (int i = 1; i <= len1; ++i)
    {
        for (int j = 1; j <= len2; ++j)
        {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            d[i][j] = std::min({ d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + cost });
        }
    }

    return d[len1][len2];
}

// Функция для вычисления процента совпадения строк
double toolLevenshteinSimilarity(const QString &s1, const QString &s2)
{
    int maxLen = std::max(s1.length(), s2.length());
    if (maxLen == 0) return 100.0; // Если обе строки пустые, считаем их совпадающими на 100%
    int dist = toolLevenshteinDistance(s1, s2);

    return (1.0 - static_cast<double>(dist) / maxLen) * 100.0;
}
