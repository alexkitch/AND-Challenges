#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <minmax.h>

void wordWrap(const char* const buffer, const long bufferLength, std::vector<std::string>& output, const int maxLineLength)
{
    // This is a bit pants really, can definitely be optimised and tidied up.

    // Beginning with some variables to help us keep track of where we're scanning for characters
    int i = 0;
    int lineBegin = 0;
    int lineEnd = 0;

    do {
        // Count forwards as far as the maximum line length allows us
        lineEnd += maxLineLength - 1;
        lineEnd = min(lineEnd, bufferLength);

        // If the next character isn't a space then we're positioned inside a word.
        // We need to iterate backwards to the next wrappable character (either a space
        // or a hyphen)
        if (lineEnd < bufferLength && buffer[lineEnd + 1] != ' ')
        {
            while (buffer[lineEnd] != ' ' && buffer[lineEnd] != 0 && buffer[lineEnd] != '-')
            {
                lineEnd--;
            }
        }
        else lineEnd++; // Otherwise, the line ends at the next character (the space).

        // Keep the 'whitespace' (or hyphen) at the end of the line.
        const bool keepWhitespace = (buffer[lineEnd] == '-' || buffer[lineEnd] == ' ');

        // Safety to avoid trying to copy negative bytes around.
        int actualLineLength(lineEnd - lineBegin + (keepWhitespace ? 1 : 0));
        if (actualLineLength <= 0) {
            break;
        }

        // Allocate a buffer to hold the wrapped sentence and copy into that buffer.
        // Move the buffer into a new string object and push to our 'results' list
        const auto lineBuffer = new char[actualLineLength + 1];
        memcpy(lineBuffer, buffer + lineBegin, actualLineLength);
        lineBuffer[actualLineLength] = 0;
        output.push_back(std::string(std::move(lineBuffer)));

        // Move the cursor to the beginning of the next line
        lineBegin += actualLineLength + (keepWhitespace ? 0 : 1);
        i++;

    } while (lineEnd < bufferLength);

    return;
}

int main()
{
    std::ifstream input("input.txt");
    if (input.is_open())
    {
        // Get the file length in bytes
        input.seekg(0, std::ios_base::end);
        auto fileSize((long)input.tellg());
        input.seekg(0, std::ios_base::beg);

        // Allocate and zero a buffer large enough to hold the file, then read it in.
        const auto buffer = new char[fileSize];
        input.read(buffer, fileSize);
        buffer[fileSize] = 0;

        // Perform the word wrapping algorithm on the input
        std::vector<std::string> lines;
        wordWrap(buffer, fileSize, lines, 32);

        // Write the resulting lines to an output file, keeping track of 'full' lines
        int fullLines = 0;
        std::ofstream output("output.txt");
        for (auto& line : lines)
        {
            output << line << std::endl;
            std::cout << "(" << line.length() << ") '" << line << "'" << std::endl;
            if (line.length() == 32) {
                fullLines++;
            }
        }

        // Print the answer
        std::cout << "Full lines: " << fullLines << std::endl;
    }
    else 
    {
        std::cout << "Error reading input text" << std::endl;
    }

    system("PAUSE");
}
