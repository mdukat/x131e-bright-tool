#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#define brightness_file "/sys/devices/pci0000:00/0000:00:02.0/drm/card0/card0-LVDS-1/intel_backlight/brightness"

int some_math(int percent);
void brightness_set(int percent);
void brightness_increment(int percent);
void brightness_decrement(int percent);
void help();
bool check_root();
void r_not_root();
int get_value();
void set_value(int value);
void r_too_high();
void r_too_low();

int some_math(int percent){
	percent = 43*percent;
	return percent;
}

void brightness_set(int percent){
	set_value(some_math(percent));
	std::cout << "Screen brightness set to " << percent << "% (" << some_math(percent) << ")\n";
}

void brightness_increment(int percent){
	int last_state = get_value();
	if(last_state > 4300){
		r_too_high();
	}else{
		int new_state = last_state + some_math(percent);
		set_value(new_state);
	}
}

void brightness_decrement(int percent){
	int last_state = get_value();
	if(last_state <= 0){
		r_too_low();
	}else{
		int new_state = last_state - some_math(percent);
		set_value(new_state);
	}
}

void set_value(int value){
	std::ofstream b_file;
	b_file.open(brightness_file, std::ios::out);
	b_file << value;
	b_file.close();
}

int get_value(){
	std::ifstream b_file;
	b_file.open(brightness_file, std::ios::in);
	std::string buffer;
	getline(b_file, buffer);
	int int_buffer;
	int_buffer = std::stoi(buffer);
	b_file.close();
	return int_buffer;
}

bool check_root(){
	std::ofstream b_file;
	b_file.open(brightness_file, std::ios::out);
	if(b_file){
		b_file.close();
		return true;
	}else{
		r_not_root();
		return false;
	}
}

void help(){
	std::cout << "bright_tool - X131e brightness tool by d3s\nUsage: bright_tool <command> <value>\n\nCommands:\n-i <0-100> - increments brightness by %\n-d <0-100> - decrements brightness by %\n-h - shows this help\n\nif no command is given, brigh_tool will set brightness to %\n\n";
}

void r_not_root(){
	std::cout << "Error: you are not root :/\n";
}

void r_too_high(){
	std::cout << "Error: value too high\n";
}

void r_too_low(){
	std::cout << "Error: value too low\n";
}

int main(int argc, const char * argv[]){
	if(argv[1] == NULL){
		help();
	}else{
	if(check_root() == false){
		return 1;
	}else if(argv[2] == NULL){
		if(strcmp(argv[1], "-h") == 0){
			help();
		}else{
			brightness_set(atoi(argv[1]));
		}
	}else{
		if(strcmp(argv[1], "-i") == 0){
			brightness_increment(atoi(argv[2]));
		}else if(strcmp(argv[1], "-d") == 0){
			brightness_decrement(atoi(argv[2]));
		}
	}
	}
}
