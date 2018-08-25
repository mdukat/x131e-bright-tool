#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string.h>

#define brightness_file "/sys/devices/pci0000:00/0000:00:02.0/drm/card0/card0-LVDS-1/intel_backlight/brightness"
#define uint unsigned int

uint percentToBrightness(uint percent);
void brightnessSet(uint percent);
void brightnessIncrement(uint percent);
void brightnessDecrement(uint percent);

void checkRoot();
uint getValue();
void setValue(uint value);
void raiseError(char* value);
void raiseHelp();
void checkPercentageCorrectness(uint percent);

uint percentToBrightness(uint percent){
	return 43*percent;
}

void brightnessSet(uint percent){
	checkPercentageCorrectness(percent);
	setValue(percentToBrightness(percent));
}

void brightnessIncrement(uint percent){
	checkPercentageCorrectness(percent);
	uint last_state = getValue();
	uint new_state = last_state + percentToBrightness(percent);
	if(new_state > 4300){
		throw 3;
	}else{
		setValue(new_state);
	}
}

void brightnessDecrement(uint percent){
	checkPercentageCorrectness(percent);
	uint last_state = getValue();
	int new_state = last_state - percentToBrightness(percent);
	if(new_state < 0){
		throw 3;
	}else{
		setValue(new_state);
	}
}

uint getValue(){
	std::ifstream b_file;
	b_file.open(brightness_file, std::ios::in);
	std::string buffer;
	getline(b_file, buffer);
	int int_buffer;
	int_buffer = std::stoi(buffer);
	b_file.close();
	return int_buffer;
}

void setValue(uint value){
	std::ofstream b_file;
	b_file.open(brightness_file, std::ios::in);
	b_file << value;
	b_file.close();
}

void checkRoot(){
	if(getuid() != 0){
		throw 0;
	}
}

void checkPercentageCorrectness(uint percent){
	if(percent < 0){
		throw 2;
	}else if(percent > 100){
		throw 3;
	}
}

void raiseError(char* value){
	std::cout << "ERROR: " << value << "\n";
}

void raiseHelp(){
	std::cout << "bright_tool - X131e brightness tool by d3s\nUsage: bright_tool <command> <value>\n\nCommands:\n-i <0-100> - increments brightness by %\n-d <0-100> - decrements brightness by %\n-s <0-100> - sets brightness to %\n-h - shows this help\n\n";
}

int main(int argc, char** argv){
	try{
	checkRoot();
	int c;
	while((c = getopt(argc, argv, "i:d:hs:")) != -1){
		switch(c){
			case 'i':
				brightnessIncrement(atoi(optarg));
				break;
			case 'd':
				brightnessDecrement(atoi(optarg));
				break;
			case 'h':
				throw 1;
				break;
			case 's':
				brightnessSet(atoi(optarg));
				break;
			case '?':
				throw 1;
				break;
			default:
				abort();
		}
	}
	}catch(int errnum){
		if(errnum == 0)
			raiseError("You are not root!");
		if(errnum == 1)
			raiseHelp();
		if(errnum == 2)
			raiseError("Value too low!");
		if(errnum == 3)
			raiseError("Value too high!");
	}
	return 0;
}
