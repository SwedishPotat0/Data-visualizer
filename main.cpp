#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

std::vector<int> x;
std::vector<int> y;

int scaleX;
int scaleY;

bool checkCordinates(int c, int r, int width, int height) {
	if (x.size() > 0) {
		for (int i = 0; i < x.size(); i++) {

			int scaledX = 1 + x[i] * (width-2) / scaleX;
			int scaledY = (height - 2) - y[i] * (height - 2) / scaleY;
			if(scaledX == c && scaledY == r) {
				return true;
			}
		}
	}

	return false;
}

void drawCoordinates(int width, int height) {
	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			if (r == height-1) {
				if(c == 0) {std::cout << "#";}
				else {std::cout << "-";}
			} else {
				if (c == 0) {std::cout << "|";}
				else {
					if (checkCordinates(c, r, width, height)) {
						std::cout << "+";
					}
					else {std::cout << " ";}
				}
			}
		}
		std::cout << '\n';
	}

}

void getData(std::string file) {
	std::filesystem::path path = file;
	std::string line;
	std::ifstream read(path);
	int l = 0;

	while (getline(read, line)) {
		if (l == 0) {
			bool space = false;
			std::string tempSX;
			std::string tempSY;
			for(int i = 0; i < line.length(); i++) {
				if (space) {
					tempSY += line[i];
				} else {
					if (line[i] == ' ') {
						space = true;
						continue;
					} 
					else { tempSX += line[i]; }
				}
			}
			scaleX = std::stoi(tempSX);
			scaleY = std::stoi(tempSY);
		} else {
			bool space = false;
			std::string tempX;
			std::string tempY;
			for (int i = 0; i < line.length(); i++) {
				if (space) {
					tempY += line[i];
				} else {
					if (line[i] == ' ') {
						space = true;
						continue;
					} 
					else { tempX += line[i]; }
				}
			} 
			x.push_back(std::stoi(tempX));
			y.push_back(std::stoi(tempY));
		}	
		l++;	
	}
}

int main(int argc, char* argv[]) {
	struct winsize size;

	if (argc < 2) { return 1; }
	getData(argv[1]);
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == 0) {
		drawCoordinates(size.ws_col, size.ws_row);
	}
	return 0;
}
