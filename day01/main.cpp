#include <fstream>
#include <cstdio>
using namespace std;

class FileStream
{
private:
    FILE* file_ = nullptr;
public:
    FileStream(char const* fname, char const* mode = "rb") {
		file_ = fopen(fname, mode);
        if (!file_) {
            printf("Failed to open file: %s\n", fname);
            exit(-1);
        }
	}
    ~FileStream() {
        if (file_) {
			fclose(file_);
		}
	}
	FileStream(FileStream const&) = delete;
	FileStream& operator=(FileStream const&) = delete;

    char get() {
		return (char)(uint8_t)fgetc(file_);
	}
    bool eof() {
		return feof(file_) != 0;
	}
};

//-----------------------------------------------------------------------------
void SolveFile(char const* fname) {
    FileStream f(fname);

    int first = -1;
    int last = -1;
    uint32_t part1 = 0;
    char buffer[4096];
    char* p = buffer;
    char const* numbers[] = {
        "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
    };

    while (!f.eof()) {
        auto c = (uint8_t)f.get();
        *p++ = (char)c;

        if (isdigit(c)) {
            if (first == -1) {
				first = c - '0';
			}
			last = c - '0';
		}
        else {
            // Part 2 adds this:
            std::string_view s(buffer, p);
            for (int i = 0; i < 10; ++i) {
                if (s.ends_with(numbers[i])) {
					*p = '\0';
                    if (first == -1) {
                        first = i;
                    }
                    last = i;
					break;
				}
			}
        }
        if (c == '\n') {
            auto num = first * 10 + last;
            part1 += num;
            first = -1;
            last = -1;
            p = buffer;
        }
    }

    printf("%d\n", part1);

    uint32_t part2 = 0; //
    printf("%d\n", part2);
}

//-----------------------------------------------------------------------------
int main() {
    SolveFile("input.txt");
    
    return 0;
}
