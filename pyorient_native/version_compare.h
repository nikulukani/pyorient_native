/* Sam Caldwell <mail@samcaldwell.net> (released public domain to the project where this code resides).
 * Version Comparison Class.
 *
 * Given a version string in the form xxx.yyy.zzz.aaa, this class will parse the string
 * and allow version comparison operators to test equivalence, etc. Expected behavior is
 * defined in test_version_compare.cpp (in tests/ directory).
 *
 */
#ifndef VERSION_COMPARE_H
#define VERSION_COMPARE_H
#endif
#include <iostream>
#include <string>

using namespace std;

class Version {
public:
	Version(const char *c){
		string version(c);
		this->major = 0;
		this->minor = 0;
		this->revision = 0;
		this->build = 0;
		std::sscanf(version.c_str(), "%d.%d.%d.%d", &this->major, &this->minor, &this->revision, &this->build);
	}

	bool operator<(const Version rhs){
		if (this->major < rhs.major)
			return true;
		if (this->minor < rhs.minor)
			return true;
		if (this->revision < rhs.revision)
			return true;
		if (this->build < rhs.build)
			return true;
		return false;
	}

	bool operator>(const Version rhs){
		if (this->major > rhs.major)
			return true;
		if (this->minor > rhs.minor)
			return true;
		if (this->revision > rhs.revision)
			return true;
		if (this->build > rhs.build)
			return true;
		return false;
	}

	bool operator==(const Version rhs){
		return this->major == rhs.major
			   && this->minor == rhs.minor
			   && this->revision == rhs.revision
			   && this->build == rhs.build;
	}

	bool operator!=(const Version rhs){
		return this->major != rhs.major
			   && this->minor != rhs.minor
			   && this->revision != rhs.revision
			   && this->build != rhs.build;
	}

public:
	int major = 0;
	int minor = 0;
	int revision = 0;
	int build = 0;
};