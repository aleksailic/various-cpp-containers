#pragma once
class Exception {
	const char * msg;
public:
	Exception() :msg("Default exception") {}
	Exception(const char * cmsg) :msg(cmsg) {}
	const char* getMsg()const { return msg; }
};