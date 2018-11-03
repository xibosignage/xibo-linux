#pragma once

#include <cmath>

class SizeScaler
{
public:
    SizeScaler(int originalWidth, int originalHeight) :
        m_originalWidth(originalWidth), m_originalHeight(originalHeight)
    {
    }

    std::pair<int, int> scaleToSize(int width, int height)
    {
        double scaleFactor = calculateScaleFactor(width, height);

        int scaledWidth = static_cast<int>(m_originalWidth / scaleFactor);
        int scaledHeight = static_cast<int>(m_originalHeight / scaleFactor);

        return {scaledWidth, scaledHeight};
    }

private:
    double calculateScaleFactor(int width, int height)
    {
        return std::max(m_originalWidth / static_cast<double>(width), m_originalHeight / static_cast<double>(height));
    }

private:
    int m_originalWidth;
    int m_originalHeight;

};
